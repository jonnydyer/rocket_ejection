
/*
 * lsm303d.h
 * Header file for lsm303d driver.
 * Datasheet available at http://www.st.com/resource/en/datasheet/lsm303d.pdf 
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#ifndef __LSM303D_H__
#define __LSM303D_H__

//I2C Address
#define LSM303D_ADDR 0x3C

//Registers
#define LSM303_REG_TEMP_OUT_L 0x05
#define LSM303_REG_TEMP_OUT_H 0x06
#define LSM303_REG_STATUS_M 0x07
#define LSM303_REG_OUT_X_L_M 0x08
#define LSM303_REG_OUT_X_H_M 0x09
#define LSM303_REG_OUT_Y_L_M 0x0A
#define LSM303_REG_OUT_Y_H_M 0x0B
#define LSM303_REG_OUT_Z_L_M 0x0C
#define LSM303_REG_OUT_Z_H_M 0x0D
#define LSM303_REG_WHO_AM_I 0x0F
#define LSM303_REG_INT_CTRL_M 0x12
#define LSM303_REG_INT_SRC_M 0x13
#define LSM303_REG_INT_THS_L_M 0x14
#define LSM303_REG_INT_THS_H_M 0x15
#define LSM303_REG_OFFSET_X_L_M 0x16
#define LSM303_REG_OFFSET_X_H_M 0x17
#define LSM303_REG_OFFSET_Y_L_M 0x18
#define LSM303_REG_OFFSET_Y_H_M 0x19
#define LSM303_REG_OFFSET_Z_L_M 0x1A
#define LSM303_REG_OFFSET_Z_H_M 0x1B
#define LSM303_REG_REFERENCE_X 0x1C
#define LSM303_REG_REFERENCE_Y 0x1D
#define LSM303_REG_REFERENCE_Z 0x1E
#define LSM303_REG_CTRL0 0x1F
#define LSM303_REG_CTRL1 0x20
#define LSM303_REG_CTRL2 0x21
#define LSM303_REG_CTRL3 0x22
#define LSM303_REG_CTRL4 0x23
#define LSM303_REG_CTRL5 0x24
#define LSM303_REG_CTRL6 0x25
#define LSM303_REG_CTRL7 0x26
#define LSM303_REG_STATUS_A 0x27
#define LSM303_REG_OUT_X_L_A 0x28
#define LSM303_REG_OUT_X_H_A 0x29
#define LSM303_REG_OUT_Y_L_A 0x2a
#define LSM303_REG_OUT_Y_H_A 0x2b
#define LSM303_REG_OUT_Z_L_A 0x2c
#define LSM303_REG_OUT_Z_H_A 0x2d
#define LSM303_REG_FIFO_CTRL 0x2e
#define LSM303_REG_FIFO_SRC 0x2f
#define LSM303_REG_IG_CFG1 0x30
#define LSM303_REG_IG_SRC1 0x31
#define LSM303_REG_IG_THS1 0x32
#define LSM303_REG_IG_DUR1 0x33
#define LSM303_REG_IG_CFG2 0x34
#define LSM303_REG_IG_SRC2 0x35
#define LSM303_REG_IG_THS2 0x36
#define LSM303_REG_IG_DUR2 0x37
#define LSM303_REG_CLICK_CFG 0x38
#define LSM303_REG_CLICK_SRC 0x39
#define LSM303_REG_CLICK_THS 0x3a
#define LSM303_REG_TIME_LIMIT 0x3b
#define LSM303_REG_TIME_LATENCY 0x3c
#define LSM303_REG_TIME_WINDOW 0x3d
#define LSM303_REG_ACT_THS 0x3e
#define LSM303_REG_ACT_DUR 0x3f

// STATUS_M / STATUS_A
#define LSM303_XDA 0x01
#define LSM303_YDA 0x02
#define LSM303_ZDA 0x04
#define LSM303_ZYXDA 0x08
#define LSM303_XOR 0x10
#define LSM303_YOR 0x20
#define LSM303_ZOR 0x40
#define LSM303_ZYXOR 0x80

// CTRL0
#define LSM303_HPIS2 0x01
#define LSM303_HPIS1 0x02
#define LSM303_HP_CLICK 0x04
#define LSM303_FTH_EN 0x20
#define LSM303_FIFO_EN 0x40
#define LSM303_BOOT 0x80

// CTRL1
#define LSM303_AXEN 0x01
#define LSM303_AYEN 0x02
#define LSM303_AZEN 0x04
#define LSM303_BDU 0x08
#define LSM303_A_POWER_DOWN 0x00
#define LSM303_A_DR_3_125HZ (0x01<<4)
#define LSM303_A_DR_6_25HZ (0x02<<4)
#define LSM303_A_DR_12_5HZ (0x03<<4)
#define LSM303_A_DR_25HZ (0x04<<4)
#define LSM303_A_DR_50HZ (0x05<<4)
#define LSM303_A_DR_100HZ (0x06<<4)
#define LSM303_A_DR_200HZ (0x07<<4)
#define LSM303_A_DR_400HZ (0x08<<4)
#define LSM303_A_DR_800HZ (0x09<<4)
#define LSM303_A_DR_1600HZ (0x0a<<4)

// CTRL2
#define LSM303_AST 0x02
#define LSM303_A_FS_2G (0x00<<3)
#define LSM303_A_FS_4G (0x01<<3)
#define LSM303_A_FS_6G (0x02<<3)
#define LSM303_A_FS_8G (0x03<<3)
#define LSM303_A_FS_16G (0x04<<3)
#define LSM303_A_BW_773HZ (0x00<<6)
#define LSM303_A_BW_194HZ (0x01<<6)
#define LSM303_A_BW_362HZ (0x02<<6)
#define LSM303_A_BW_50HZ (0x03<<6)

// CTRL3
#define LSM303_INT1_EMPTY 0x01
#define LSM303_INT1_DRDY_M 0x02
#define LSM303_INT1_DRDY_A 0x04

// CTRL4
#define LSM303_INT2_FTH 0x01
#define LSM303_INT2_OVERRUN 0x02
#define LSM303_INT2_DRDY_M 0x04
#define LSM303_INT2_DRDY_A 0x08

// CTRL5
#define LSM303_LIR1 0x01
#define LSM303_LIR2 0x02
#define LSM303_TEMP_EN 0x80
#define LSM303_M_DR_3_125HZ (0x00<<2)
#define LSM303_M_DR_6_25HZ (0x01<<2)
#define LSM303_M_DR_12_5HZ (0x02<<2)
#define LSM303_M_DR_25HZ (0x03<<2)
#define LSM303_M_DR_50HZ (0x04<<2)
#define LSM303_M_DR_100HZ (0x05<<2)
#define LSM303_M_LOW_RES 0x00
#define LSM303_M_HI_RES 0x60

// CTRL6
#define LSM303_M_FS_2GAUSS (0x00<<5)
#define LSM303_M_FS_4GAUSS (0x01<<5)
#define LSM303_M_FS_8GAUSS (0x02<<5)
#define LSM303_M_FS_12GAUSS (0x03<<5)

// CTRL7
#define LSM303_M_POWER_DOWN 0x03
#define LSM303_M_SINGLE_CONV 0x01
#define LSM303_M_CONT_CONV 0x00
#define LSM303_M_LOW_POWER 0x04
#define LSM303_T_ONLY 0x10
#define LSM303_A_FILT_DATA_SELECT 0x20
#define LSM303_A_HP_NORMAL_MODE (0x00<<6)
#define LSM303_A_HP_REF_SIGNAL (0x01<<6)
#define LSM303_A_HP_AUTO_RESET (0x03<<6)

int8_t lsm303_init(void);
uint8_t lsm303_accel_data_status(void);
uint8_t lsm303_mag_data_status(void);
uint8_t lsm303_data_ready_int(void);
int8_t lsm303_read_accel(int16_t *x, int16_t *y, int16_t *z);
int8_t lsm303_read_mag(int16_t *x, int16_t *y, int16_t *z);

#endif
