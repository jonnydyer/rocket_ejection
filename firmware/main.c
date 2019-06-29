//
//  LLUA.c
//  LLUA


#include "global.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>     /* for _delay_ms() */

#include <swTimer.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "requests.h"       /* The custom request numbers we use */

#include "avrlib/i2c.h"
#include "avrlib/buffer.h"

#include "bmp280.h"
#include "lis331.h"
#include "lsm303d.h"
#include "serialize.h"

// Defines
#define LED_MASK ((1<<LED_RED_PIN) | (1<<LED_GREEN_PIN) | (1<<LED_BLUE_PIN))
#define RED_BLUE_MASK ((1<<LED_RED_PIN) | (1<<LED_BLUE_PIN))
#define TOGGLE_LED(x) PORTD = (PORTD & ~(x)) | (PORTD ^ (x))

#define TOTAL_BUFF_LEN (3*STREAM_BUFF_LEN*sizeof(int32_t) + 9*STREAM_BUFF_LEN*sizeof(int16_t))

// Constants

typedef struct ma16_filt{
    int16_t buff[MA_FILT_LEN];
    uint8_t ind;
    int16_t val;
}ma16_filt_t;

uint8_t sequence[][2] = {
	{~(1<<LED_RED_PIN), 10},
	{~(1<<LED_BLUE_PIN), 10},
	{(1<<LED_RED_PIN) | (1<<LED_BLUE_PIN) | (1<<LED_GREEN_PIN), 100},
};

uint8_t seq_ind, g_i2c_dev_idx;
sw_timer_t led_timer;
sw_timer_t bmp280_timer;
cBuffer g_stream;
uint8_t g_stream_buff[TOTAL_BUFF_LEN];
uint8_t g_i2c_dev_addrs[4];

typedef enum event
{
	no_event=0,
	SW_1_press,
	SW_2_press
}event_t;


void init(void);
void idle_task(void);
void init_USB(void);
void enumerateI2C(void);
void add_to_filt(ma16_filt_t *filt, int16_t val);
event_t check_events(void);
void jump_to_bootloader(void);

int main (void) 
{
	event_t ev;
	init();
	while(1)
	{
		while((ev = check_events()))
		{
			switch(ev)
			{
				case no_event:
					break;
				case SW_1_press:
                    TOGGLE_LED(1<<LED_GREEN_PIN);
					break;
				case SW_2_press:
					jump_to_bootloader();
					break;
				default:
					break;
			}
		}
        wdt_reset();
        usbPoll();
		idle_task();
	}
	return 0;
}


usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;
    static uchar    dataBuffer[TOTAL_BUFF_LEN];  /* buffer must stay valid when usbFunctionSetup returns */
    static uint8_t i;
    static int16_t temp;

    if(rq->bRequest == CUSTOM_RQ_ECHO){ /* echo -- used for reliability tests */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 4;
    }else if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
        if(rq->wValue.bytes[0] & 1){    /* set LED */
            PORTD &= ~(1<<LED_GREEN_PIN);
        }else{                          /* clear LED */
            PORTD |= (1<<LED_GREEN_PIN);
        }
    }else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
        dataBuffer[0] = ((PORTD & (1<<LED_GREEN_PIN)) != 0);
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 1;                       /* tell the driver to send 1 byte */
    }
    else if(rq->bRequest == CUSTOM_RQ_GET_DATA){
        for(i=0; i<TOTAL_BUFF_LEN; i++)
        {
            temp = bufferGetFromFront(&g_stream);
            if(temp >= 0){
                dataBuffer[i] = temp;
            }
            else
                break;
        }
        usbMsgPtr = dataBuffer;
        return i+1;
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

void init(void)
{
    init_USB();

	swTimer_init();
	DDRD |= LED_MASK;
	PORTD |= LED_MASK | (1<<SW_2_PIN);
	PORTB |= (1<<SW_1_PIN);

    EXTINT_DDR &= (~EXTINT_LIS331_PIN) & (~EXTINT_LSM303_PIN);

    I2C_DDR &= (~I2C_SCL) & (~I2C_SDA);
    I2C_PORT |= I2C_SCL | I2C_SDA;
    i2cReset(); 
    i2cSetBitrate(400);
    enumerateI2C();

    bmp280_init();
    lis331_init();
    lsm303_init();
    
    bufferInit(&g_stream, g_stream_buff, TOTAL_BUFF_LEN);

    sei();       // THis already happens in swTimer_init() but jsut in case....

    wdt_enable(WDTO_1S);

	swTimer_set(&led_timer, 100);
	swTimer_start(&led_timer);

    swTimer_set(&bmp280_timer, 2);
    swTimer_start(&bmp280_timer);
}

void init_USB(void)
{
    uint8_t i;
    usbInit();
    usbDeviceDisconnect();
    // Wait 250 ms
    i = 0;
    while(--i){
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
}

void idle_task(void)
{
    static int32_t T, P;
    static int16_t x, y, z, x2, y2, z2, mx, my, mz;
    static ma16_filt_t xfilt, yfilt, zfilt;
    static uint32_t time;
	if(swTimer_expired(&led_timer)){
		seq_ind++;
		if(seq_ind > 2)
			seq_ind = 0;
		PORTD = (~RED_BLUE_MASK & PORTD) | (RED_BLUE_MASK & sequence[seq_ind][0]);
		swTimer_set(&led_timer, sequence[seq_ind][1]);
		swTimer_start(&led_timer);
	}

    if(swTimer_expired(&bmp280_timer)){
        time = getSysTicks();
        bmp280_get_temp_press(&T, &P);

        if(lsm303_mag_data_status() & LSM303_ZYXDA){
            lsm303_read_mag(&mx, &my, &mz);
        }

        serialize_uint32(&g_stream, &time);
        serialize_int32(&g_stream, &T);
        serialize_int32(&g_stream, &P);
        serialize_int16(&g_stream, &(xfilt.val));
        serialize_int16(&g_stream, &(yfilt.val));
        serialize_int16(&g_stream, &(zfilt.val));
        serialize_int16(&g_stream, &x2);
        serialize_int16(&g_stream, &y2);
        serialize_int16(&g_stream, &z2);
        serialize_int16(&g_stream, &mx);
        serialize_int16(&g_stream, &my);
        serialize_int16(&g_stream, &mz);
        swTimer_restart(&bmp280_timer);
    }
    if(lis331_data_ready()){
        lis331_read(&x, &y, &z); 
        add_to_filt(&xfilt, x);
        add_to_filt(&yfilt, y);
        add_to_filt(&zfilt, z);
    }
    if(lsm303_data_ready_int()){
        lsm303_read_accel(&x2, &y2, &z2);
    }
}

void add_to_filt(ma16_filt_t *filt, int16_t val){
    if(filt->ind < (MA_FILT_LEN - 1)){
        filt->val += val - filt->buff[filt->ind];
        filt->buff[filt->ind] = val;
        filt->ind++;
    }
    else{
        filt->val += val - filt->buff[0];
        filt->buff[0] = val;
        filt->ind = 1;
    }
}
void enumerateI2C(void)
{
    uint8_t addr;
    int8_t retval;

    PORTD &= ~(1<<LED_GREEN_PIN);
    for(addr=0; addr<0x7F; addr++)
    {
        
        TWCR &= ~BV(TWIE);
	    // send start condition
	    i2cSendStart();
	    retval = i2cWaitForComplete();
	    if(retval != I2C_OK) break;

	    // send device address with write
	    i2cSendByte( (addr<<1) & 0xFE );
	    retval = i2cWaitForComplete();
	    if(retval != I2C_OK) break;
	    if( TWSR == TW_MT_SLA_ACK)
        {
            g_i2c_dev_addrs[g_i2c_dev_idx++] = addr;   
        }
        
	    i2cSendStop();
        TWCR |= BV(TWIE);
        _delay_us(100);
    }
    PORTD |= (1<<LED_GREEN_PIN);
}

event_t check_events(void)
{
	static uint8_t PIND_state = 0xff;
	static uint8_t PINB_state = 0xff;
	
	if(! (PINB & (1<<SW_1_PIN)))
	{
		if(PINB_state & (1<<SW_1_PIN))
		{
			PINB_state = PINB;
			return SW_1_press;
		}
	}
	
	if(! (PIND & (1<<SW_2_PIN)))
	{
		if(PIND_state & (1<<SW_2_PIN))
		{
			PIND_state = PIND;
			return SW_2_press;
		}
	}
	PINB_state = PINB;
	PIND_state = PIND;
	return no_event;
}

void jump_to_bootloader(void)
{
    cli();
    // This makes custom USBasploader come up.
    MCUSR = 0;

    // ATmega168PA
    // initialize ports
    PORTB = 0; PORTC= 0; PORTD = 0;
    DDRB = 0; DDRC= 0; DDRD = 0;

    // disable interrupts
    EIMSK = 0; EECR = 0; SPCR = 0;
    ACSR = 0; SPMCSR = 0; WDTCSR = 0; PCICR = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0;
    ADCSRA = 0; TWCR = 0; UCSR0B = 0;

    // Boot Loader Section Start Address:
    // BOOTSZ       Size        Address
    // (lock bit)   (word)      (word)      (byte)
    // '11'         128         0x1F80      0x3F00
    // '10'         256         0x1F00      0x3E00
    // '01'         512         0x1E00      0x3C00
    // '00'         1024        0x1C00      0x3800
    asm volatile("jmp 0x3800");
}
