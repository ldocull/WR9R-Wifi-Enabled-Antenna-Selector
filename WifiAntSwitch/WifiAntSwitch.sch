EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title "WR9R LAN ANTENNA SELECTOR"
Date "2020-08-06"
Rev "B"
Comp "WR9R"
Comment1 "Moved IO35->IO27"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x06_Female J1
U 1 1 5F2C1D7A
P 2100 1950
F 0 "J1" H 1992 2335 50  0000 C CNN
F 1 "INPUTS" H 1992 2244 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 2100 1950 50  0001 C CNN
F 3 "~" H 2100 1950 50  0001 C CNN
	1    2100 1950
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Female J2
U 1 1 5F2C3604
P 9500 1850
F 0 "J2" H 9528 1826 50  0000 L CNN
F 1 "OUTPUTS" H 9528 1735 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 9500 1850 50  0001 C CNN
F 3 "~" H 9500 1850 50  0001 C CNN
	1    9500 1850
	1    0    0    -1  
$EndComp
$Sheet
S 4250 1150 900  350 
U 5F2C6ED8
F0 "Switch" 50
F1 "Switch.sch" 50
F2 "Input" I L 4250 1200 50 
F3 "Output" I R 5150 1200 50 
F4 "Return" I R 5150 1400 50 
F5 "Select" I L 4250 1400 50 
F6 "VDCout" I R 5150 1300 50 
F7 "Sense" I L 4250 1300 50 
$EndSheet
$Sheet
S 4250 1750 900  350 
U 5F313B7C
F0 "sheet5F313B75" 50
F1 "Switch.sch" 50
F2 "Input" I L 4250 1800 50 
F3 "Output" I R 5150 1800 50 
F4 "Return" I R 5150 2000 50 
F5 "Select" I L 4250 2000 50 
F6 "VDCout" I R 5150 1900 50 
F7 "Sense" I L 4250 1900 50 
$EndSheet
$Sheet
S 4250 2350 900  350 
U 5F313D76
F0 "sheet5F313D6F" 50
F1 "Switch.sch" 50
F2 "Input" I L 4250 2400 50 
F3 "Output" I R 5150 2400 50 
F4 "Return" I R 5150 2600 50 
F5 "Select" I L 4250 2600 50 
F6 "VDCout" I R 5150 2500 50 
F7 "Sense" I L 4250 2500 50 
$EndSheet
$Sheet
S 4250 2950 900  350 
U 5F313F40
F0 "sheet5F313F39" 50
F1 "Switch.sch" 50
F2 "Input" I L 4250 3000 50 
F3 "Output" I R 5150 3000 50 
F4 "Return" I R 5150 3200 50 
F5 "Select" I L 4250 3200 50 
F6 "VDCout" I R 5150 3100 50 
F7 "Sense" I L 4250 3100 50 
$EndSheet
$Sheet
S 4250 3550 900  350 
U 5F3141D6
F0 "sheet5F3141CF" 50
F1 "Switch.sch" 50
F2 "Input" I L 4250 3600 50 
F3 "Output" I R 5150 3600 50 
F4 "Return" I R 5150 3800 50 
F5 "Select" I L 4250 3800 50 
F6 "VDCout" I R 5150 3700 50 
F7 "Sense" I L 4250 3700 50 
$EndSheet
Wire Wire Line
	2300 1750 3050 1750
Wire Wire Line
	3050 1750 3050 1200
Wire Wire Line
	3050 1200 4250 1200
Wire Wire Line
	2300 1850 3200 1850
Wire Wire Line
	3200 1850 3200 1800
Wire Wire Line
	3200 1800 4250 1800
Wire Wire Line
	2300 1950 3200 1950
Wire Wire Line
	3200 1950 3200 2400
Wire Wire Line
	3200 2400 4250 2400
Wire Wire Line
	2300 2050 3050 2050
Wire Wire Line
	3050 2050 3050 3000
Wire Wire Line
	3050 3000 4250 3000
Wire Wire Line
	4250 3600 2950 3600
Wire Wire Line
	2950 3600 2950 2150
Wire Wire Line
	2950 2150 2300 2150
Wire Wire Line
	2300 2250 2800 2250
Wire Wire Line
	2800 2250 2800 4150
Wire Wire Line
	2800 4150 2900 4150
Wire Wire Line
	5400 4150 5400 3800
Wire Wire Line
	5400 2000 5150 2000
Wire Wire Line
	5150 2600 5400 2600
Connection ~ 5400 2600
Wire Wire Line
	5400 2600 5400 2050
Wire Wire Line
	5150 3200 5400 3200
Connection ~ 5400 3200
Wire Wire Line
	5400 3200 5400 2600
Wire Wire Line
	5150 3800 5400 3800
Connection ~ 5400 3800
Wire Wire Line
	5400 3800 5400 3200
Wire Wire Line
	5150 1400 5400 1400
Wire Wire Line
	5400 1400 5400 2000
