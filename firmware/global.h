
//*****************************************************************************
//
// File Name	: 'global.h'
// Title		: AVR project global include 
// Author		: Pascal Stang
// Created		: 7/12/2001
// Revised		: 9/30/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//	Description : This include file is designed to contain items useful to all
//					code files and projects.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GLOBAL_H
#define GLOBAL_H

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"

// project/system dependent defines

#define LED_RED_PIN PD4
#define LED_GREEN_PIN PD5
#define LED_BLUE_PIN PD6
#define SW_1_PIN PB0
#define SW_2_PIN PD7

#define EXTINT_PORT PORTC
#define EXTINT_PIN PINC
#define EXTINT_DDR DDRC
#define EXTINT_LIS331_PIN (1<<PC2)
#define EXTINT_LSM303_PIN (1<<PC3)

#define PYRO_OUT_DDR DDRC
#define PYRO_OUT_PORT PORTC
#define PYRO_OUT_A_PIN (1<<PC0)
#define PYRO_OUT_B_PIN (1<<PC1)

//I2C Defs
#define I2C_PORT PORTC
#define I2C_DDR DDRC
#define I2C_SCL (1<<PC5)
#define I2C_SDA (1<<PC4)

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#define SYSTICK_RATE 100
#define STREAM_BUFF_LEN 20
#define MA_FILT_LEN 16
#endif
