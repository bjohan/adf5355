EESchema Schematic File Version 4
EELAYER 29 0
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
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5CBCDEBB
P 4550 2900
F 0 "A1" H 4550 1811 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 4550 1720 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 4700 1950 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 4550 1900 50  0001 C CNN
	1    4550 2900
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x10_Female J1
U 1 1 5CBCE7CD
P 7300 2900
F 0 "J1" H 7328 2876 50  0000 L CNN
F 1 "Conn_01x10_Female" H 7328 2785 50  0000 L CNN
F 2 "Connector_Molex:Molex_KK-254_AE-6410-10A_1x10_P2.54mm_Vertical" H 7300 2900 50  0001 C CNN
F 3 "~" H 7300 2900 50  0001 C CNN
	1    7300 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 2500 6650 2500
Wire Wire Line
	7100 2700 6650 2700
Wire Wire Line
	7100 2800 6650 2800
Wire Wire Line
	7100 3400 6650 3400
Text Label 6750 2500 0    50   ~ 0
VIN
Text Label 6750 2600 0    50   ~ 0
6V
Text Label 6750 2700 0    50   ~ 0
5V
Text Label 6750 2800 0    50   ~ 0
3V3
Text Label 6750 2900 0    50   ~ 0
CLK
Text Label 6750 3000 0    50   ~ 0
DATA
Text Label 6750 3100 0    50   ~ 0
MUX
Text Label 6750 3200 0    50   ~ 0
LE
Text Label 6750 3300 0    50   ~ 0
CE
$Comp
L power:GND #PWR0101
U 1 1 5CBCFFE7
P 6650 3400
F 0 "#PWR0101" H 6650 3150 50  0001 C CNN
F 1 "GND" H 6655 3227 50  0000 C CNN
F 2 "" H 6650 3400 50  0001 C CNN
F 3 "" H 6650 3400 50  0001 C CNN
	1    6650 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3100 6150 2500
Wire Wire Line
	6150 2500 5600 2500
Wire Wire Line
	6150 3100 7100 3100
Wire Wire Line
	6000 3200 6000 2600
Wire Wire Line
	6000 2600 5450 2600
Wire Wire Line
	6000 3200 7100 3200
Wire Wire Line
	5050 3400 5100 3400
Wire Wire Line
	6450 3400 6450 3000
Wire Wire Line
	6450 3000 7100 3000
Wire Wire Line
	5050 3600 5100 3600
Wire Wire Line
	6350 3600 6350 2900
Wire Wire Line
	6350 2900 7100 2900
Wire Wire Line
	5050 3500 5100 3500
Wire Wire Line
	6200 3500 6200 3300
Wire Wire Line
	6200 3300 7100 3300
$Comp
L Device:R R5
U 1 1 5CBD3A87
P 5250 3400
F 0 "R5" V 5043 3400 50  0000 C CNN
F 1 "R" V 5134 3400 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5180 3400 50  0001 C CNN
F 3 "~" H 5250 3400 50  0001 C CNN
	1    5250 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 3400 5450 3400
$Comp
L Device:R R6
U 1 1 5CBD3F1B
P 5250 3500
F 0 "R6" V 5043 3500 50  0000 C CNN
F 1 "R" V 5134 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5180 3500 50  0001 C CNN
F 3 "~" H 5250 3500 50  0001 C CNN
	1    5250 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 3500 5650 3500
$Comp
L Device:R R7
U 1 1 5CBD407E
P 5250 3600
F 0 "R7" V 5043 3600 50  0000 C CNN
F 1 "R" V 5134 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5180 3600 50  0001 C CNN
F 3 "~" H 5250 3600 50  0001 C CNN
	1    5250 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 3600 5850 3600
$Comp
L Device:R R3
U 1 1 5CBD422F
P 5250 2500
F 0 "R3" V 5043 2500 50  0000 C CNN
F 1 "R" V 5134 2500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5180 2500 50  0001 C CNN
F 3 "~" H 5250 2500 50  0001 C CNN
	1    5250 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 2500 5050 2500
$Comp
L Device:R R4
U 1 1 5CBD45D4
P 5250 2600
F 0 "R4" V 5043 2600 50  0000 C CNN
F 1 "R" V 5134 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5180 2600 50  0001 C CNN
F 3 "~" H 5250 2600 50  0001 C CNN
	1    5250 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 2600 5050 2600
$Comp
L Device:R R8
U 1 1 5CBD473C
P 5450 2750
F 0 "R8" H 5380 2704 50  0000 R CNN
F 1 "R" H 5380 2795 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5380 2750 50  0001 C CNN
F 3 "~" H 5450 2750 50  0001 C CNN
	1    5450 2750
	-1   0    0    1   
