#define telszam "+36305601204";
//üzenet vége eeprom-ban: 00 FF !!!!!!!!!!!!!

//AT+CMGF=1 !!!!!!!!!!!!4
//AT+IPR=9600
//!!+CSAS       AT&W0

// PIC18F2525 Configuration Bit Settings

#include <p18F2525.h>

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



//#pragma config OSC = HS
//#pragma config PWRT = ON
//#pragma config WDT = OFF
//#pragma config WDTPS = 1
//#pragma config MCLRE = ON
//#pragma config PBADEN = OFF
//#pragma config LVP = OFF

#include 	<timers.h>					// Timer library functions
#include	"sms.h"
#include	<p18f2525.h>

//---------------------------------------------------------------------
//Constant Definitions
//---------------------------------------------------------------------

#define VpsupI	PORTAbits.RA0
#define	VbatI	PORTAbits.RA1		
#define	AI2		PORTAbits.RA2

#define PWRKEY	PORTAbits.RA4
#define AI3		PORTAbits.RA5

#define RI		PORTBbits.RB0
#define STAT2	PORTBbits.RB1
#define AI1		PORTBbits.RB2
#define DI1		PORTBbits.RB3		
#define CHGEN	PORTBbits.RB4
#define VDDEXT	PORTBbits.RB5
#define DI4		PORTBbits.RB6

#define	DI2		PORTCbits.RC1		
#define DI3		PORTCbits.RC2


//---------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------

const rom char SEND_CMD[] = "\rAT+CMGS=";
const rom char QUOTE[] = "\x22";
const rom char CR[] = "\x0D";
const rom char CTRL_Z[] = "\x1A";

const rom char PHONENO[]="\rAT+CMGS=\"+36305601204\"\rSzhely SMS kuldo bekapcsolas. \x1A";
const rom char PHONENO[]="\rAT+CMGS=\""+telszam+"\"\rSzhely SMS kuldo bekapcsolas. \x1A";

const rom char PWRONMSG[]="Szhely SMS kuldo bekapcsolas";
const rom char CMGF[]="\rAT+CMGF=1\r";
const rom char IPR[]="\rAT\rAT\rAT\rAT\rAT+IPR=9600\r";
const rom char ATW[]="\rAT&W0\r";
const rom char AT[]="ATATATATATAT";
//+CSAS       AT&W0


char message[38];
char data[38];

unsigned char
event_numbers, query_number, i,
di1_bounce, di2_bounce, di3_bounce, di4_bounce;

struct { // Holds status bits
    unsigned di1 : 1;
    unsigned di2 : 1;
    unsigned di3 : 1;
    unsigned di4 : 1;
    unsigned d : 1;
    unsigned e : 1;
    unsigned f : 1;
    unsigned g : 1;
} flag;

int event, delay;
long temp;

union INT {
    int i;
    char b[2];
};

union INT
adin,
address;

//---------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------

void servo_isr(void); // Does servo calculations
void isrhandler(void); // Located at high interrupt vector

void Setup(void); // Configures peripherals and variables
void GSM_PwrOn(void);

void putrsUSART(const rom char *data); // Writes a string from ROM to the USART
void putsUSART(char *data); // Writes a string from RAM to the USART

char readEEPROM(int addr);
void writeEEPROM(int addr, char data);

void readStringEEPROM(int addr, char string[38], char final_character);
void writeStringEEPROM(int addr, char string[38]);

