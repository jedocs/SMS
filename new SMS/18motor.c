/*
 
+CMT: "+36305601204",,"13/12/16,00:19:23+04"
1234
0Rac_ab_af\``jaijbc[`dR=:0abcd=00000000000000
ERROR

+CMT: "+36305601204",,"13/12/16,00:21:47+04"
123
R[cfc`ef`ab`dR\0Rac_ab_af\``jbajdg[`dR=:0abc=000000000000000ignore
ERROR
*/
//üzenet vége eeprom-ban: 00 FF !!!!!!!!!!!!!
// PIC18F2525 Configuration Bit Settings

#include 	<timers.h>					// Timer library functions
#include	"sms.h"
#include	<p18f2525.h>

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
//------------------------------------------------

unsigned char telszam[40];
unsigned char datum[40];
unsigned char uzenet[40];
unsigned char i,di1_bounce, di2_bounce, di3_bounce, di4_bounce,parse = 0;
unsigned int timer = 0,charno = 0, delay;
long temp;

struct { // Holds status bits
    unsigned di1 : 1;
    unsigned di2 : 1;
    unsigned di3 : 1;
    unsigned di4 : 1;
    unsigned sms : 1;
    unsigned ss : 1;
    unsigned leallt : 1;
    unsigned elindult : 1;
} flag;

struct { // Holds status bits
    unsigned smsok : 1;
    unsigned msgok : 1;
    unsigned tx : 1;
    unsigned dd : 1;
    unsigned sg : 1;
    unsigned l : 1;
    unsigned lo : 1;
    unsigned tp : 1;
} flag1;

//---------------------------------------------------------------------´
// Interrupt Code
//---------------------------------------------------------------------

#pragma interrupt servo_isr save = temp	// Designate servo_isr as an interrupt function and save key registers

#pragma code isrcode=0x0008 //0x0808				// Locate ISR handler code at interrupt vector

void isrhandler(void) // This function directs execution to the
{ // actual interrupt code
    _asm
    goto servo_isr
            _endasm
}

#pragma code

//---------------------------------------------------------------------
// servo_isr()
// Performs the servo calculations
//---------------------------------------------------------------------

void servo_isr(void) {

    if (delay) delay--;
    if (timer) timer--;

    // DI2 debounce ************************************************
    if (DI2 ^ flag.di2) {
        di2_bounce--;
        if (!di2_bounce) {
            if (DI2) {
                flag.elindult=1;//event = event | DI_2_WENT_HI;
                flag.di2 = 1;
                di2_bounce = DEBOUNCE_DELAY;
            } else {
                flag.leallt=1;//event = event | DI_2_WENT_LOW;
                flag.di2 = 0;
                di2_bounce = DEBOUNCE_DELAY;
            }
        } //if (!di2_bounce)
    }//if (DI2 ^ flag.di1)
    else {
        di2_bounce = DEBOUNCE_DELAY;
    }
    PIR1bits.TMR2IF = 0; // Clear Timer2 Interrupt Flag.
}

