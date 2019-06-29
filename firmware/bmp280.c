/*
 * bmp280.c
 * Source file for bmp280 driver.
 * Datasheet available at https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#include <stdint.h>
#include <avrlib/i2c.h>
#include <avr/delay.h>
#include "bmp280.h"
#include "i2cconf.h"

uint8_t g_dev_addr;
int32_t g_tfine;
struct compensation_struct{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} g_comp;

int32_t read_20bit_reg(uint8_t start_addr);
int32_t compute_temp(int32_t adc_T);
int32_t compute_pressure(int32_t adc_P);

/* bmp280_init
 * Initializes device at slave address dev_addr
 */
uint8_t bmp280_init(void)
{
    int8_t retval = I2C_OK;
    g_dev_addr = BMP280_ADDR<<1;
    
    retval = bmp280_write_reg(BMP280_REG_RESET, BMP280_RESET);
    if(retval != I2C_OK) return retval;

    // Seem to need about 10ms to get device to reset
    _delay_ms(10);
    
    retval = bmp280_load_compensation();
    if(retval != I2C_OK) return retval;

    retval = bmp280_read_regs(BMP280_REG_ID, 1);
    if(retval != I2C_OK) return retval;

    retval = bmp280_write_reg(BMP280_REG_CONFIG, 
                ((BMP280_ODR_0_5_MS<<BMP280_STANDBY_DURN_POS) & BMP280_STANDBY_DURN_MASK) | ((BMP280_IIR_16<<BMP280_FILTER_POS) & BMP280_FILTER_MASK));
    if(retval != I2C_OK) return retval;
    
    retval = bmp280_write_reg(BMP280_REG_CTRL_MEAS,
            ((BMP280_OS_2<<BMP280_OS_TEMP_POS) & BMP280_OS_TEMP_MASK) |
            ((BMP280_OS_16<<BMP280_OS_PRES_POS) & BMP280_OS_PRES_MASK) | 
            ((BMP280_NORMAL_MODE<<BMP280_POWER_MODE_POS) & BMP280_POWER_MODE_MASK));
    if(retval != I2C_OK) return retval;

    return g_buff[0];
}

uint8_t bmp280_busy(void){
    bmp280_read_regs(BMP280_REG_STATUS, 1);
    return (g_buff[0] != 0);
}

/* bmp280_load_compensation
 * This loads all the compensation coefficients from the device.
 * Probably a more elegant way to do this but this closely mirrors
 * datasheet so I think readibility is better here.
 */
int8_t bmp280_load_compensation(void){
    int8_t retval = I2C_OK;

    retval = bmp280_read_regs(BMP280_REG_DIG_T1, 2);
    g_comp.dig_T1 = (uint16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_T2, 2);
    g_comp.dig_T2 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_T3, 2);
    g_comp.dig_T3 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P1, 2);
    g_comp.dig_P1 = (uint16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P2, 2);
    g_comp.dig_P2 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P3, 2);
    g_comp.dig_P3 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P4, 2);
    g_comp.dig_P4 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P5, 2);
    g_comp.dig_P5 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P6, 2);
    g_comp.dig_P6 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P7, 2);
    g_comp.dig_P7 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P8, 2);
    g_comp.dig_P8 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));

    retval = bmp280_read_regs(BMP280_REG_DIG_P9, 2);
    g_comp.dig_P9 = (int16_t)((uint16_t)g_buff[0] | (g_buff[1]<<8));
    return retval;
}

int32_t read_20bit_reg(uint8_t start_addr){
    bmp280_read_regs(start_addr, 3);
    return (int32_t)(((uint32_t)g_buff[0]<<12) | (g_buff[1]<<4) | (g_buff[2] >>4));
}

int32_t compute_temp(int32_t adc_T){
    int32_t var1, var2;

    var1 = ((((adc_T >> 3) - ((int32_t)g_comp.dig_T1 << 1)))
		* ((int32_t)g_comp.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)g_comp.dig_T1))
		* ((adc_T >> 4) - ((int32_t)g_comp.dig_T1))) >> 12)
		* ((int32_t)g_comp.dig_T3)) >> 14;

    g_tfine = var1 + var2;
    return (g_tfine * 5 + 128) >> 8;
}

int32_t compute_pressure(int32_t adc_P){
    int32_t var1, var2;
    uint32_t pressure = 0;

    var1 = (((int32_t) g_tfine) >> 1) - (int32_t) 64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) g_comp.dig_P6);
	var2 = var2 + ((var1 * ((int32_t) g_comp.dig_P5)) << 1);
	var2 = (var2 >> 2) + (((int32_t) g_comp.dig_P4) << 16);
	var1 = (((g_comp.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3)
	+ ((((int32_t) g_comp.dig_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int32_t) g_comp.dig_P1)) >> 15);
	pressure = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125;

	/* Avoid exception caused by division with zero */
	if (var1 != 0) {
		/* Check for overflows against UINT32_MAX/2; if pres is left-shifted by 1 */
		if (pressure < 0x80000000)
			pressure = (pressure << 1) / ((uint32_t) var1);
		else
			pressure = (pressure / (uint32_t) var1) * 2;

		var1 = (((int32_t) g_comp.dig_P9)
		* ((int32_t) (((pressure >> 3) * (pressure >> 3)) >> 13))) >> 12;
		var2 = (((int32_t) (pressure >> 2)) * ((int32_t) g_comp.dig_P8)) >> 13;
		pressure = (uint32_t) ((int32_t) pressure + ((var1 + var2 + g_comp.dig_P7) >> 4));
	} else {
		pressure = 0;
	}
	return pressure;
}

/* bmp280_get_temp
 * Returns temperature measured by device in decicentigrade (.01C / DN)
 */
int32_t bmp280_get_temp(void){
    int32_t adc_T;

    adc_T = read_20bit_reg(BMP280_REG_TEMP_MSB);
    return compute_temp(adc_T);
}

/* bmp280_get_pressure
 * Returns pressure measured by device in Pascal (1Pa / DN)
 */
int32_t bmp280_get_pressure(void){
    int32_t adc_P;
    
    bmp280_get_temp();
    adc_P = read_20bit_reg(BMP280_REG_PRESS_MSB);
    return compute_pressure(adc_P);
}

void bmp280_get_temp_press(int32_t *t, int32_t *p){
    int32_t adc_T, adc_P;
    bmp280_read_regs(BMP280_REG_PRESS_MSB, 6);
    adc_P = (int32_t)(((uint32_t)g_buff[0]<<12) | (g_buff[1]<<4) | (g_buff[2] >>4));
    adc_T = (int32_t)(((uint32_t)g_buff[3]<<12) | (g_buff[4]<<4) | (g_buff[5] >>4));

    *t = compute_temp(adc_T);
    *p = compute_pressure(adc_P);
}
/* bmp280_read_regs
 * Reads len 8bit registers starting at addr
 */

int8_t bmp280_read_regs(uint8_t addr, uint8_t len)
{
    int8_t retval = I2C_OK;

    // First send address
    g_buff[0] = addr;
    retval = i2cMasterSend(g_dev_addr, 1, g_buff);
    if(retval != I2C_OK) return retval;
    
    // Now receive
    retval = i2cMasterReceive(g_dev_addr, len, g_buff);
    return retval;
}


/* bmp280_write_reg
 * Writes a register at addr
 */
int8_t bmp280_write_reg(uint8_t addr, uint8_t val)
{
    g_buff[0] = addr;
    g_buff[1] = val;
    return i2cMasterSend(g_dev_addr, 2, g_buff);
}
