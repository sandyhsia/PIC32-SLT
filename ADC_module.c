#include <plib.h>
#include "ADC_Display.h"
#include <stdio.h>
#include <stdlib.h>
// PBCLK=4MHz

#define SAMPLE_NUM 6


float Conv_RealVolt(int);
uchar* number_to_char(float);
void ADC_init();
void get_sensors();

char ResultString[5]; //converted string's head
int finger[SAMPLE_NUM];


int main()
{	
	int ADCValue = 0;
	float RealVolt;
	uchar* DisplayVoltChar;//avoid memory leak
	OSCSetPBDIV (OSC_PB_DIV_1); //configure PBDIV so PBCLK = SYSCLK
	MCU_init();
	LCD_init();
	LCD_goto(0x00);
	LCD_puts("Volt is");

 	ADC_init();
	while(1)
	{
			get_sensors();
	int i;
	for( i=0;i<SAMPLE_NUM;i++)
	{
		LCD_goto(0x40);
		LCD_puts(number_to_char(Conv_RealVolt(finger[i])));
		//DelayMsec(1000); //wait for 2 s
	}

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
	AD1CON2bits.SMPI=SAMPLE_NUM; // 5 sample sequences before interrupt
	AD1CON3 = 0x0002; 		// Manual Sample, Tad = internal 6 TPB = 6*0.25us = 1.5us
	AD1CSSL= 0x001f; // select AN0-AN4, currently 5 values
	

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
	finger[0]=ADC1BUF0;
	finger[1]=ADC1BUF1;
	finger[2]=ADC1BUF2;
	finger[3]=ADC1BUF3;
	finger[4]=ADC1BUF4;
/*	finger[5]=ADC1BUF5;
	finger[6]=ADC1BUF6;
	finger[7]=ADC1BUF7;
	finger[8]=ADC1BUF8;
	finger[9]=ADC1BUF9;*/


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