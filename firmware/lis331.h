
/*
 * lis331.h
 * Header file for lis331 driver.
 * Datasheet available at http://www.st.com/resource/en/datasheet/h3lis331dl.pdf
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#ifndef __LIS331_H__
#define __LIS331_H__

//I2C Address
#define LIS331_ADDR 0x32

//Registers
#define LIS331_REG_WHO_AM_I 0x0F
#define LIS331_REG_CTRL_REG1 0x20
#define LIS331_REG_CTRL_REG2 0x21
#define LIS331_REG_CTRL_REG3 0x22
#define LIS331_REG_CTRL_REG4 0x23
#define LIS331_REG_CTRL_REG5 0x24
#define LIS331_REG_HP_FILTER_RESET 0x25
#define LIS331_REG_REFERENCE 0x26
#define LIS331_REG_STATUS_REG 0x27
#define LIS331_REG_OUT_X_L 0x28
#define LIS331_REG_OUT_X_H 0x29
#define LIS331_REG_OUT_Y_L 0x2A
#define LIS331_REG_OUT_Y_H 0x2B
#define LIS331_REG_OUT_Z_L 0x2C
#define LIS331_REG_OUT_Z_H 0x2D
#define LIS331_REG_INT1_CFG 0x30
#define LIS331_REG_INT1_SRC 0x31
#define LIS331_REG_INT1_THS 0x32
#define LIS331_REG_INT1_DURATION 0x33
#define LIS331_REG_INT2_CFG 0x34
#define LIS331_REG_INT2_SRC 0x35
#define LIS331_REG_INT2_THS 0x36
#define LIS331_REG_INT2_DURATION 0x37

// CTRL_REG1
// Power Modes
#define LIS331_POWER_OFF 0x00
#define LIS331_POWER_NORM 0x20 
#define LIS331_POWER_LOW_0_5 0x40 
#define LIS331_POWER_LOW_1 0x60 
#define LIS331_POWER_LOW_2 0x80 
#define LIS331_POWER_LOW_5 0xA0 
#define LIS331_POWER_LOW_10 0xC0 

// Data Rates
#define LIS331_DATA_RATE_50 0x00
#define LIS331_DATA_RATE_100 0x08
#define LIS331_DATA_RATE_400 0x10
#define LIS331_DATA_RATE_1000 0x18


// Enable and disable channel.
#define LIS331_Z_ENABLE 0x04
#define LIS331_Z_DISABLE 0x00
#define LIS331_Y_ENABLE 0x02
#define LIS331_Y_DISABLE 0x00
#define LIS331_X_ENABLE 0x01
#define LIS331_X_DISABLE 0x00

// CTRL_REG2
#define LIS331_BOOT 0x80
#define LIS331_HPM_NORMAL 0x00
#define LIS331_HPM_REF_SIG 0x20
#define LIS331_FILTER_DATA 0x10
#define LIS331_HPEN2 0x08
#define LIS331_HPEN1 0x04
#define LIS331_HPC_8 0x00
#define LIS331_HPC_16 0x01
#define LIS331_HPC_32 0x02
#define LIS331_HPC_64 0x03

// CTRL_REG_#
#define LIS331_IHL 0x80
#define LIS331_PP_OD 0x40
#define LIS331_LIR2 0x20
#define LIS331_I2_CFG1 0x10
#define LIS331_I2_CFG0 0x08
#define LIS331_LIR1 0x04
#define LIS331_I1_CFG1 0x02
#define LIS331_I1_CFG0 0x01


// CTRL_REG_4
#define LIS331_FS_2G 0x00
#define LIS331_FS_4G 0x10
#define LIS331_FS_8G 0x30
#define LIS331_BDU 0x80
#define LIS331_BLE 0x40

// H3LIS331 CTRL_REG_4
#define H3LIS331_FS_100G 0x00
#define H3LIS331_FS_200G 0x10
#define H3LIS331_FS_400G 0x30

// STATUS_REG
#define LIS331_ZYXOR 0x80
#define LIS331_ZOR 0x40
#define LIS331_YOR 0x20
#define LIS331_XOR 0x10
#define LIS331_ZYXDA 0x08
#define LIS331_ZDA 0x04
#define LIS331_YDA 0x02
#define LIS331_XDA 0x01

int8_t lis331_init(void);
uint8_t lis331_data_status(void);
uint8_t lis331_data_ready(void);
int8_t lis331_read(int16_t *x, int16_t *y, int16_t *z);

#endif
