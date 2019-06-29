/*
 * lsm303d.c
 * Source file for lsm303d driver.
 * Datasheet available at http://www.st.com/resource/en/datasheet/lsm303d.pdf 
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#include <avrlib/i2c.h>
#include <util/delay.h>
#include "lsm303d.h"

static int8_t lsm303_write_reg(uint8_t sub, uint8_t val);
static uint8_t lsm303_read_reg(uint8_t sub);
static uint8_t lsm303_read_regs(uint8_t sub, uint8_t len);

int8_t lsm303_init(void){
    int8_t retval = I2C_OK;

    // Leaving CRTL0,CTRL4,CTRL7 at default values...

    retval = lsm303_write_reg(LSM303_REG_CTRL1, LSM303_BDU |
                                                LSM303_AXEN |
                                                LSM303_AYEN |
                                                LSM303_AZEN |
                                                LSM303_A_DR_100HZ);
    if(retval != I2C_OK) return retval;

    retval = lsm303_write_reg(LSM303_REG_CTRL2, LSM303_A_FS_16G |
                                                LSM303_A_BW_50HZ);
    if(retval != I2C_OK) return retval;

    retval = lsm303_write_reg(LSM303_REG_CTRL3, LSM303_INT1_DRDY_A);
    if(retval != I2C_OK) return retval;

    retval = lsm303_write_reg(LSM303_REG_CTRL5, LSM303_M_HI_RES |
                                                LSM303_M_DR_50HZ | 
                                                LSM303_TEMP_EN);
    if(retval != I2C_OK) return retval;

    retval = lsm303_write_reg(LSM303_REG_CTRL6, LSM303_M_FS_4GAUSS);
    if(retval != I2C_OK) return retval;

    retval = lsm303_write_reg(LSM303_REG_CTRL7, LSM303_M_CONT_CONV);
    if(retval != I2C_OK) return retval;

    return retval;
}

uint8_t lsm303_accel_data_status(void){
    return lsm303_read_reg(LSM303_REG_STATUS_A);
}

uint8_t lsm303_mag_data_status(void){
    return lsm303_read_reg(LSM303_REG_STATUS_M);
}

uint8_t lsm303_data_ready_int(void){
    return (EXTINT_PIN & EXTINT_LSM303_PIN) > 0;
}

int8_t lsm303_read_accel(int16_t *x, int16_t *y, int16_t *z){
    int8_t retval = I2C_OK;
    retval = lsm303_read_regs(LSM303_REG_OUT_X_L_A, 6);
    if(retval != I2C_OK) return retval;

    *x = (int16_t)((uint16_t)g_buff[0] | ((uint16_t)g_buff[1]<<8));
    *y = (int16_t)((uint16_t)g_buff[2] | ((uint16_t)g_buff[3]<<8));
    *z = (int16_t)((uint16_t)g_buff[4] | ((uint16_t)g_buff[5]<<8));

    return retval;
}

int8_t lsm303_read_mag(int16_t *x, int16_t *y, int16_t *z){
    int8_t retval = I2C_OK;
    retval = lsm303_read_regs(LSM303_REG_OUT_X_L_M, 6);
    if(retval != I2C_OK) return retval;

    *x = (int16_t)((uint16_t)g_buff[0] | ((uint16_t)g_buff[1]<<8));
    *y = (int16_t)((uint16_t)g_buff[2] | ((uint16_t)g_buff[3]<<8));
    *z = (int16_t)((uint16_t)g_buff[4] | ((uint16_t)g_buff[5]<<8));

    return retval;
}

static uint8_t lsm303_read_regs(uint8_t sub, uint8_t len){
    int8_t retval = I2C_OK;
    
    // First send address
    g_buff[0] = 0x80 | sub; //0x80 | for multi-byte read
    retval = i2cMasterSend(LSM303D_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;
    
    // Now receive
    retval = i2cMasterReceive(LSM303D_ADDR, len, g_buff);
    if(retval != I2C_OK) return retval;

    return retval;
}

/* lsm303_write_reg
 * writes a register at addr
 */
static int8_t lsm303_write_reg(uint8_t sub, uint8_t val){
    g_buff[0] = sub;
    g_buff[1] = val;
    return i2cMasterSend(LSM303D_ADDR, 2, g_buff);
}

/* lsm303_read_reg
 * writes a register at addr
 */
static uint8_t lsm303_read_reg(uint8_t sub){
    int8_t retval = I2C_OK;
    
    // First send address
    g_buff[0] = sub;
    retval = i2cMasterSend(LSM303D_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;
    
    // Now receive
    retval = i2cMasterReceive(LSM303D_ADDR, 1, g_buff);
    if(retval != I2C_OK) return retval;

    return g_buff[0];
}
