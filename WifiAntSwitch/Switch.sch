EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title "WR9R LAN ANTENNA SELECTOR"
Date "2020-08-06"
Rev "B"
Comp "WR9R"
Comment1 "Moved IO35->IO27"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3700 3000 0    50   Input ~ 0
Input
Text HLabel 8100 3400 2    50   Input ~ 0
Output
Text HLabel 8050 5350 2    50   Input ~ 0
Return
Wire Wire Line
	8100 3400 7750 3400
$Comp
L Device:R_Small_US R3
U 1 1 5F2F77BC
P 4750 4650
AR Path="/5F2C6ED8/5F2F77BC" Ref="R3"  Part="1" 
AR Path="/5F31349A/5F2F77BC" Ref="R?"  Part="1" 
AR Path="/5F313B7C/5F2F77BC" Ref="R7"  Part="1" 
AR Path="/5F313D76/5F2F77BC" Ref="R11"  Part="1" 
AR Path="/5F313F40/5F2F77BC" Ref="R15"  Part="1" 
AR Path="/5F3141D6/5F2F77BC" Ref="R19"  Part="1" 
F 0 "R19" V 4545 4650 50  0000 C CNN
F 1 "510" V 4636 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 4750 4650 50  0001 C CNN
F 3 "~" H 4750 4650 50  0001 C CNN
	1    4750 4650
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5F30BB91
P 5100 3900
AR Path="/5F2C6ED8/5F30BB91" Ref="C3"  Part="1" 
AR Path="/5F31349A/5F30BB91" Ref="C?"  Part="1" 
AR Path="/5F313B7C/5F30BB91" Ref="C5"  Part="1" 
AR Path="/5F313D76/5F30BB91" Ref="C7"  Part="1" 
AR Path="/5F313F40/5F30BB91" Ref="C9"  Part="1" 
AR Path="/5F3141D6/5F30BB91" Ref="C11"  Part="1" 
F 0 "C11" H 5192 3946 50  0000 L CNN
F 1 "1uF, 25V" H 5192 3855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5100 3900 50  0001 C CNN
F 3 "~" H 5100 3900 50  0001 C CNN
	1    5100 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5F30CA00
P 7750 4300
AR Path="/5F2C6ED8/5F30CA00" Ref="C4"  Part="1" 
AR Path="/5F31349A/5F30CA00" Ref="C?"  Part="1" 
AR Path="/5F313B7C/5F30CA00" Ref="C6"  Part="1" 
AR Path="/5F313D76/5F30CA00" Ref="C8"  Part="1" 
AR Path="/5F313F40/5F30CA00" Ref="C10"  Part="1" 
AR Path="/5F3141D6/5F30CA00" Ref="C12"  Part="1" 
F 0 "C12" H 7842 4346 50  0000 L CNN
F 1 "0.01uf, 100V" H 7842 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7750 4300 50  0001 C CNN
F 3 "~" H 7750 4300 50  0001 C CNN
	1    7750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 4200 7750 3400
Connection ~ 7750 3400
Wire Wire Line
	7750 3400 7100 3400
Wire Wire Line
	7750 4400 7750 5350
Connection ~ 7750 5350
Wire Wire Line
	7750 5350 8050 5350
Wire Wire Line
	4650 4650 3700 4650
Text HLabel 3700 4650 0    50   Input ~ 0
Select
Text HLabel 8100 2200 2    50   Input ~ 0
VDCout
Wire Wire Line
	5100 4000 5100 5350
$Comp
L Device:R_Small_US R4
U 1 1 5F38825E
P 7100 3850
AR Path="/5F2C6ED8/5F38825E" Ref="R4"  Part="1" 
AR Path="/5F31349A/5F38825E" Ref="R?"  Part="1" 
AR Path="/5F313B7C/5F38825E" Ref="R8"  Part="1" 
AR Path="/5F313D76/5F38825E" Ref="R12"  Part="1" 
AR Path="/5F313F40/5F38825E" Ref="R16"  Part="1" 
AR Path="/5F3141D6/5F38825E" Ref="R20"  Part="1" 
F 0 "R20" H 7168 3896 50  0000 L CNN
F 1 "6.8K" H 7168 3805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7100 3850 50  0001 C CNN
F 3 "~" H 7100 3850 50  0001 C CNN
	1    7100 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D10
