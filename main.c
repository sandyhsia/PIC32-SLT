//#include <plib.h>
#include "ADC_Display.h"
#include "match.h"
#include "sample.h"
#include <stdio.h>
#include <stdlib.h>

/* I/O Assignment:
 * RD0 & RD1 occupied by LCD
 *
 * Press SW3 (RD13 CN19) to change language
 * Acknoledged by LED3 (RD2)
 * if Engilsh, LED3 OFF, RC1 = 0;
 * if CHinese, LED3 ON, RC1 = 1;
*/

// PBCLK=4MHz
#pragma config FPBDIV = DIV_1
// 1:256 timer prescale
// note: max 65536 cycles
#define CHECK_INTERVAL 0x1E83 // 7811 OCT for 500ms
#define TIME_OUT 0x7A0F // 31247 OCT for 2s

// Function Header
void TMR4_init(void);
void TMR5_init(void);
unsigned char* int_to_str(int);
void initUART(void);
void initChangeNotice(void);

// Global Variables
int prevIndex = -1;
int currentIndex = -2;
char recognizedString[5];
int readCN19 = 0;
int languageMode = 0; // 0 English, 1 Chinese

int main()
{	
	int ADCValue = 0;
	float RealVolt;
	uchar* DisplayVoltChar;//avoid memory leak
	//OSCSetPBDIV (OSC_PB_DIV_1); //configure PBDIV so PBCLK = SYSCLK
    // commented out for OS incompatibility
	MCU_init();
	LCD_init();
 	TMR4_init();
 	TMR5_init();
 	ADC_init();
	initUART();
	initChangeNotice();

	LCD_goto(0x00);
	LCD_puts("Gesture: ");




	while(1)
	{
		readCN19=PORTDbits.RD13;	// watch out for language switch
		get_sensors();
		
		// testing code. No longer needed.
		// LCD_goto(0x00);
		// LCD_puts(number_to_char(Conv_RealVolt(finger[5])));
		// LCD_puts(number_to_char(Conv_RealVolt(finger[6])));
		// LCD_puts(number_to_char(Conv_RealVolt(finger[7])));
		// LCD_puts(number_to_char(Conv_RealVolt(finger[8])));
		// LCD_goto(0x40);
		// LCD_puts(number_to_char(Conv_RealVolt(finger[9])));
		// LCD_puts(number_to_char(Conv_RealVolt(acc[3])));
		// LCD_puts(number_to_char(Conv_RealVolt(acc[4])));
		// LCD_puts(number_to_char(Conv_RealVolt(acc[5])));

		// DelayMsec(1000); //wait for 1 s
	}

return 0;
}


void ADC_init()
{
	AD1PCFG = 0x0; 	// PORTB (RB2 particularly) = analog
	
	AD1CON1 = 0x0014; 	// SAMP bit = 0 ends sampling ...
						// and starts converting
						// 16bit integer
						// Autostart sampling when conversions finish
						// CLRASAM=1, automatically stop adc after 
						// conversion sequence finish
	AD1CON2 = 0x6400; // Vref+, Vref-, Scan mode
	AD1CON2bits.SMPI=SAMPLE_NUM-1; // SAMPLE_NUM sample sequences before interrupt
	AD1CON3 = 0x0002; 		// Manual Sample, Tad = internal 6 TPB = 6*0.25us = 1.5us
	AD1CSSL= 0xffff; // select AN0-AN15
	

	IPC6bits.AD1IP=5; // Set Priority to 5
	IPC6bits.AD1IS=3; // Set Sub Priority to 3
	IFS1CLR = 0x0002; // Ensure the interrupt flag is clear
	IEC1SET = 0x0002; // Enable ADC interrupts

	AD1CON1SET=0x8000; // turn on the ADC
}

// A conversion sequenquence is finished. Read the data out.
#pragma interrupt ADC_ISR ipl5 vector 27
void ADC_ISR()
{
	IFS1CLR = 0x0002; // Ensure the interrupt flag is clear
//left hand
	finger[0]=ADC1BUF0;
	finger[1]=ADC1BUF1;
	finger[2]=ADC1BUF2;
	finger[3]=ADC1BUF3;
	finger[4]=ADC1BUF4;


	acc[0]=ADC1BUF5;
	acc[1]=ADC1BUF6;
	acc[2]=ADC1BUF7;
//right hand
	acc[3]=ADC1BUF8;
	acc[4]=ADC1BUF9; 
	acc[5]=ADC1BUFA;

	finger[5]=ADC1BUFB;
	finger[6]=ADC1BUFC;
	finger[7]=ADC1BUFD;
	finger[8]=ADC1BUFE;
	finger[9]=ADC1BUFF;


}