Connection ~ 5400 2000
Wire Wire Line
	5150 1200 6950 1200
Wire Wire Line
	6950 1200 6950 1550
Wire Wire Line
	6950 1550 9300 1550
Wire Wire Line
	9300 1650 6950 1650
Wire Wire Line
	6950 1650 6950 1800
Wire Wire Line
	6950 1800 5150 1800
Wire Wire Line
	5150 2400 7050 2400
Wire Wire Line
	7050 2400 7050 1750
Wire Wire Line
	7050 1750 9300 1750
Wire Wire Line
	5150 3000 7150 3000
Wire Wire Line
	7150 3000 7150 1850
Wire Wire Line
	7150 1850 9300 1850
Wire Wire Line
	5150 3600 7250 3600
Wire Wire Line
	7250 3600 7250 1950
Wire Wire Line
	7250 1950 9300 1950
Wire Wire Line
	9300 2050 5400 2050
Connection ~ 5400 2050
Wire Wire Line
	5400 2050 5400 2000
$Comp
L Regulator_Linear:LM7805_TO220 U1
U 1 1 5F32F8A8
P 4050 4950
F 0 "U1" H 4050 5192 50  0000 C CNN
F 1 "LM7805_TO220" H 4050 5101 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 4050 5175 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 4050 4900 50  0001 C CNN
	1    4050 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5F33141A
P 4450 5300
F 0 "C2" H 4542 5346 50  0000 L CNN
F 1 "1uF, 25V" H 4542 5255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4450 5300 50  0001 C CNN
F 3 "~" H 4450 5300 50  0001 C CNN
	1    4450 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5F331BCF
P 3550 5300
F 0 "C1" H 3642 5346 50  0000 L CNN
F 1 "1uF, 25V" H 3642 5255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3550 5300 50  0001 C CNN
F 3 "~" H 3550 5300 50  0001 C CNN
	1    3550 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5F33439A
P 3800 5700
F 0 "#PWR01" H 3800 5450 50  0001 C CNN
F 1 "GND" H 3805 5527 50  0000 C CNN
F 2 "" H 3800 5700 50  0001 C CNN
F 3 "" H 3800 5700 50  0001 C CNN
	1    3800 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 5400 4450 5600
Wire Wire Line
	4450 5600 4050 5600
Wire Wire Line
	3550 5600 3550 5400
Wire Wire Line
	3550 5600 2900 5600
Wire Wire Line
	2900 5600 2900 4800
Connection ~ 3550 5600
Connection ~ 2900 4150
Wire Wire Line
	2900 4150 5400 4150
Wire Wire Line
	4050 5250 4050 5600
Connection ~ 4050 5600
Wire Wire Line
	4050 5600 3800 5600
Wire Wire Line
	3800 5700 3800 5600
Connection ~ 3800 5600
Wire Wire Line
	3800 5600 3550 5600
Wire Wire Line
	4450 5200 4450 4950
Wire Wire Line
	4450 4950 4350 4950
Wire Wire Line
	3550 5200 3550 4950
Wire Wire Line
	3550 4950 3750 4950
Wire Wire Line
	3550 4950 3550 4700
Wire Wire Line
	3550 4500 5750 4500
Wire Wire Line
	5750 4500 5750 3700
Wire Wire Line
	5750 1300 5150 1300
Connection ~ 3550 4950
Wire Wire Line
	5150 1900 5750 1900
Connection ~ 5750 1900
Wire Wire Line
	5750 1900 5750 1300
Wire Wire Line
	5150 2500 5750 2500
Connection ~ 5750 2500
Wire Wire Line
	5750 2500 5750 1900
Wire Wire Line
	5150 3100 5750 3100
Connection ~ 5750 3100
Wire Wire Line
	5750 3100 5750 2500
Wire Wire Line
	5150 3700 5750 3700
Connection ~ 5750 3700
Wire Wire Line
	5750 3700 5750 3100
$Comp
L ESP32-DEVKITC-32D:ESP32-DEVKITC-32D U2
U 1 1 5F34558F
P 7150 5150
F 0 "U2" H 7150 6317 50  0000 C CNN
F 1 "ESP32-DEVKITC-32D" H 7150 6226 50  0000 C CNN
F 2 "MODULE_ESP32-DEVKITC-32D" H 7150 5150 50  0001 L BNN
F 3 "4" H 7150 5150 50  0001 L BNN
F 4 "Espressif Systems" H 7150 5150 50  0001 L BNN "Field4"
	1    7150 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 6050 4950 6050
Wire Wire Line
	4950 6050 4950 4950
Wire Wire Line
	4950 4950 4450 4950
Connection ~ 4450 4950
$Comp
L power:GND #PWR03
U 1 1 5F34F250
P 8200 6200
F 0 "#PWR03" H 8200 5950 50  0001 C CNN
F 1 "GND" H 8205 6027 50  0000 C CNN
F 2 "" H 8200 6200 50  0001 C CNN
F 3 "" H 8200 6200 50  0001 C CNN
	1    8200 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5F34F6CC
