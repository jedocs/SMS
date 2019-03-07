
; Initialization Code for PIC18F2525, Family: GP control, Package: 28-Pin SDIP 28pins

#include P18F2525.inc



; Filter Controls used to Generate Code:

; POR Match Filter OFF

; Provisioned Feature Filter ON

; Masks are Ignored and uses UnMasked Register Writes

; Feature=fuses - fuses (DCR) configuration

; B7=IESO B6=FCMEN B3:0=FOSC3:0

CONFIG1H=0x02

; B4:3=BORV1:0 B2:1=BOREN1:0 B0=PWRTEN-L

CONFIG2L=0xF7

; B4:1=WDTPS3:0 B0=WDTEN

CONFIG2H=0xFF

; B7=MCLRE B2=LPT1OSC B1=PBADEN B0=CCP2MX

CONFIG3H=0xFF

; B7=DEBUG-L B6=XINST B2=LVP B0=STVREN

CONFIG4L=0xBF

; B3:0=CP3:0

CONFIG5L=0xFF

; B7=CPD B6=CPB

CONFIG5H=0xFF

; B3:0=WR3:0

CONFIG6L=0xFF

; B7=WRTD B6=WRTB B5=WRTC

CONFIG6H=0xFF

; B3:0=EBTR3:0

CONFIG7L=0xFF

; B6=EBTRB

CONFIG7H=0xFF

VI_CODE	CODE

VisualInitialization:

; Feature=Interrupts - Disable Interrupts during configuration

; B4=EE B3=BCL B2=LVD B1=TMR3 B0=CCP2

	clrf PIE2

; B7=PSP B6=AD B5=RC1 B4=TX1 B3=SSP B2=CCP1 B1=TMR2 B0=TMR1

	clrf PIE1

; Feature=Oscillator - Oscillator configuration

; B7=IDLEN B6:4=IRCF2:0 B3=OSTS B2=IOFS B1:0=SCS1:0

	clrf OSCCON

; B7=INTSRC B6=PLLEN B4:0=TUN4:0

	clrf OSCTUNE

; Feature=Reset - Reset configuration

; B5=IRVST B4=LVDEN B3:0=LVDL3:0

	clrf LVDCON

; B0=SWDTEN

	clrf WDTCON

; B7=IPEN B6=SBOREN B4=RI-L B3=TO-L B2=PD-L B1=POR-L B0=BOR-L

	clrf RCON

; Feature=IOPortA - IO Ports configuration

; port A is 6 bits wide

; set TRIS to all inputs before setting initial value

	movlw 0x3F
	movwf TRISA

	movlw 0x10
	movwf PORTA

; set port bit as input (1) or output (0)

	movlw 0xEF
	movwf TRISA

; Feature=IOPortB - IO Ports configuration

; port B is 8 bits wide

; set TRIS to all inputs before setting initial value

	movlw 0xFF
	movwf TRISB

	movlw 0x10
	movwf PORTB

; set port bit as input (1) or output (0)

	movlw 0xEF
	movwf TRISB

; Feature=IOPortC - IO Ports configuration

; port C is 8 bits wide

; set TRIS to all inputs before setting initial value

	movlw 0xFF
	movwf TRISC

	clrf PORTC

; set port bit as input (1) or output (0)

	movlw 0xFF
	movwf TRISC

; Feature=IOPortD - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortE - IO Ports configuration

; Feature code removed since notprovisioned.

; Feature=IOPortF - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortG - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortH - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortJ - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=PSP - PSP configuration

; Feature code removed since notprovisioned.

; Feature=EMI - EMI configuration

; Feature code removed since does not exist on this device.

; Feature=CCP1 - CCP configuration

; Feature code removed since notprovisioned.

; Feature=CCP2 - CCP configuration

; Feature code removed since notprovisioned.

; Feature=CCP3 - CCP configuration

; Feature code removed since does not exist on this device.

; Feature=CCP4 - CCP configuration

; Feature code removed since does not exist on this device.