void sendSMS(unsigned char event_numbers, char message[38]);


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

    // DI1 debounce ************************************************
    if (DI1 ^ flag.di1) {
        di1_bounce--;
        if (!di1_bounce) {
            if (DI1) {
                event = event | DI_1_WENT_HI;
                flag.di1 = 1;
                di1_bounce = DEBOUNCE_DELAY;
            } else {
                event = event | DI_1_WENT_LOW;
                flag.di1 = 0;
                di1_bounce = DEBOUNCE_DELAY;
            }
        } //if (!di1_bounce)
    }//if (DI1 ^ flag.di1)

    else {
        di1_bounce = DEBOUNCE_DELAY;
    }

    // DI2 debounce ************************************************
    if (DI2 ^ flag.di2) {
        di2_bounce--;
        if (!di2_bounce) {
            if (DI2) {
                event = event | DI_2_WENT_HI;
                flag.di2 = 1;
                di2_bounce = DEBOUNCE_DELAY;
            } else {
                event = event | DI_2_WENT_LOW;
                flag.di2 = 0;
                di2_bounce = DEBOUNCE_DELAY;
            }
        } //if (!di2_bounce)
    }//if (DI2 ^ flag.di1)

    else {
        di2_bounce = DEBOUNCE_DELAY;
    }

    // DI3 debounce *************************************************
    if (DI3 ^ flag.di3) {
        di3_bounce--;
        if (!di3_bounce) {
            if (DI3) {
                event = event | DI_3_WENT_HI;
                flag.di3 = 1;
                di3_bounce = DEBOUNCE_DELAY;
            } else {
                event = event | DI_3_WENT_LOW;
                flag.di3 = 0;
                di3_bounce = DEBOUNCE_DELAY;
            }
        } //if (!di3_bounce)
    }//if (DI3 ^ flag.di3)

    else {
        di3_bounce = DEBOUNCE_DELAY;
    }

    // DI4 debounce **************************************************
    if (DI4 ^ flag.di4) {
        di4_bounce--;
        if (!di4_bounce) {
            if (DI4) {
                event = event | DI_4_WENT_HI;
                flag.di4 = 1;
                di4_bounce = DEBOUNCE_DELAY;
            } else {
                event = event | DI_4_WENT_LOW;
                flag.di4 = 0;
                di4_bounce = DEBOUNCE_DELAY;
            }
        } //if (!di4_bounce)
    }//if (DI4 ^ flag.di4)

    else {
        di4_bounce = DEBOUNCE_DELAY;
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

    TRISA = 0x3F;
    PORTA = 0;
    TRISA = 0x2F;

    TRISB = 0xFF;
    PORTB = 0;
    TRISB = 0b00111111;

    TRISC = 0xFF;
    PORTC = 0;
    //	TRISCbits.TRISC6=0;

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

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    CHGEN = 1;

    PWRKEY = 0;
	PORTBbits.RB7 = 1;
    //	event = 0x0007;
    //	query_number= 8;

}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void) {
  
  	Setup(); // Setup peripherals and software

    GSM_PwrOn();

			PORTBbits.RB7=0;
            delay = 5000;
            while (delay);
			PORTBbits.RB6=1;


putrsUSART(IPR);
putrsUSART(CMGF);
putrsUSART(ATW);


			//putrsUSART(PHONENO);

// event = event | DI_3_WENT_LOW;
 
    	PORTBbits.RB6=0;
	PORTBbits.RB7=1;
    
while (1) // Loop forever
    {
        //ClrWdt();							// Clear the WDT

        //*****************************************************************************

        if (event) {
            //			if ((event & DI_1_WENT_LOW))
            //				{
            //				event_numbers = readEEPROM(EVENT_1_NUMBERS);
            //				readStringEEPROM (DI_1_WENT_LOW_MESSAGE, message, 0xff);
            //				sendSMS (event_numbers,message);
            //				event = event & !DI_1_WENT_LOW;
            //				}
            //
            //			if ((event & DI_1_WENT_HI))
            //				{
            //				event_numbers = readEEPROM(EVENT_1_NUMBERS);
            //				readStringEEPROM (DI_1_WENT_HI_MESSAGE, message, 0xff);
            //				sendSMS (event_numbers,message);
            //				event = event & !DI_1_WENT_HI;
            //	     		}

            //	    	if ((event & DI_1_QUERY))
            //				{
            //	            if (DI1) readStringEEPROM (DI_1_QUERY_HI_ANSWER, message, 0xff);
            //				else readStringEEPROM (DI_1_QUERY_LOW_ANSWER, message, 0xff);
            //				sendSMS (query_number,message);
            //			   	event = event & !DI_1_QUERY;
            //				}
            //______________________________________________

            if ((event & DI_2_WENT_LOW)) {
                event_numbers = readEEPROM(EVENT_2_NUMBERS);
                readStringEEPROM(DI_2_WENT_LOW_MESSAGE, message, 0xff);
                sendSMS(event_numbers, message);
                event = event & !DI_2_WENT_LOW;
            }

            if ((event & DI_2_WENT_HI)) {
                event_numbers = readEEPROM(EVENT_2_NUMBERS);
                readStringEEPROM(DI_2_WENT_HI_MESSAGE, message, 0xff);
                sendSMS(event_numbers, message);
                event = event & !DI_2_WENT_HI;
            }

            if ((event & DI_2_QUERY)) {
                if (DI2) readStringEEPROM(DI_2_QUERY_HI_ANSWER, message, 0xff);
                else readStringEEPROM(DI_2_QUERY_LOW_ANSWER, message, 0xff);
                sendSMS(query_number, message);
                event = event & !DI_2_QUERY;
            }

            //______________________________________________

            if ((event & DI_3_WENT_LOW)) {
                event_numbers = readEEPROM(EVENT_3_NUMBERS);
                readStringEEPROM(DI_3_WENT_LOW_MESSAGE, message, 0xff);
                sendSMS(event_numbers, message);
                event = event & !DI_3_WENT_LOW;
            }

            if ((event & DI_3_WENT_HI)) {
                event_numbers = readEEPROM(EVENT_3_NUMBERS);
                readStringEEPROM(DI_3_WENT_HI_MESSAGE, message, 0xff);
                sendSMS(event_numbers, message);
                event = event & !DI_3_WENT_HI;
            }

            if ((event & DI_3_QUERY)) {
                if (DI3) readStringEEPROM(DI_3_QUERY_HI_ANSWER, message, 0xff);
                else readStringEEPROM(DI_3_QUERY_LOW_ANSWER, message, 0xff);
                sendSMS(query_number, message);
                event = event & !DI_3_QUERY;
            }
            //______________________________________________

            //			if ((event & DI_4_WENT_LOW))
            //				{
            //				event_numbers = readEEPROM(EVENT_4_NUMBERS);
            //				readStringEEPROM (DI_4_WENT_LOW_MESSAGE, message, 0xff);
            //				sendSMS (event_numbers,message);
            //				event = event & !DI_4_WENT_LOW;
            //				}

            //			if ((event & DI_4_WENT_HI))
            //				{
            //				event_numbers = readEEPROM(EVENT_4_NUMBERS);
            //				readStringEEPROM (DI_4_WENT_HI_MESSAGE, message, 0xff);
            //				sendSMS (event_numbers,message);
            //				event = event & !DI_4_WENT_HI;
            //	     		}

            //	    	if ((event & DI_4_QUERY))
            //				{
            //	            if (DI4) readStringEEPROM (DI_4_QUERY_HI_ANSWER, message, 0xff);
            //				else readStringEEPROM (DI_4_QUERY_LOW_ANSWER, message, 0xff);
            //				sendSMS (query_number,message);
            //			   	event = event & !DI_4_QUERY;
            //				}
        }

    } // variables.
}