void TMR4_init() {
    T4CON = 0x0;            // stop all 16/32-bit TIMER4 operation
    T4CONSET = 0x0070;      // enable 16-bit mode; prescaler 1:256; internal clock PBCLK
    PR4 = CHECK_INTERVAL;   // max 65536 cycles
    T4CONSET = 0x8000;      // start timer
	IFS0bits.T4IF = 0; 
	IEC0bits.T4IE = 1;
	IPC4bits.T4IP = 6;
	IPC4bits.T4IS = 0;
	
}

void TMR5_init() {
    T5CON = 0x0;            // stop all 16/32-bit TIMER5 operation
    T5CONSET = 0x0070;      // enable 16-bit mode; prescaler 1:256; internal clock PBCLK
    PR5 = TIME_OUT;			// max 65536 cycles
    T5CONSET = 0x8000;      // start timer
	IFS0bits.T5IF = 0;
}

// lower priority than ADC_ISR, 
// as we need latestst sensor readings inside T3_ISR
#pragma interrupt T4_ISR ipl6 vector 16
void T4_ISR (void) {
	T4CONCLR = 0x8000; //Stop 32-bit timer
	IEC0bits.T4IE = 0; // disable interrupt

	if(IFS0bits.T4IF) {
		prevIndex = currentIndex;
		currentIndex = match();

		if (currentIndex == -1) {
			// reconition failed
			LCD_goto(0x08);
			LCD_puts("Undefined");
			LCD_goto(0x40);
			LCD_puts("Try Again..");
		} else if (currentIndex == -2) {
			// reconition failed
			LCD_goto(0x08);
			LCD_puts("Default");
			LCD_goto(0x40);
			LCD_puts("Pending Signal");
		} else if (currentIndex >= 0) {
			// information gesture
			if (prevIndex != currentIndex) {
				LCD_goto(0x08);
				LCD_puts(int_to_str(currentIndex));
				LCD_goto(0x40);
				LCD_puts("Pending");

				U1ATXREG =  currentIndex;
				IEC0bits.U1ATXIE = 1;
				IFS0bits.T5IF = 0;		// reset timeout
			} else {
				if (IFS0bits.T5IF != 0) {	// if timeout
					LCD_goto(0x08);
					LCD_puts(int_to_str(currentIndex));
					LCD_goto(0x40);

					LCD_puts("Pending");
					U1ATXREG = currentIndex;
					IEC0bits.U1ATXIE = 1;
					TMR5 = 0;
					IFS0bits.T5IF = 0;		// reset timeout
				}
			}
		} else if (currentIndex < -1) {
			// command gesture
			LCD_goto(0x08);
			LCD_puts(int_to_str(currentIndex));
			LCD_goto(0x40);
			LCD_puts("Command Executed");
		}
	}

	IFS0bits.T4IF = 0; // clear flag
	IEC0bits.T4IE = 1; // enable interrupt
	T4CONSET = 0x8000; // Re-start 16-bit timer
}


void get_sensors()
{
	int i;
	AD1CON1bits.ASAM=1; // reset the bit that can be cleared by the software
						// auto start sampling after last conversion.
	AD1CON1SET = 0x0002;	// start sampling ...
	PR1 = 0XFFFF;
	T1CON=0;
	for(i=0;i<SAMPLE_NUM;i++)
	{
		TMR1 = 0; 
		T1CONSET = 0x8000; 
		asm("NOP");

		while(TMR1 < 800); 		// wait for 20Us => 500KHZ

		AD1CON1CLR = 0x0002; 	// start Converting
		T1CONCLR=0x8000; // Stop timer 1
	}
}




float Conv_RealVolt(int ValueIn16Bit)
{
	return ((float)ValueIn16Bit/1024*3.3); // our ValueIn16Bit = Volt_in / Vref+ * 1024, if we set Vref+ to b3 3.3
									// I think in this way we can get back the initial voltage input from AN2; 
}

unsigned char* number_to_char(float number)
{
	
	int prepareNum = number*1000; //Since we are required to have 2 digits behind the decimal point
								  //So I take intotal 4 digits and will do the approximation

	ResultString[0]= (prepareNum - prepareNum%1000)/1000+48; // ones digit
	prepareNum = prepareNum%1000;

	ResultString[1] = '.';

	ResultString[2] = (prepareNum - prepareNum%100)/100+48;	// decimal digit
	prepareNum = prepareNum%100;

	if(prepareNum%100 >= 5)
	{
		ResultString[3] = ((prepareNum - prepareNum%10)/10+1)%10+48;	// hundredth digit
	}	// avoid overflowing to 58(":"), noting 57("9")
	else
	{
		ResultString[3] = (prepareNum - prepareNum%10)/10+48;	// hundredth digit
		prepareNum = prepareNum%10;
	}
	ResultString[4]='\0';
	return ResultString; 
}

