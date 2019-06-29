EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:bmp280_fp_riser-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BMP280 U1
U 1 1 59814F09
P 3900 2900
F 0 "U1" H 3750 3350 60  0000 C CNN
F 1 "BMP280" H 3750 2700 60  0000 C CNN
F 2 "" H 3900 2900 60  0000 C CNN
F 3 "" H 3900 2900 60  0000 C CNN
F 4 "BMP280" H 3850 3450 60  0001 C CNN "manf#"
	1    3900 2900
	-1   0    0    -1  
$EndComp
$Comp
L BMP280 U2
U 1 1 59814F5A
P 5300 2900
F 0 "U2" H 5150 3350 60  0000 C CNN
F 1 "BMP280" H 5150 2700 60  0000 C CNN
F 2 "Housings_LGA:LGA8-2-2.5(BMP280)" H 6400 3200 60  0000 C CNN
F 3 "" H 5300 2900 60  0000 C CNN
F 4 "BMP280" H 5250 3450 60  0001 C CNN "manf#"
	1    5300 2900
	1    0    0    -1  
$EndComp
Text Notes 3200 2100 0    60   ~ 0
Original (faulty) footprint\n
Text Notes 4900 2100 0    60   ~ 0
New, correct footprint
Wire Wire Line
	5350 3250 4550 3250
Wire Wire Line
	4550 3250 4550 2300
Wire Wire Line
	4550 2300 3850 2300
Wire Wire Line
	3850 3250 4500 3250
Wire Wire Line
	4500 3250 4500 2250
Wire Wire Line
	4500 2250 5350 2250
Wire Wire Line
	5350 2250 5350 2300
Wire Wire Line
	4300 2600 4600 2600
Wire Wire Line
	4600 2600 4600 3300
Wire Wire Line
	4600 3300 5450 3300
Wire Wire Line
	5450 3300 5450 3250
Wire Wire Line
	4900 2600 4650 2600
Wire Wire Line
	4650 2600 4650 3350
Wire Wire Line
	4650 3350 3750 3350
Wire Wire Line
	3750 3350 3750 3250
Wire Wire Line
	4300 2700 4350 2700
Wire Wire Line
	4350 2700 4350 2200
Wire Wire Line
	4350 2200 5450 2200
Wire Wire Line
	5450 2200 5450 2300
Wire Wire Line
	3750 2300 3750 2150
Wire Wire Line
	3750 2150 4850 2150
Wire Wire Line
	4850 2150 4850 2700
Wire Wire Line
	4850 2700 4900 2700
Wire Wire Line
	4900 2800 4700 2800
Wire Wire Line
	4700 2800 4700 2900
Wire Wire Line
	4700 2900 4300 2900
Wire Wire Line
	4300 2800 4450 2800
Wire Wire Line
	4450 2800 4450 2850
Wire Wire Line
	4450 2850 4900 2850
Wire Wire Line
	4900 2850 4900 2900
$EndSCHEMATC