//---------------------------------------------------------------------
// putrsUSART()
// Writes a string of characters in program memory to the USART
//---------------------------------------------------------------------

void putrsUSART(const rom char *data) {
    do {
    	delay=10;
		while(delay);
	    
		while (!(TXSTA & 0x02));
        if (*data) TXREG = *data;
    } while (*data++);
}


//---------------------------------------------------------------------
// putsUSART()
// Writes a string of characters in data memory to the USART
//---------------------------------------------------------------------

void putsUSART(char *data) {
    do {
        delay=10;
		while(delay);
	    
		while (!(TXSTA & 0x02));
        if (*data) TXREG = *data;
    } while (*data++);
}

///////////////////////////////////////////////////////////////////////////////
// writeEEPROM
//

void writeEEPROM(int addr, char data) {
    EEADRH = (addr >> 8) & 0x03;
    EEADR = (addr & 0x0ff);

    EEDATA = data; // Data Memory Value to write
    EECON1bits.EEPGD = 0; // Point to data memory
    EECON1bits.CFGS = 0; // Access program FLASH or Data EEPROM Memory
    EECON1bits.WREN = 1; // Enable writes

    //required sequence
    INTCONbits.GIE = 0; // Disable interrupts
    EECON2 = 0x55; // Write 55h
    EECON2 = 0xaa; // Write AAh
    EECON1bits.WR = 1; // Set WR bit to begin write
    INTCONbits.GIE = 1; // Enable Interrupts

    while (!PIR2bits.EEIF); // wait for interrupt to signal write complete
    PIR2bits.EEIF = 0; // clear EEPROM write operation interrupt flag
    EECON1bits.WREN = 0; // disable writes on write complete
}

///////////////////////////////////////////////////////////////////////////////
// writeStringEEPROM
//
// Write_String_EEPROM 
//

void writeStringEEPROM(int addr, char string[38]) {
    unsigned char ii, string_len;

    string_len = strlen(string);
    for (ii = 0; ii < string_len; ii++) {
        writeEEPROM(addr, *(string + ii));
        addr++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// readEEPROM
//
// Read_EEPROM 
//

char readEEPROM(int addr) {
    char data;

    EEADRH = (addr >> 8) & 0x03;
    EEADR = (addr & 0x0ff);

    EECON1bits.EEPGD = 0; // Point to data memory
    EECON1bits.CFGS = 0; // Access program FLASH or Data EEPROM Memory
    EECON1bits.RD = 1; // Enable read
    data = EEDATA;

    return data;
}

///////////////////////////////////////////////////////////////////////////////
// readStringEEPROM
//

void readStringEEPROM(int addr, char string[38], char final_character) {
    char char_read;
    char ii;

    char_read = 0;
    ii = 0;
    while (char_read != final_character) {
        char_read = readEEPROM(addr);
        *string = char_read;
        addr++;
        string++;
        ii++;
    }
}

//////////////////////////////////////////////////////////////////////////////////
//send an SMS
//

void sendSMS(unsigned char event_numbers, char message[]) {
    int addr;

    PORTBbits.RB6 = 1;

    for (i = 0; i < 8; i++) {
        if ((event_numbers & 1)) //pl.: 10001111, 1-4-ik, 8-ik számra megy üzenet
        {

            addr = i * PHONE_NO_LENGHT + PHONE_NO_1;
            readStringEEPROM(addr, data, 0xff);

            putrsUSART(SEND_CMD);
            putrsUSART(QUOTE);
            putsUSART(data);
            putrsUSART(QUOTE);
            putrsUSART(CR);
            delay = 100;
            while (delay);

            putsUSART(message);
            putrsUSART(CTRL_Z);

            delay = 255;
            while (delay);

        }

        event_numbers >>= 1;

        PORTBbits.RB6 = 0;
    }
}

//*************************************************************

void GSM_PwrOn(void) {
    if (!VDDEXT) {
        delay = 500;
        while (!VDDEXT) {
            PWRKEY = 0;
            if (!delay) break;
        }
        PWRKEY = 1;
    }
}