unsigned char* int_to_str(int value) {
	// supports -99 to 99 in value
	if (value > 0) {
		recognizedString[0] = '+';
		recognizedString[3] = value % 10+48;
		recognizedString[2] = (value % 100)/10+48;
		recognizedString[1] =  value/100 + 48;
	} else if (value == 0) {
		recognizedString[0] = '0';
		recognizedString[3] = '0';
		recognizedString[2] = '0';
		recognizedString[1] = '0';
	} else {
		value = -value;
		recognizedString[0] = '-';
		recognizedString[3] = value % 10+48;
		recognizedString[2] = (value % 100)/10+48;
		recognizedString[1] =  value/100 + 48;
	}
	recognizedString[4]='\0';
	return recognizedString; 
}

void initUART(void) {

	// start with transmission 
	
	//U1AMODE = 0x8008; //Enable UART for 8-bit data
	U1AMODEbits.ON = 1;			// turn it on	

	// Initialize UxBRG register for the appropriate baud rate 
	U1AMODEbits.BRGH = 1;		//enable high speed mode

	U1ABRG = 103;				//103 (dec)
	// 8 bit data bits,1 stop bits and parity 
	// enable transmission *
	U1AMODEbits.UEN = 0;			//UxTX and UxRX enabled and commanderred
	U1ASTASET = 0x5400; //interrupt when all character sent, TX_EN and RX_EN are set
			 // interrupt asserted when received buffer not empty
	 

	IPC6bits.U1AIP = 5;		// no idea why
	IPC6bits.U1AIS = 1;
	IFS0bits.U1ATXIF = 0;		// clear flag
	IFS0bits.U1ARXIF = 0;		
	IEC0bits.U1ATXIE = 1;		// then enable interrupt
	IEC0bits.U1ARXIE = 1;
	
}

/* Configure Change Notice */
void initChangeNotice(void) {
	/* Disable vector interrupts prior to configuration */
	asm("di"); 				// Disable all interrupts 

	/* Configure CN module */
	TRISDbits.TRISD13 = 1;	// ensure SW3 (RD13 CN19) set to input
	CNCONbits.ON = 1;		// enable CN module
	CNEN = 0x00080000;		// enable CN19
	CNPUE = 0x00080000;		// enable pullup for CN19

	/* Read ports to set a reference on change notice pins */
	readCN19 = PORTDbits.RD13;

	/* Configure CN interrupt */
	IPC6bits.CNIP = 1;		// priority level 1
	IPC6bits.CNIS = 1;		// subpriority level 1
	IFS1bits.CNIF = 0;		// clear flag
	IEC1bits.CNIE = 1;		// enable CN interrupt

	/* Re-enable all interrupt */
	asm("ei");

	/* set LED3 (RD2) and I/O RC1 to output */
	TRISCbits.TRISC1 = 0;
	TRISDbits.TRISD2 = 0;
	PORTCbits.RC1 = 0;		// default to English
	PORTDbits.RD2 = 0;
}


/* UART1 TX/RX ISR */
#pragma interrupt UART1_ISR ipl5 vector 24
void UART1_ISR (void) {
	IEC0bits.U1ATXIE = 0;		// then disable interrupt


	// UART1 Transmitter U1TX ISR
	if (IFS0bits.U1ATXIF == 1) {
		LCD_goto(0x40);
		LCD_puts("Message Sent");
		IFS0bits.U1ATXIF = 0;
	} 

	if (IFS0bits.U1ARXIF == 1) {
		

		receivedASCII = U1ARXREG;
		if(receivedASCII>0x1f&&receivedASCII<0x80)
		{
			ASCIIinStr[0] = receivedASCII;
		
			LCD_goto(0x0);
			LCD_puts("Receiving..");
			LCD_goto(0x40);
			LCD_puts(ASCIIinStr);
		}

	
	IFS0bits.U1ARXIF = 0;
	} 

	//IEC0bits.U1ATXIE = 1;
}

/* Change Notice ISR */
/* CN ISR - handling SW input signal */
#pragma interrupt CN_ISR ipl1 vector 26
void CN_ISR (void) {
	IEC1bits.CNIE = 0;		// disable CN interrupt

	if (readCN19 > PORTDbits.RD13) { // once every push
		if (languageMode == 0) {
			// set to Chinese
			languageMode = 1;
			// LED3(RD2) ON
			PORTDbits.RD2 = 1;
			// set I/O for Arduino communication
			PORTCbits.RC1 = 1;
			// print to LCD
			LCD_goto(0x0);
			LCD_puts("language Mode:");
			LCD_goto(0x40);
			LCD_puts("Chinese");
		} else {
			// set to English
			languageMode = 0;
			// LED3(RD2) ON
			PORTDbits.RD2 = 0;
			// set I/O for Arduino communication
			PORTCbits.RC1 = 0;
			// print to LCD
			LCD_goto(0x0);
			LCD_puts("language Mode:");
			LCD_goto(0x40);
			LCD_puts("English");
		}
	}

	readCN19 = PORTDbits.RD13;

	IFS1bits.CNIF = 0;		// clear flag
	IEC1bits.CNIE = 1;		// re-enable CN interrupt
}