#include <plib.h>
#include "ADC_Display.h"
#include "match.h"
#include "sample.h"
#include <stdio.h>
#include <stdlib.h>
// PBCLK=4MHz


int main()
{	
	int ADCValue = 0;
	float RealVolt;
	uchar* DisplayVoltChar;//avoid memory leak
	OSCSetPBDIV (OSC_PB_DIV_1); //configure PBDIV so PBCLK = SYSCLK
	MCU_init();
	LCD_init();

	//initUART();

	LCD_goto(0x00);
	LCD_puts("Volt is");

 	ADC_init();
	while(1)
	{
			get_sensors();

	
		LCD_goto(0x00);
		LCD_puts(number_to_char(Conv_RealVolt(finger[5])));
		LCD_puts(number_to_char(Conv_RealVolt(finger[6])));
		LCD_puts(number_to_char(Conv_RealVolt(finger[7])));
		LCD_puts(number_to_char(Conv_RealVolt(finger[8])));
		LCD_goto(0x40);
		LCD_puts(number_to_char(Conv_RealVolt(finger[9])));
		LCD_puts(number_to_char(Conv_RealVolt(acc[3])));
		LCD_puts(number_to_char(Conv_RealVolt(acc[4])));
		LCD_puts(number_to_char(Conv_RealVolt(acc[5])));

		DelayMsec(1000); //wait for 1 s
	

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

void initUART(void) {

	// start with transmission 
	
	//U1AMODE = 0x8008; //Enable UART for 8-bit data
	U1AMODEbits.ON = 1;			// turn it on	

	// Initialize UxBRG register for the appropriate baud rate 
	U1AMODEbits.BRGH = 1;		//enable high speed mode

	U1ABRG = 0x0340;				//832 (dec)
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


/* UART1 TX/RX ISR */
#pragma interrupt UART1_ISR ipl5 vector 24
void UART1_ISR (void) {
	IEC0bits.U1ATXIE = 0;		// then disable interrupt
	// reenable in CN

	// UART1 Transmitter U1TX ISR
	if (IFS0bits.U1ATXIF == 1) {


	//	LCD_goto(0x40);
	//	LCD_puts("Complete");
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

	int i;
	for(i=0;i<SAMPLE_NUM;i++)
	{
		U1ATXREG = finger[i];
	}
	
	IFS0bits.U1ARXIF = 0;

	} 
}