; Feature=CCP5 - CCP configuration

; Feature code removed since does not exist on this device.

; Feature=MSSP - MSSP configuration

; Feature code removed since notprovisioned.

; Feature=UART1 - USART configuration

; (RCSTA)B7=SPEN B6=RX9 B5=SREN B4=CREN B3=ADDEN B2=FERR B1=OERR B0=RX9D

; (TXSTA)B7=CSRC B6=TX9 B5=TXEN B4=SYNC B2=BRGH B1=TRMT B0=TX9D

; (SPBRG)Baud rate generator

; (RCREG)Receive register

	movlw 0x80                             ; set up receive options
	movwf RCSTA

	movlw 0x36                             ; set up transmit options
	movwf TXSTA

	movlw 0x40                             ; set up Baud Rate control options
	movwf BAUDCON

	movlw 0x19                             ; set up baud
	movwf SPBRG

	movf RCREG, W                          ; flush receive buffer

	movf RCREG, W

; Feature=UART2 - USART configuration

; Feature code removed since does not exist on this device.

; Feature=A2D - A2D configuration

; Feature code removed since notprovisioned.

; Feature=Comparator1 - Comparator configuration

; Feature code removed since notprovisioned.

; Feature=VoltageRef - Voltage Reference configuration

; Feature code removed since notprovisioned.

; Feature=required - Interrupt flags cleared and interrupt configuration

; interrupt priorities

; B4=EE B3=BCL B2=LVD B1=TMR3 B0=CCP2

	movlw 0xFF
	movwf IPR2

; B7=PSP B6=AD B5=RC1 B4=TX1 B3=SSP B2=CCP1 B1=TMR2 B0=TMR1

	movlw 0xFF
	movwf IPR1

; clear int flags

; B4=EE B3=BCL B2=LVD B1=TMR3 B0=CCP2

	clrf PIR2

; B7=PSP B6=AD B5=RC1 B4=TX1 B3=SSP B2=CCP1 B1=TMR2 B0=TMR1

	clrf PIR1

; global and external interrupt enables

; B7=GIE B6=PEIE B5=TMR0IE B4=INTOIE B3=RBIE B2=TMR0IF B1=INTOIF B0=RBIF

	movlw 0xC0
	movwf INTCON

; B7=RBPU-L B6:4=INTEDG0:2 B2=TMR0IP B0=RBIP

	movlw 0xFF
	movwf INTCON2

; B7:6=INT2:1IP B4:3=INT2:1IE B1:0=INT2:1IF

	movlw 0xC0
	movwf INTCON3

; Feature=Timer0 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer2 - Timers configuration

; (CON)B6:3=TOUTPS3:0 B2=TMRON B1:0=TCKPS1:0

; (TMR)Timer register (cleared)

; (PR)Timer preload register (set)

	bcf T2CON,TMR2ON

	movlw 0x1D                             ; set options with timer on/off (bit2)
	movwf T2CON

	clrf TMR2                              ; preset timer values

	movlw 0xFA                             ; preload timer values
	movwf PR2

; Feature=Timer4 - Timers configuration

; Feature code removed since does not exist on this device.

; Feature=Timer1 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer3 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=CPU - CPU register configuration

; Feature=Interrupts - enable interrupts

; feature interrupt enables

; B4=EE B3=BCL B2=LVD B1=TMR3 B0=CCP2

	clrf PIE2

; B7=PSP B6=AD B5=RC1 B4=TX1 B3=SSP B2=CCP1 B1=TMR2 B0=TMR1

	movlw 0x02
	movwf PIE1

; interrupt priorities

; B4=EE B3=BCL B2=LVD B1=TMR3 B0=CCP2

	movlw 0xFF
	movwf IPR2

; B7=PSP B6=AD B5=RC1 B4=TX1 B3=SSP B2=CCP1 B1=TMR2 B0=TMR1

	movlw 0xFF
	movwf IPR1

	return

; Feature=Reset - Reset Error Handlers

	GLOBAL VisualInitialization

	END