//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void Setup(void) {

    PIE2 = 0;
    PIE1 = 0;

    OSCCON = 0;
    OSCTUNE = 0;
    HLVDCON = 0;
    WDTCON = 0;
    RCON = 0;

    PORTA = 0;
    TRISA = 0x2F;
    PORTB = 0;
    TRISB = 0b00111111;
    TRISC = 0xFF;
    PORTC = 0;

    RCSTA = 0b00010000;
    TXSTA = 0b00000110;
    BAUDCON = 0b01000000;
    SPBRG = 25; //baudrate: 9600bps
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

    i = RCREG;
    i = RCREG;

    ADCON0 = 0b00000001;
    ADCON1 = 0b00010110;
    ADCON2 = 0b10111001; //right justified,ACQ time 20 TAD, clock Fosc/8

    IPR2 = 0;
    IPR1 = 0;
    PIR2 = 0;
    PIR1 = 0;
    INTCON = 0;
    INTCON2 = 0b10000000;
    INTCON3 = 0b00000000;

    INTCON2bits.RBPU = 1; // RBPU disable

    temp = 0;

    OpenTimer2(T2_PS_1_4 & T2_POST_1_4 & TIMER_INT_ON); // 244 Hz int.

    flag.sms = 0;
    flag1.smsok = 0;
    flag1.msgok = 0;
    timer = 0;
    flag.leallt = 0;
    flag.elindult = 0;

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    CHGEN = 1;

    PWRKEY = 0;
    PORTBbits.RB7 = 1;
}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void) {

    Setup(); // Setup peripherals and software

    GSM_PwrOn();

    while (1) // Loop forever
    { //WDT!

        if (!VDDEXT) GSM_PwrOn();

        if (RCSTAbits.OERR) {
            RCSTAbits.CREN = 0; // Disable UART receiver
            RCSTAbits.CREN = 1; // Enable UART receiver
        }

        if (PIR1bits.RCIF && !flag.sms && !flag1.tx) // Check for USART interrupt
        {
            switch (parse) {

                case 0:
                {
                    if (RCREG == '+' && !timer) {
                        timer = timeout;
                        charno = 0;

                    } else if (timer) {

                        switch (charno) {

                            case 0:
                            {
                                if (RCREG == 'C') charno = 1;
                                else goto IGNORE;
                            }
                                break;

                            case 1:
                            {
                                if (RCREG == 'M') charno = 2;
                                else goto IGNORE;
                            }
                                break;

                            case 2:
                            {
                                if (RCREG == 'T') charno = 3;
                                else goto IGNORE;
                            }
                                break;

                            case 3:
                            {
                                if (RCREG == ':') charno = 4;
                                else goto IGNORE;
                            }
                                break;

                            case 4:
                            {
                                if (RCREG == ' ') {
                                    parse = 1;
                                    i = 0;

                                } else goto IGNORE;
                            }
                                break;

                            default:
                            {
                                goto IGNORE;
                            }
                        }
                    }
                }
                    break;

                case 1:
                {
                    if (timer) {

                        telszam[i] = RCREG;

                        if (telszam[i] == ',' && i == 15) {
                            parse = 2;
                            i = 0;
                        }
                        if (i > 15) goto IGNORE;
                        i++;
                    } else goto IGNORE;
                }
                    break;

                case 2:

                    if (timer) {

                        datum[i] = RCREG;

                        if (datum[i] == 10 && i == 24) {
                            parse = 3;
                            i = 0;
                        }
                        if (i > 24) goto IGNORE;
                        i++;
                    } else goto IGNORE;
                    break;

                case (3):

                    if (timer) {
                        uzenet[i] = RCREG;

                        if (uzenet[i] == 13) {
                            flag.sms = 1;
                            timer = 0;
                            parse = 0;
                        }
                        if (i > 6) goto IGNORE;
                        i++;
                    } else goto IGNORE;
            }

            goto TOVABB;

IGNORE:
            timer = 0;
            parse = 0;
            for (i = 0; i < 30; i++) {
                telszam[i] = 0;
                datum[i] = 0;
                uzenet[i] = 0;
            }
            flag.sms = 0;
TOVABB:
            ;
        }

        if (flag.sms) {

            flag1.smsok = 1;
            for (i = 1; i < 13; i++) {
                if (QUERY_NO[i] != telszam[i]) flag1.smsok = 0;
            }

            flag1.msgok = 1;
            for (i = 1; i < 5; i++) {
                if (QUERY_MSG[i] != uzenet[i]) flag1.msgok = 0;
            }

            if (flag1.smsok && flag1.msgok && flag.sms) {

                if (flag.di2) {
                    PORTBbits.RB6 = 1;
                    putrsUSART(DI2_HI_query);
                    delay = 1000;
                    while (delay);
                    PORTBbits.RB6 = 0;
                } else {
                    PORTBbits.RB6 = 1;
                    putrsUSART(DI2_LOW_query);
                    delay = 1000;
                    while (delay);
                    PORTBbits.RB6 = 0;
                }
                flag1.smsok = 0;
                flag1.msgok = 0;
            }

            for (i = 0; i < 30; i++) {
                telszam[i] = 0;
                datum[i] = 0;
                uzenet[i] = 0;
            }
            flag.sms = 0;
        }

        if (flag.leallt) {

            PORTBbits.RB6 = 1;
            flag.leallt = 0;
            putrsUSART(DI2_LOW);
            delay = 1000;
            while (delay);
            PORTBbits.RB6 = 0;

#if defined (MULTINO)
            PORTBbits.RB6 = 1;
            flag.leallt = 0;
            putrsUSART(DI2_LOW1);
            delay = 1000;
            while (delay);
            PORTBbits.RB6 = 0;
#endif
        }

        if (flag.elindult) {
            PORTBbits.RB6 = 1;
            flag.elindult = 0;
            putrsUSART(DI2_HI);
            delay = 1000;
            while (delay);
            PORTBbits.RB6 = 0;

#if defined (MULTINO)
            PORTBbits.RB6 = 1;
            flag.elindult = 0;
            putrsUSART(DI2_HI1);
            delay = 1000;
            while (delay);
            PORTBbits.RB6 = 0;
#endif
        }
    }
}
//---------------------------------------------------------------------
// putrsUSART()
// Writes a string of characters in program memory to the USART
//---------------------------------------------------------------------

void putrsUSART(const rom char *data) {

    flag1.tx = 1;
    do {
        delay = 10;
        while (delay);

        while (!(TXSTA & 0x02));
        if (*data) TXREG = *data;
    } while (*data++);
    flag1.tx = 0;
}

//---------------------------------------------------------------------
// putsUSART()
// Writes a string of characters in data memory to the USART
//---------------------------------------------------------------------

void putsUSART(char *data) {

    flag1.tx = 1;

    do {
        delay = 10;
        while (delay);

        while (!(TXSTA & 0x02));
        if (*data) TXREG = *data;
    } while (*data++);
    flag1.tx = 0;
}
//------------------------------------------------------------------

void GSM_PwrOn(void) {
    if (!VDDEXT) {

        PWRKEY = 0;
        delay = 800;
        while (delay);
        PWRKEY = 1;
    }
    PORTBbits.RB7 = 0;
    delay = 5000;
    while (delay);
    PORTBbits.RB6 = 1;
    putrsUSART(IPR);
    putrsUSART(CMGF);
    putrsUSART(CNMI);
    putrsUSART(ATW);
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 1;
}

