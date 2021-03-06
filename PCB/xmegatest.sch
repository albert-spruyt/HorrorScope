EESchema Schematic File Version 4
LIBS:xmegatest-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "minimal xmega"
Date ""
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:USB_B_Micro J2
U 1 1 5B80368B
P 4450 1750
F 0 "J2" H 4505 2217 50  0000 C CNN
F 1 "USB_B_Micro" H 4505 2126 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Amphenol_10103594-0001LF_Horizontal" H 4600 1700 50  0001 C CNN
F 3 "~" H 4600 1700 50  0001 C CNN
	1    4450 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5B803881
P 4500 2300
F 0 "#PWR0101" H 4500 2050 50  0001 C CNN
F 1 "GND" H 4505 2127 50  0000 C CNN
F 2 "" H 4500 2300 50  0001 C CNN
F 3 "" H 4500 2300 50  0001 C CNN
	1    4500 2300
	1    0    0    -1  
$EndComp
Text Label 5500 5200 0    50   ~ 0
USBD-
Text Label 5500 5300 0    50   ~ 0
USBD+
Wire Wire Line
	4750 1750 5050 1750
Wire Wire Line
	4750 1850 5050 1850
Text Label 5050 1750 0    50   ~ 0
USBD+
Text Label 5050 1850 0    50   ~ 0
USBD-
$Comp
L Regulator_Linear:LM1117-3.3 U1
U 1 1 5B803D53
P 2700 1600
F 0 "U1" H 2700 1842 50  0000 C CNN
F 1 "LM1117-3.3" H 2700 1751 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2700 1600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 2700 1600 50  0001 C CNN
	1    2700 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 5B803F83
P 1500 1600
F 0 "#PWR0103" H 1500 1450 50  0001 C CNN
F 1 "+5V" H 1515 1773 50  0000 C CNN
F 2 "" H 1500 1600 50  0001 C CNN
F 3 "" H 1500 1600 50  0001 C CNN
	1    1500 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5B804068
P 3800 1600
F 0 "#PWR0104" H 3800 1450 50  0001 C CNN
F 1 "+3.3V" H 3815 1773 50  0000 C CNN
F 2 "" H 3800 1600 50  0001 C CNN
F 3 "" H 3800 1600 50  0001 C CNN
	1    3800 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 5B80456F
P 7400 2250
F 0 "C7" V 7171 2250 50  0000 C CNN
F 1 "100nF" V 7262 2250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7400 2250 50  0001 C CNN
F 3 "~" H 7400 2250 50  0001 C CNN
	1    7400 2250
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C4
U 1 1 5B80620E
P 3100 1750
F 0 "C4" H 3215 1796 50  0000 L CNN
F 1 "100uF" H 3215 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 3100 1750 50  0001 C CNN
F 3 "~" H 3100 1750 50  0001 C CNN
	1    3100 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C2
U 1 1 5B806C36
P 2100 1750
F 0 "C2" H 2215 1796 50  0000 L CNN
F 1 "100uF" H 2215 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 2100 1750 50  0001 C CNN
F 3 "~" H 2100 1750 50  0001 C CNN
	1    2100 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5B806C3D
P 2700 2000
F 0 "#PWR0105" H 2700 1750 50  0001 C CNN
F 1 "GND" H 2705 1827 50  0000 C CNN
F 2 "" H 2700 2000 50  0001 C CNN
F 3 "" H 2700 2000 50  0001 C CNN
	1    2700 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1600 3100 1600
Connection ~ 3100 1600
Connection ~ 2100 1600
Wire Wire Line
	2100 1600 2400 1600
$Comp
L Device:Crystal Y1
U 1 1 5B80D8A0
P 2850 2800
F 0 "Y1" H 2850 3068 50  0000 C CNN
F 1 "32.768hz" H 2850 2977 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_MicroCrystal_CC7V-T1A-2Pin_3.2x1.5mm_HandSoldering" H 2850 2800 50  0001 C CNN
F 3 "~" H 2850 2800 50  0001 C CNN
	1    2850 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3100 5650 3100
Text Label 5650 3100 0    50   ~ 0
XTAL1
Text Label 2300 2800 0    50   ~ 0
XTAL1
Wire Wire Line
	2300 2800 2550 2800
Wire Wire Line
	3000 2800 3150 2800
