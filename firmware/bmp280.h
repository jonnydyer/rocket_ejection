/*
 * bmp280.h
 * Header file for bmp280 driver.
 * Datasheet available at https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#ifndef __BMP280_H__
#define __BMP280_H__

#include <stdint.h>

#define BMP280_CHIP_ID 0x58
#define BMP280_RESET 0xB6
#define BMP280_ADDR 0x76

// I2C Registers
#define BMP280_REG_TEMP_XLSB 0xFC
#define BMP280_REG_TEMP_LSB 0xFB
#define BMP280_REG_TEMP_MSB 0xFA
#define BMP280_REG_PRESS_XLSB 0xF9
#define BMP280_REG_PRESS_LSB 0xF8
#define BMP280_REG_PRESS_MSB 0xF7
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_ID 0xD0

// CTRL_MEAS
#define BMP280_CTRL_MEAS_OSR_T_MASK 0xE0
#define BMP280_CTRL_MEAS_OSR_P_MASK 0x1C
#define BMP280_CTRL_MEAS_MODE_MASK 0x03

//Config
#define BMP280_CONFIG_T_SB_MASK 0xE0
#define BMP280_CONFIG_FILTER_MASK 0x1C
#define BMP280_CONFIG_SPIW_EN_MASK 0x01


// Calibration registers
#define BMP280_REG_DIG_T1 0x88
#define BMP280_REG_DIG_T2 0x8A
#define BMP280_REG_DIG_T3 0x8C
#define BMP280_REG_DIG_P1 0x8E
#define BMP280_REG_DIG_P2 0x90
#define BMP280_REG_DIG_P3 0x92
#define BMP280_REG_DIG_P4 0x94
#define BMP280_REG_DIG_P5 0x96
#define BMP280_REG_DIG_P6 0x98
#define BMP280_REG_DIG_P7 0x9A
#define BMP280_REG_DIG_P8 0x9C
#define BMP280_REG_DIG_P9 0x9E

//Oversampling
#define BMP280_OS_NULL 0x00
#define BMP280_OS_1 0x01
#define BMP280_OS_2 0x02
#define BMP280_OS_4 0x03
#define BMP280_OS_8 0x04
#define BMP280_OS_16 0x07

//IIR
#define BMP280_IIR_OFF 0x00
#define BMP280_IIR_2 0x01
#define BMP280_IIR_4 0x02
#define BMP280_IIR_8 0x03
#define BMP280_IIR_16 0x04

#define BMP280_SLEEP_MODE	0x00
#define BMP280_FORCED_MODE	0x01
#define BMP280_NORMAL_MODE	0x03

/* ODR options */
#define BMP280_ODR_0_5_MS	0x00
#define BMP280_ODR_62_5_MS	0x01
#define BMP280_ODR_125_MS	0x02
#define BMP280_ODR_250_MS	0x03
#define BMP280_ODR_500_MS	0x04
#define BMP280_ODR_1000_MS	0x05
#define BMP280_ODR_2000_MS	0x06
#define BMP280_ODR_4000_MS	0x07

/*Filter coefficient macros */
#define BMP280_FILTER_OFF	0x00
#define BMP280_FILTER_COEFF_2	0x01
#define BMP280_FILTER_COEFF_4	0x02
#define BMP280_FILTER_COEFF_8	0x03
#define BMP280_FILTER_COEFF_16	0x04

/*SPI 3-Wire macros */
#define BMP280_SPI3_WIRE_ENABLE		1
#define BMP280_SPI3_WIRE_DISABLE	0

/*Measurement status */
#define BMP280_MEAS_DONE	0
#define BMP280_MEAS_ONGOING	1

/*Image update */
#define BMP280_IM_UPDATE_DONE		0
#define BMP280_IM_UPDATE_ONGOING	1

/*Position and mask macros */
#define BMP280_STATUS_IM_UPDATE_POS	0
#define BMP280_STATUS_IM_UPDATE_MASK	0x01
#define BMP280_STATUS_MEAS_POS		3
#define BMP280_STATUS_MEAS_MASK		0x08
#define BMP280_OS_TEMP_POS		5
#define BMP280_OS_TEMP_MASK		0xE0
#define BMP280_OS_PRES_POS		2
#define BMP280_OS_PRES_MASK		0x1C
#define BMP280_POWER_MODE_POS		0
#define BMP280_POWER_MODE_MASK		0x03
#define BMP280_STANDBY_DURN_POS		5
#define BMP280_STANDBY_DURN_MASK	0xE0
#define BMP280_FILTER_POS		2
#define BMP280_FILTER_MASK		0x1C
#define BMP280_SPI3_ENABLE_POS		0
#define BMP280_SPI3_ENABLE_MASK		0x01

// TODO: Move this into bmp280.c once debugging done
uint8_t bmp280_init(void);
int8_t bmp280_read_regs(uint8_t addr, uint8_t len);
int8_t bmp280_write_reg(uint8_t addr, uint8_t val);
int8_t bmp280_load_compensation(void);
int32_t bmp280_get_temp(void);
int32_t bmp280_get_pressure(void);
void bmp280_get_temp_press(int32_t *t, int32_t *p);
uint8_t bmp280_busy(void);

#endif
