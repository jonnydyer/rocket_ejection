/*
 * lis331.c
 * Source file for lis331 driver.
 * Datasheet available at http://www.st.com/resource/en/datasheet/h3lis331dl.pdf
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#include <avrlib/i2c.h>
#include <util/delay.h>
#include "lis331.h"

static int8_t lis331_write_reg(uint8_t sub, uint8_t val);
static uint8_t lis331_read_reg(uint8_t sub);

int8_t lis331_init(void){
    int8_t retval = I2C_OK;

    //retval = lis331_write_reg(LIS331_REG_CTRL_REG2, 0x00);
    //if(retval != I2C_OK) return retval;
    //
    // Wait for BOOT bit to clear on startup
    while(lis331_read_reg(LIS331_REG_CTRL_REG2) & LIS331_BOOT){
        _delay_ms(1);
    }

    retval = lis331_write_reg(LIS331_REG_CTRL_REG4, LIS331_BDU | H3LIS331_FS_100G);
    if(retval != I2C_OK) return retval;

    retval = lis331_write_reg(LIS331_REG_CTRL_REG3, LIS331_I1_CFG1);    // INT1 configured for Data Ready
    if(retval != I2C_OK) return retval;

    retval = lis331_write_reg(LIS331_REG_CTRL_REG1, LIS331_POWER_NORM | 
                                                    LIS331_DATA_RATE_400 | 
                                                    LIS331_X_ENABLE | 
                                                    LIS331_Y_ENABLE | 
                                                    LIS331_Z_ENABLE);
    if(retval != I2C_OK) return retval;

    return retval;
}

uint8_t lis331_data_status(void){
    return lis331_read_reg(LIS331_REG_STATUS_REG);
}

uint8_t lis331_data_ready(void){
    return (EXTINT_PIN & EXTINT_LIS331_PIN) > 0;
}

int8_t lis331_read(int16_t *x, int16_t *y, int16_t *z){
    int8_t retval = I2C_OK;
    
    // First send address
    g_buff[0] = 0x80 | LIS331_REG_OUT_X_L; //0x80 | for multi-byte read
    retval = i2cMasterSend(LIS331_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;
    
    // Now receive
    retval = i2cMasterReceive(LIS331_ADDR, 6, g_buff);
    if(retval != I2C_OK) return retval;

    *x = (int16_t)((uint16_t)g_buff[0] | ((uint16_t)g_buff[1]<<8)) >> 4;
    *y = (int16_t)((uint16_t)g_buff[2] | ((uint16_t)g_buff[3]<<8)) >> 4;
    *z = (int16_t)((uint16_t)g_buff[4] | ((uint16_t)g_buff[5]<<8)) >> 4;

    return retval;
}

/* lis331_write_reg
 * writes a register at addr
 */
static int8_t lis331_write_reg(uint8_t sub, uint8_t val){
    g_buff[0] = sub;
    g_buff[1] = val;
    return i2cMasterSend(LIS331_ADDR, 2, g_buff);
}

/* lis331_read_reg
 * writes a register at addr
 */
static uint8_t lis331_read_reg(uint8_t sub){
    int8_t retval = I2C_OK;
    
    // First send address
    g_buff[0] = sub;
    retval = i2cMasterSend(LIS331_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;
    
    // Now receive
    retval = i2cMasterReceive(LIS331_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;

    return g_buff[0];
}