$Comp
L Device:C_Small C1
U 1 1 5B80F278
P 2550 3050
F 0 "C1" H 2642 3096 50  0000 L CNN
F 1 "C_Small" H 2642 3005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2550 3050 50  0001 C CNN
F 3 "~" H 2550 3050 50  0001 C CNN
	1    2550 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5B80F2F8
P 3150 3050
F 0 "C3" H 3242 3096 50  0000 L CNN
F 1 "C_Small" H 3242 3005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3150 3050 50  0001 C CNN
F 3 "~" H 3150 3050 50  0001 C CNN
	1    3150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3150 2550 3250
Wire Wire Line
	2550 3250 2850 3250
Wire Wire Line
	3150 3250 3150 3150
Wire Wire Line
	2850 3250 2850 3300
Connection ~ 2850 3250
Wire Wire Line
	2850 3250 3150 3250
$Comp
L power:GND #PWR0106
U 1 1 5B80FA3F
P 2850 3300
F 0 "#PWR0106" H 2850 3050 50  0001 C CNN
F 1 "GND" H 2855 3127 50  0000 C CNN
F 2 "" H 2850 3300 50  0001 C CNN
F 3 "" H 2850 3300 50  0001 C CNN
	1    2850 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2950 2550 2800
Connection ~ 2550 2800
Wire Wire Line
	2550 2800 2700 2800
Wire Wire Line
	3150 2950 3150 2800
Text Notes 2300 3500 0    50   ~ 0
crystal\n(optional)
$Comp
L Device:C_Small C6
U 1 1 5B818349
P 7400 1900
F 0 "C6" V 7171 1900 50  0000 C CNN
F 1 "100nF" V 7262 1900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7400 1900 50  0001 C CNN
F 3 "~" H 7400 1900 50  0001 C CNN
	1    7400 1900
	0    1    1    0   
$EndComp
Text Notes 7900 1600 0    50   ~ 0
decoupling: close to XMEGA!
Wire Wire Line
	3100 1900 3100 2000
Wire Wire Line
	3100 2000 2700 2000
Wire Wire Line
	2100 1900 2100 2000
Wire Wire Line
	2100 2000 2700 2000
Connection ~ 2700 2000
Wire Wire Line
	2700 1900 2700 2000
Wire Wire Line
	4500 2300 4500 2150
Wire Wire Line
	4500 2150 4450 2150
Wire Wire Line
	7500 1900 7650 1900
Wire Wire Line
	7500 2250 7650 2250
$Comp
L power:GND #PWR0107
U 1 1 5B8266F4
P 7650 2250
F 0 "#PWR0107" H 7650 2000 50  0001 C CNN
F 1 "GND" H 7655 2077 50  0000 C CNN
F 2 "" H 7650 2250 50  0001 C CNN
F 3 "" H 7650 2250 50  0001 C CNN
	1    7650 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 5B828042
P 6700 1400
F 0 "#PWR0108" H 6700 1250 50  0001 C CNN
F 1 "+3.3V" H 6715 1573 50  0000 C CNN
F 2 "" H 6700 1400 50  0001 C CNN
F 3 "" H 6700 1400 50  0001 C CNN
	1    6700 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 1900 7650 2250
Connection ~ 7650 2250
Wire Wire Line
	7300 1900 7150 1900
Connection ~ 7150 1900
$Comp
L Device:C_Small C5
U 1 1 5B82E253
P 7400 1550
F 0 "C5" V 7171 1550 50  0000 C CNN
F 1 "100nF" V 7262 1550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7400 1550 50  0001 C CNN
F 3 "~" H 7400 1550 50  0001 C CNN
	1    7400 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	7500 1550 7650 1550
Wire Wire Line
	7650 1550 7650 1900
Wire Wire Line
	7300 1550 7150 1550
Wire Wire Line
	7150 1550 7150 1900
Text Notes 2400 1950 0    50   ~ 0
LDO
$Comp
L power:GND #PWR0109
U 1 1 5B835F7F
P 7050 5200
F 0 "#PWR0109" H 7050 4950 50  0001 C CNN
F 1 "GND" V 7055 5072 50  0000 R CNN
F 2 "" H 7050 5200 50  0001 C CNN
F 3 "" H 7050 5200 50  0001 C CNN
	1    7050 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6700 5200 7050 5200