U 1 1 5F3890A8
P 7100 4700
AR Path="/5F3141D6/5F3890A8" Ref="D10"  Part="1" 
AR Path="/5F2C6ED8/5F3890A8" Ref="D2"  Part="1" 
AR Path="/5F313B7C/5F3890A8" Ref="D4"  Part="1" 
AR Path="/5F313D76/5F3890A8" Ref="D6"  Part="1" 
AR Path="/5F313F40/5F3890A8" Ref="D8"  Part="1" 
F 0 "D10" V 7139 4582 50  0000 R CNN
F 1 "599-0120-007F" H 7350 4850 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 7100 4700 50  0001 C CNN
F 3 "https://www.mouser.com/ProductDetail/Dialight/599-0120-007F?qs=gTYE2QTfZfSQtH19g4L1Mg%3D%3D" H 7100 4700 50  0001 C CNN
	1    7100 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 3750 7100 3400
Wire Wire Line
	7100 3950 7100 4550
Wire Wire Line
	7100 4850 7100 5350
Connection ~ 7100 5350
Wire Wire Line
	7100 5350 7750 5350
$Comp
L Device:R_Small_US R1
U 1 1 5F38D6D2
P 3950 3500
AR Path="/5F2C6ED8/5F38D6D2" Ref="R1"  Part="1" 
AR Path="/5F31349A/5F38D6D2" Ref="R?"  Part="1" 
AR Path="/5F313B7C/5F38D6D2" Ref="R5"  Part="1" 
AR Path="/5F313D76/5F38D6D2" Ref="R9"  Part="1" 
AR Path="/5F313F40/5F38D6D2" Ref="R13"  Part="1" 
AR Path="/5F3141D6/5F38D6D2" Ref="R17"  Part="1" 
F 0 "R17" H 3882 3454 50  0000 R CNN
F 1 "22K" H 3882 3545 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3950 3500 50  0001 C CNN
F 3 "~" H 3950 3500 50  0001 C CNN
	1    3950 3500
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R2
U 1 1 5F38DFA9
P 3950 4000
AR Path="/5F2C6ED8/5F38DFA9" Ref="R2"  Part="1" 
AR Path="/5F31349A/5F38DFA9" Ref="R?"  Part="1" 
AR Path="/5F313B7C/5F38DFA9" Ref="R6"  Part="1" 
AR Path="/5F313D76/5F38DFA9" Ref="R10"  Part="1" 
AR Path="/5F313F40/5F38DFA9" Ref="R14"  Part="1" 
AR Path="/5F3141D6/5F38DFA9" Ref="R18"  Part="1" 
F 0 "R18" H 4018 4046 50  0000 L CNN
F 1 "6.8K" H 4018 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3950 4000 50  0001 C CNN
F 3 "~" H 3950 4000 50  0001 C CNN
	1    3950 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4100 3950 5350
Wire Wire Line
	3950 5350 5100 5350
Wire Wire Line
	3950 3400 3950 3000
Wire Wire Line
	3950 3000 3700 3000
Wire Wire Line
	3950 3600 3950 3800
Text HLabel 3700 3800 0    50   Input ~ 0
Sense
Wire Wire Line
	3700 3800 3950 3800
Connection ~ 3950 3800
Wire Wire Line
	3950 3800 3950 3900
$Comp
L Relay_SolidState:ASSR-1218 U3
U 1 1 5F2CC684
P 6350 2900
AR Path="/5F2C6ED8/5F2CC684" Ref="U3"  Part="1" 
AR Path="/5F313B7C/5F2CC684" Ref="U4"  Part="1" 
AR Path="/5F313D76/5F2CC684" Ref="U5"  Part="1" 
AR Path="/5F313F40/5F2CC684" Ref="U6"  Part="1" 
AR Path="/5F3141D6/5F2CC684" Ref="U7"  Part="1" 
F 0 "U7" H 6350 3225 50  0000 C CNN
F 1 "ASSR-1218" H 6350 3134 50  0000 C CNN
F 2 "Package_SO:SO-4_4.4x4.3mm_P2.54mm" H 6150 2700 50  0001 L CIN
F 3 "https://docs.broadcom.com/docs/AV02-0173EN" H 6350 2900 50  0001 L CNN
	1    6350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2200 7100 2200
Wire Wire Line
	6650 2800 7100 2800
Wire Wire Line
	7100 2800 7100 2200
Connection ~ 7100 2200
Wire Wire Line
	7100 2200 8100 2200
Wire Wire Line
	6650 3000 7100 3000
Wire Wire Line
	7100 3000 7100 3400
Connection ~ 7100 3400
Wire Wire Line
	6050 3000 6050 5350
Wire Wire Line
	6050 5350 7100 5350
Wire Wire Line
	6050 5350 5100 5350
Connection ~ 6050 5350
Connection ~ 5100 5350
Wire Wire Line
	4850 4650 5700 4650
Wire Wire Line
	5700 4650 5700 2800
Wire Wire Line
	5700 2800 6050 2800
Wire Wire Line
	5100 2200 5100 3800
$EndSCHEMATC