P 6150 6250
F 0 "#PWR02" H 6150 6000 50  0001 C CNN
F 1 "GND" H 6155 6077 50  0000 C CNN
F 2 "" H 6150 6250 50  0001 C CNN
F 3 "" H 6150 6250 50  0001 C CNN
	1    6150 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 5550 6150 5550
Wire Wire Line
	6150 5550 6150 6250
Wire Wire Line
	7950 4250 8200 4250
Wire Wire Line
	8200 4250 8200 4850
Wire Wire Line
	7950 4850 8200 4850
Connection ~ 8200 4850
Wire Wire Line
	8200 4850 8200 6200
Wire Wire Line
	6350 5250 5650 5250
Wire Wire Line
	6350 4850 5650 4850
Wire Wire Line
	6350 4950 5650 4950
Wire Wire Line
	6350 5050 5650 5050
Wire Wire Line
	6350 5150 5650 5150
Text Label 5750 5250 0    50   ~ 0
Sel0
Text Label 5750 4850 0    50   ~ 0
Sel1
Text Label 5750 4950 0    50   ~ 0
Sel2
Text Label 5750 5050 0    50   ~ 0
Sel3
Text Label 5750 5150 0    50   ~ 0
Sel4
Wire Wire Line
	4250 1400 3700 1400
Wire Wire Line
	4250 2000 3700 2000
Wire Wire Line
	4250 2600 3700 2600
Wire Wire Line
	4250 3200 3700 3200
Wire Wire Line
	4250 3800 3700 3800
Text Label 3800 1400 0    50   ~ 0
Sel0
Text Label 3800 2000 0    50   ~ 0
Sel1
Text Label 3800 2600 0    50   ~ 0
Sel2
Text Label 3800 3200 0    50   ~ 0
Sel3
Text Label 3800 3800 0    50   ~ 0
Sel4
Wire Wire Line
	4250 1300 3700 1300
Wire Wire Line
	4250 1900 3700 1900
Wire Wire Line
	4250 2500 3700 2500
Wire Wire Line
	4250 3100 3700 3100
Wire Wire Line
	4250 3700 3700 3700
Text Label 3800 1300 0    50   ~ 0
Inp0
Text Label 3800 1900 0    50   ~ 0
Inp1
Text Label 3800 2500 0    50   ~ 0
Inp2
Text Label 3800 3100 0    50   ~ 0
Inp3
Text Label 3800 3700 0    50   ~ 0
Inp4
Wire Wire Line
	7950 5050 8900 5050
Wire Wire Line
	7950 4950 8900 4950
Wire Wire Line
	7950 4750 8900 4750
Wire Wire Line
	7950 4450 8900 4450
Wire Wire Line
	7950 4350 8900 4350
Text Label 8600 4350 0    50   ~ 0
Inp0
Text Label 8600 4450 0    50   ~ 0
Inp1
Text Label 8600 4750 0    50   ~ 0
Inp2
Text Label 8600 4950 0    50   ~ 0
Inp3
Text Label 8600 5050 0    50   ~ 0
Inp4
Wire Wire Line
	7950 4550 8300 4550
Wire Wire Line
	8300 4550 8300 2150
Wire Wire Line
	8300 2150 9300 2150
Wire Wire Line
	7950 4650 8400 4650
Wire Wire Line
	8400 4650 8400 2250
Wire Wire Line
	8400 2250 9300 2250
$Comp
L Connector:Conn_01x02_Female J3
U 1 1 5F2D8121
P 2200 4700
F 0 "J3" H 2092 4885 50  0000 C CNN
F 1 "POWER" H 2092 4794 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 4700 50  0001 C CNN
F 3 "~" H 2200 4700 50  0001 C CNN
	1    2200 4700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2400 4700 3550 4700
Connection ~ 3550 4700
Wire Wire Line
	3550 4700 3550 4500
Wire Wire Line
	2400 4800 2900 4800
Connection ~ 2900 4800
Wire Wire Line
	2900 4800 2900 4150
NoConn ~ 6350 4750
NoConn ~ 6350 4650
NoConn ~ 6350 4550
NoConn ~ 6350 4450
NoConn ~ 6350 4350
NoConn ~ 6350 4250
NoConn ~ 6350 5350
NoConn ~ 6350 5450
NoConn ~ 6350 5650
NoConn ~ 6350 5750
NoConn ~ 6350 5850
NoConn ~ 6350 5950
NoConn ~ 7950 6050
NoConn ~ 7950 5950
NoConn ~ 7950 5850
NoConn ~ 7950 5750
NoConn ~ 7950 5650
NoConn ~ 7950 5550
NoConn ~ 7950 5450
NoConn ~ 7950 5350
NoConn ~ 7950 5250
NoConn ~ 7950 5150
$EndSCHEMATC