$Comp
L Device:R_Small R1
U 1 1 5B837083
P 3950 5000
F 0 "R1" V 3754 5000 50  0000 C CNN
F 1 "10k" V 3845 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3950 5000 50  0001 C CNN
F 3 "~" H 3950 5000 50  0001 C CNN
	1    3950 5000
	0    1    1    0   
$EndComp
Text Notes 2050 1250 0    50   ~ 0
TODO: think about this cap design\n(hence the unpopulated pads)
$Comp
L Connector:AVR-PDI-6 J1
U 1 1 5B83A25B
P 2900 4700
F 0 "J1" H 2620 4746 50  0000 R CNN
F 1 "AVR-PDI-6" H 2620 4655 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" V 2650 4650 50  0001 C CNN
F 3 " ~" H 1625 4150 50  0001 C CNN
	1    2900 4700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0110
U 1 1 5B8414DB
P 4200 5000
F 0 "#PWR0110" H 4200 4850 50  0001 C CNN
F 1 "+3.3V" H 4215 5173 50  0000 C CNN
F 2 "" H 4200 5000 50  0001 C CNN
F 3 "" H 4200 5000 50  0001 C CNN
	1    4200 5000
	-1   0    0    1   
$EndComp
Text Label 4600 4700 2    50   ~ 0
RESET
Wire Wire Line
	6000 2700 5650 2700
Text Label 5650 2700 0    50   ~ 0
RESET
Wire Wire Line
	6000 2800 5650 2800
Text Label 5650 2800 0    50   ~ 0
PDI_DATA
Text Label 3600 4600 2    50   ~ 0
PDI_DATA
$Comp
L power:+3.3V #PWR0111
U 1 1 5B84A0E3
P 2800 4300
F 0 "#PWR0111" H 2800 4150 50  0001 C CNN
F 1 "+3.3V" H 2815 4473 50  0000 C CNN
F 2 "" H 2800 4300 50  0001 C CNN
F 3 "" H 2800 4300 50  0001 C CNN
	1    2800 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5B84A133
P 2800 5100
F 0 "#PWR0112" H 2800 4850 50  0001 C CNN
F 1 "GND" H 2805 4927 50  0000 C CNN
F 2 "" H 2800 5100 50  0001 C CNN
F 3 "" H 2800 5100 50  0001 C CNN
	1    2800 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Female J5
U 1 1 5B84B569
P 7750 3000
F 0 "J5" H 7777 2976 50  0000 L CNN
F 1 "Conn_01x08_Female" H 7777 2885 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7750 3000 50  0001 C CNN
F 3 "~" H 7750 3000 50  0001 C CNN
	1    7750 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 3400 7550 3400
Wire Wire Line
	7400 3300 7550 3300
Wire Wire Line
	7400 3200 7550 3200
Wire Wire Line
	7400 3100 7550 3100
Wire Wire Line
	7400 3000 7550 3000
Wire Wire Line
	7400 2900 7550 2900
Wire Wire Line
	7400 2800 7550 2800
Wire Wire Line
	7400 2700 7550 2700
$Comp
L Connector:Conn_01x08_Female J7
U 1 1 5B853ADE
P 7750 4400
F 0 "J7" H 7777 4376 50  0000 L CNN
F 1 "Conn_01x08_Female" H 7777 4285 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7750 4400 50  0001 C CNN
F 3 "~" H 7750 4400 50  0001 C CNN
	1    7750 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4100 7550 4100
Wire Wire Line
	7400 4200 7550 4200
Wire Wire Line
	7400 4300 7550 4300
Wire Wire Line
	7400 4400 7550 4400
Wire Wire Line
	7400 4500 7550 4500
Wire Wire Line
	7400 4600 7550 4600
Wire Wire Line
	7400 4800 7550 4800
Wire Wire Line
	6000 4800 5900 4800
Wire Wire Line
	6000 4700 5950 4700
Wire Wire Line
	5950 4700 5950 5200
Wire Wire Line
	5900 4800 5900 5300
Wire Wire Line
	5500 5200 5950 5200
Wire Wire Line
	5500 5300 5900 5300
Wire Wire Line
	3300 4600 3600 4600