$EndComp
Connection ~ 5450 2600
Wire Wire Line
	5450 2600 5400 2600
$Comp
L Device:R R10
U 1 1 5CBD4AFF
P 5600 2650
F 0 "R10" H 5530 2604 50  0000 R CNN
F 1 "R" H 5530 2695 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5530 2650 50  0001 C CNN
F 3 "~" H 5600 2650 50  0001 C CNN
	1    5600 2650
	-1   0    0    1   
$EndComp
Connection ~ 5600 2500
Wire Wire Line
	5600 2500 5400 2500
$Comp
L Device:R R9
U 1 1 5CBD4D98
P 5450 3550
F 0 "R9" H 5380 3504 50  0000 R CNN
F 1 "R" H 5380 3595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5380 3550 50  0001 C CNN
F 3 "~" H 5450 3550 50  0001 C CNN
	1    5450 3550
	-1   0    0    1   
$EndComp
Connection ~ 5450 3400
Wire Wire Line
	5450 3400 6450 3400
$Comp
L Device:R R11
U 1 1 5CBD5448
P 5650 3650
F 0 "R11" H 5580 3604 50  0000 R CNN
F 1 "R" H 5580 3695 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5580 3650 50  0001 C CNN
F 3 "~" H 5650 3650 50  0001 C CNN
	1    5650 3650
	-1   0    0    1   
$EndComp
Connection ~ 5650 3500
Wire Wire Line
	5650 3500 6200 3500
$Comp
L Device:R R12
U 1 1 5CBD5871
P 5850 3750
F 0 "R12" H 5780 3704 50  0000 R CNN
F 1 "R" H 5780 3795 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5780 3750 50  0001 C CNN
F 3 "~" H 5850 3750 50  0001 C CNN
	1    5850 3750
	-1   0    0    1   
$EndComp
Connection ~ 5850 3600
Wire Wire Line
	5850 3600 6350 3600
$Comp
L power:GND #PWR0102
U 1 1 5CBD5CA0
P 5450 2950
F 0 "#PWR0102" H 5450 2700 50  0001 C CNN
F 1 "GND" H 5455 2777 50  0000 C CNN
F 2 "" H 5450 2950 50  0001 C CNN
F 3 "" H 5450 2950 50  0001 C CNN
	1    5450 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5CBD5F8A
P 5600 2850
F 0 "#PWR0103" H 5600 2600 50  0001 C CNN
F 1 "GND" H 5605 2677 50  0000 C CNN
F 2 "" H 5600 2850 50  0001 C CNN
F 3 "" H 5600 2850 50  0001 C CNN
	1    5600 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5CBD620D
P 5450 3750
F 0 "#PWR0104" H 5450 3500 50  0001 C CNN
F 1 "GND" H 5455 3577 50  0000 C CNN
F 2 "" H 5450 3750 50  0001 C CNN
F 3 "" H 5450 3750 50  0001 C CNN
	1    5450 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5CBD66BD
P 5650 3850
F 0 "#PWR0105" H 5650 3600 50  0001 C CNN
F 1 "GND" H 5655 3677 50  0000 C CNN
F 2 "" H 5650 3850 50  0001 C CNN
F 3 "" H 5650 3850 50  0001 C CNN
	1    5650 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5CBD67E2
P 5850 3950
F 0 "#PWR0106" H 5850 3700 50  0001 C CNN
F 1 "GND" H 5855 3777 50  0000 C CNN
F 2 "" H 5850 3950 50  0001 C CNN
F 3 "" H 5850 3950 50  0001 C CNN
	1    5850 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2900 5450 2950
Wire Wire Line
	5600 2800 5600 2850
Wire Wire Line
	5450 3700 5450 3750
Wire Wire Line
	5650 3800 5650 3850
Wire Wire Line
	5850 3900 5850 3950
$Comp
L Device:R R1
U 1 1 5CBD9563
P 4800 1700
F 0 "R1" V 4593 1700 50  0000 C CNN
F 1 "R" V 4684 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4730 1700 50  0001 C CNN
F 3 "~" H 4800 1700 50  0001 C CNN
	1    4800 1700
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5CBD956D
P 4800 1800
F 0 "R2" V 4593 1800 50  0000 C CNN
F 1 "R" V 4684 1800 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4730 1800 50  0001 C CNN
F 3 "~" H 4800 1800 50  0001 C CNN
	1    4800 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 1900 4650 1800
Wire Wire Line
	4650 1800 4650 1700
Connection ~ 4650 1800
Wire Wire Line
	6650 2500 6650 1700
Wire Wire Line
	6650 1700 4950 1700
Wire Wire Line
	6550 2600 6550 1800
Wire Wire Line
	6550 1800 4950 1800
Wire Wire Line
	6550 2600 7100 2600
$EndSCHEMATC
