//#define ORFI
//#define SZHELY
#define NEURO
//---------------------------------------------------------------------

#if defined (ORFI)

#if defined (SZHELY) || defined (NEURO)
#error "Több site van kiválasztva!"
#endif
#warning " ORFI"
const rom char DI2_HI[] = "\rAT+CMGS=\"+36305601204\"\rORFI MR: a kompresszor elindult. \x1A";
const rom char DI2_LOW[] = "\rAT+CMGS=\"+36305601204\"\rORFI MR: a kompresszor leallt. \x1A";
//#define MULTINO
//const rom char DI2_HI1[]="\rAT+CMGS=\"+3630359055\"\rORFI MR: a kompresszor elindult. \x1A";
//const rom char DI2_LOW1[]="\rAT+CMGS=\"+3630359055\"\rORFI MR: a kompresszor leallt. \x1A";
const rom char DI2_HI_query[] = "\rAT+CMGS=\"+36305601204\"\rORFI MR tavfelugyelet ok, a kompresszor uzemel. \x1A";
const rom char DI2_LOW_query[] = "\rAT+CMGS=\"+36305601204\"\rORFI MR tavfelugyelet ok, a kompresszor NEM uzemel. \x1A";
const rom char QUERY_NO[] = "\"+36305601204\"";
const rom char QUERY_MSG[] = " Asdf";

#elif defined (SZHELY)

#if defined (ORFI) || defined (NEURO)
#error "Több site van kiválasztva!"
#endif
#warning "Szombathely"
const rom char DI2_HI[] = "\rAT+CMGS=\"+36305601204\"\rSzombathely MR: a kompresszor elindult. \x1A";
const rom char DI2_LOW[] = "\rAT+CMGS=\"+36305601204\"\rSzombathely MR: a kompresszor leallt. \x1A";
#define MULTINO
const rom char DI2_HI1[] = "\rAT+CMGS=\"+36303590553\"\rSzombathely MR: a kompresszor elindult. \x1A";
const rom char DI2_LOW1[] = "\rAT+CMGS=\"+36303590553\"\rSzombathely MR: a kompresszor leallt. \x1A";
const rom char DI2_HI_query[] = "\rAT+CMGS=\"+36305601204\"\rSzombathely MR tavfelugyelet ok, a kompresszor uzemel. \x1A";
const rom char DI2_LOW_query[] = "\rAT+CMGS=\"+36305601204\"\rSzombathely MR tavfelugyelet ok, a kompresszor NEM uzemel. \x1A";
const rom char QUERY_NO[] = "\"+36305601204\"";
const rom char QUERY_MSG[] = " Asdf";


#elif defined (NEURO)

#if defined (SZHELY) || defined (ORFI)
#error "Több site van kiválasztva!"
#endif
#warning "SOTE Neuro"

const rom char DI2_HI[] = "\rAT+CMGS=\"+36305601204\"\rSE Neuro MR: a kompresszor elindult. \x1A";
const rom char DI2_LOW[] = "\rAT+CMGS=\"+36305601204\"\rSE Neuro MR: a kompresszor leallt. \x1A";
//#define MULTINO
//const rom char DI2_HI1[]="\rAT+CMGS=\"+3630359055\"\rSE Neuro MR: a kompresszor elindult. \x1A";
//const rom char DI2_LOW1[]="\rAT+CMGS=\"+3630359055\"\rSE Neuro MR: a kompresszor leallt. \x1A";
const rom char DI2_HI_query[] = "\rAT+CMGS=\"+36305601204\"\rSE Neuro MR tavfelugyelet ok, a kompresszor uzemel. \x1A";
const rom char DI2_LOW_query[] = "\rAT+CMGS=\"+36305601204\"\rSE Neuro MR tavfelugyelet ok, a kompresszor NEM uzemel. \x1A";
const rom char QUERY_NO[] = "\"+36305601204\"";
const rom char QUERY_MSG[] = " Asdf";

#else
#error "Válaszd ki a site-ot!"
#endif

const rom char CMGF[] = "\rAT+CMGF=1\r";
const rom char IPR[] = "\rAT\rAT\rAT\rAT\rAT+IPR=9600\r";
const rom char ATW[] = "\rAT&W0\r";
const rom char CMGD[] = "\rAT+CMGD=1\r";
const rom char CNMI[] = "\rAT+CNMI=2,2,0,0\r";