$Comp
L Switch:SW_Push SW1
U 1 1 5B8BB05A
P 4300 4300
F 0 "SW1" V 4254 4448 50  0000 L CNN
F 1 "SW_Push" V 4345 4448 50  0000 L CNN
F 2 "Button_Switch_SMD:SW_SPST_FSMSM" H 4300 4500 50  0001 C CNN
F 3 "" H 4300 4500 50  0001 C CNN
	1    4300 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 4500 4300 4700
Connection ~ 4300 4700
Wire Wire Line
	4300 4700 4600 4700
Wire Notes Line
	4850 4450 3800 4450
Wire Notes Line
	3800 3400 4850 3400
Wire Wire Line
	4050 5000 4200 5000
Wire Notes Line
	3650 4050 3650 5350
Wire Notes Line
	2150 5350 2150 4050
Text Notes 4800 3600 2    50   ~ 0
reset button\n(optional)
Text Notes 3500 4350 2    50   ~ 0
PDI connector\n(optional)
Wire Notes Line
	2200 3550 2200 2450
Wire Notes Line
	3600 2450 3600 3550
Text Notes 5150 2200 2    50   ~ 0
USB connector
Wire Wire Line
	6000 3000 5650 3000
Text Label 5650 3000 0    50   ~ 0
XTAL2
Wire Wire Line
	3150 2800 3450 2800
Connection ~ 3150 2800
Text Label 3450 2800 2    50   ~ 0
XTAL2
Wire Notes Line
	2200 3550 3600 3550
Wire Notes Line
	2200 2450 3600 2450
Wire Notes Line
	2150 5350 3650 5350
Wire Notes Line
	2150 4050 3650 4050
Wire Wire Line
	3300 4700 3750 4700
Wire Wire Line
	3850 5000 3750 5000
Wire Wire Line
	3750 5000 3750 4700
Connection ~ 3750 4700
Wire Wire Line
	3750 4700 4300 4700
Text Notes 3850 3700 0    50   ~ 0
(no cap due to PDI)
Wire Wire Line
	4300 3800 4500 3800
$Comp
L power:GND #PWR0113
U 1 1 5B98A9B8
P 4500 3800
F 0 "#PWR0113" H 4500 3550 50  0001 C CNN
F 1 "GND" V 4505 3672 50  0000 R CNN
F 2 "" H 4500 3800 50  0001 C CNN
F 3 "" H 4500 3800 50  0001 C CNN
	1    4500 3800
	0    -1   -1   0   
$EndComp
Text Notes 6200 1600 0    50   ~ 0
power cut
$Comp
L Device:R_Small R2
U 1 1 5B843378
P 4300 3950
F 0 "R2" H 4359 3996 50  0000 L CNN
F 1 "0" H 4359 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 4300 3950 50  0001 C CNN
F 3 "~" H 4300 3950 50  0001 C CNN
	1    4300 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4100 4300 4050
Wire Wire Line
	4300 3800 4300 3850
Wire Notes Line
	4850 3400 4850 4450
Wire Notes Line
	3800 3400 3800 4450
$Comp
L MCU_Microchip_ATmega:ATxmega128A4U-AU U2
U 1 1 5B80331B
P 6700 3800
F 0 "U2" H 6700 2314 50  0000 C CNN
F 1 "ATxmega128A4U-AU" H 6700 2223 50  0000 C CNN
F 2 "Package_QFP:TQFP-44_10x10mm_P0.8mm" H 6700 3800 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8387-8-and16-bit-AVR-Microcontroller-XMEGA-A4U_Datasheet.pdf" H 6700 3800 50  0001 C CNN
	1    6700 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:Ferrite_Bead_Small L1
U 1 1 5B878050
P 6800 2100
F 0 "L1" H 6900 2146 50  0000 L CNN
F 1 "Ferrite_Bead_Small" H 6900 2055 50  0000 L CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 6730 2100 50  0001 C CNN
F 3 "~" H 6800 2100 50  0001 C CNN
	1    6800 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6800 1900 6700 1900
Wire Wire Line
	6700 1900 6700 2400
Wire Wire Line
	6800 1900 7150 1900
Connection ~ 6800 1900
Wire Wire Line
	6800 2400 6800 2250
Wire Wire Line
	6800 2250 6800 2200
Connection ~ 6800 2250
Wire Wire Line
	6800 1900 6800 2000
Connection ~ 6700 1900
$Comp
L Device:Polyfuse_Small F1
U 1 1 5B8B3AD1
P 5050 1550
F 0 "F1" V 4845 1550 50  0000 C CNN
F 1 "fuse" V 4936 1550 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 5100 1350 50  0001 L CNN
F 3 "~" H 5050 1550 50  0001 C CNN
	1    5050 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 1550 4950 1550
$Comp
L power:+5V #PWR0102
U 1 1 5B8DDF24
P 5450 1550
F 0 "#PWR0102" H 5450 1400 50  0001 C CNN
F 1 "+5V" H 5465 1723 50  0000 C CNN
F 2 "" H 5450 1550 50  0001 C CNN
F 3 "" H 5450 1550 50  0001 C CNN
	1    5450 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 1550 5450 1550
Text Notes 3850 5300 0    50   ~ 0
keep reset high
Text Notes 6500 2300 2    50   ~ 0
bead for AVCC\n(or 0-ohm resistor)
Wire Wire Line
	7500 5050 7550 5050
$Comp
L Device:Jumper_NO_Small JP1
U 1 1 5B931282
P 7650 5050
F 0 "JP1" H 7650 5235 50  0000 C CNN
F 1 "0" H 7650 5144 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7650 5050 50  0001 C CNN
F 3 "~" H 7650 5050 50  0001 C CNN
	1    7650 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	7750 5050 7800 5050
$Comp
L power:GND #PWR0114
U 1 1 5B95E86E
P 7800 5050
F 0 "#PWR0114" H 7800 4800 50  0001 C CNN
F 1 "GND" V 7805 4922 50  0000 R CNN
F 2 "" H 7800 5050 50  0001 C CNN
F 3 "" H 7800 5050 50  0001 C CNN
	1    7800 5050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6800 2250 7050 2250
NoConn ~ 4750 1950
$Comp
L power:+5V #PWR0116
U 1 1 5B9C901E
P 1300 2400
F 0 "#PWR0116" H 1300 2250 50  0001 C CNN
F 1 "+5V" H 1315 2573 50  0000 C CNN
F 2 "" H 1300 2400 50  0001 C CNN
F 3 "" H 1300 2400 50  0001 C CNN
	1    1300 2400
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5B9BA351
P 1300 2400
F 0 "#FLG0101" H 1300 2475 50  0001 C CNN
F 1 "PWR_FLAG" H 1300 2574 50  0000 C CNN
F 2 "" H 1300 2400 50  0001 C CNN
F 3 "~" H 1300 2400 50  0001 C CNN
	1    1300 2400
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5BA35F63
P 7050 2250
F 0 "#FLG0103" H 7050 2325 50  0001 C CNN
F 1 "PWR_FLAG" H 7050 2424 50  0000 C CNN
F 2 "" H 7050 2250 50  0001 C CNN
F 3 "~" H 7050 2250 50  0001 C CNN
	1    7050 2250
	1    0    0    -1  
$EndComp
Connection ~ 7050 2250
Wire Wire Line
	7050 2250 7300 2250
$Comp
L Device:C_Small C11
U 1 1 5BCBA323
P 7400 1200
F 0 "C11" V 7171 1200 50  0000 C CNN
F 1 "100nF" V 7262 1200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7400 1200 50  0001 C CNN
F 3 "~" H 7400 1200 50  0001 C CNN
	1    7400 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	7500 1200 7650 1200
Wire Wire Line
	7650 1200 7650 1550
Wire Wire Line
	7300 1200 7150 1200
Wire Wire Line
	7150 1200 7150 1550
Connection ~ 7650 1550
Connection ~ 7150 1550
Wire Wire Line
	3100 1600 3800 1600
Wire Wire Line
	1500 1600 2100 1600
Wire Wire Line
	7400 4700 7550 4700
$Comp
L power:+3.3V #PWR0115
U 1 1 5C0349B7
P 7500 5050
F 0 "#PWR0115" H 7500 4900 50  0001 C CNN
F 1 "+3.3V" H 7515 5223 50  0000 C CNN
F 2 "" H 7500 5050 50  0001 C CNN
F 3 "" H 7500 5050 50  0001 C CNN
	1    7500 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1400 6700 1900
$EndSCHEMATC