//---------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------

void servo_isr(void); // Does servo calculations
void isrhandler(void); // Located at high interrupt vector

void Setup(void); // Configures peripherals and variables
void GSM_PwrOn(void);

void putrsUSART(const rom char *data); // Writes a string from ROM to the USART
void putsUSART(char *data); // Writes a string from RAM to the USART

//---------------------------------------------------------------------
//Constant Definitions
//---------------------------------------------------------------------

#define VpsupI	PORTAbits.RA0
#define	VbatI	PORTAbits.RA1
#define	AI2	PORTAbits.RA2

#define PWRKEY	PORTAbits.RA4
#define AI3	PORTAbits.RA5

#define RI	PORTBbits.RB0
#define STAT2	PORTBbits.RB1
#define AI1	PORTBbits.RB2
#define DI1	PORTBbits.RB3
#define CHGEN	PORTBbits.RB4
#define VDDEXT	PORTBbits.RB5
#define DI4	PORTBbits.RB6

#define	DI2	PORTCbits.RC1
#define DI3	PORTCbits.RC2
#define timeout 20

#define EVENT_1_NUMBERS 0
#define EVENT_2_NUMBERS 1
#define EVENT_3_NUMBERS 2
#define EVENT_4_NUMBERS 3
#define EVENT_5_NUMBERS 4
#define EVENT_6_NUMBERS 5
#define EVENT_7_NUMBERS 6
#define EVENT_8_NUMBERS 7

#define PHONE_NO_LENGHT 14

#define PHONE_NO_1 8
#define PHONE_NO_2 22
#define PHONE_NO_3 36
#define PHONE_NO_4 50
#define PHONE_NO_5 64
#define PHONE_NO_6 78
#define PHONE_NO_7 92
#define PHONE_NO_8 106

#define DI_1_WENT_LOW 0x0001
#define DI_1_WENT_HI 0x0002
#define DI_1_QUERY 0x0004

#define DI_2_WENT_LOW 0x0008
#define DI_2_WENT_HI 0x0010
#define DI_2_QUERY 0x0020

#define DI_3_WENT_LOW 0x0040
#define DI_3_WENT_HI 0x0080
#define DI_3_QUERY 0x0100

#define DI_4_WENT_LOW 0x0200
#define DI_4_WENT_HI 0x0400
#define DI_4_QUERY 0x0800

#define AI_1_HI_THRESHOLD 13
#define AI_1_LOW_THRESHOLD 14
#define AI_1_QUERY 15

#define AI_2_HI_THRESHOLD 16
#define AI_2_LOW_THRESHOLD 17
#define AI_2_QUERY 18

#define AI_3_HI_THRESHOLD 19
#define AI_3_LOW_THRESHOLD 20
#define AI_3_QUERY 21

#define DI_1_WENT_LOW_MESSAGE 120
#define DI_1_WENT_HI_MESSAGE 158
#define DI_1_QUERY_LOW_ANSWER 196
#define DI_1_QUERY_HI_ANSWER 234

#define DI_2_WENT_LOW_MESSAGE 272
#define DI_2_WENT_HI_MESSAGE 310
#define DI_2_QUERY_LOW_ANSWER 348
#define DI_2_QUERY_HI_ANSWER 386

#define DI_3_WENT_LOW_MESSAGE 424
#define DI_3_WENT_HI_MESSAGE 462
#define DI_3_QUERY_LOW_ANSWER 500
#define DI_3_QUERY_HI_ANSWER 538

#define DI_4_WENT_LOW_MESSAGE 576
#define DI_4_WENT_HI_MESSAGE 614
#define DI_4_QUERY_LOW_ANSWER 652
#define DI_4_QUERY_HI_ANSWER 690

#define AI_1_HI_THRESHOLD_MESSAGE 690
#define AI_1_LOW_THRESHOLD_MESSAGE 728
#define AI_1_QUERY_ANSWER 766

#define AI_2_HI_THRESHOLD_MESSAGE 800
#define AI_2_LOW_THRESHOLD_MESSAGE 838
#define AI_2_QUERY_ANSWER 876

#define AI_3_HI_THRESHOLD_MESSAGE 910
#define AI_3_LOW_THRESHOLD_MESSAGE 948
#define AI_3_QUERY_ANSWER 986

#define DEBOUNCE_DELAY 255


