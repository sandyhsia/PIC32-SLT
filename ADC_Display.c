/************************************************************************
* LCD.c
************************************************************************/
#include "ADC_Display.h"
//#include <plib.h>
/* main driver */
static int count; // variable for timer2 to get out from the while loop

/* initialize the PIC32 MCU */
void MCU_init() {
/* setup I/O ports to connect to the LCD module */
	TRISD=0;
	TRISE=0;
	RS=0;
	
/* setup Timer to count for 1 us and ms */

	T3CON=0;
	T2CON=0;//stop any operation of timer 2
	T2CON=0x8;//32bit, prescale by 1
	TMR2=0;
	TMR3=0;


/* Configure Timer interrupts */
/* enable global and individual interrupts */
//Interrupt
	INTCONbits.MVEC=1;
	asm("ei");
	IPC3SET=0x0000001C;//interrupt priority level 7, subpriority level 0
	IFS0CLR=0x00001000;//clear timer interrupt flag
	IEC0SET=0x00001000;//enable timer3 interrupt

}
/* initialize the LCD module */
void LCD_init() {
DelayMsec(15); //wait for 15 ms
RS = 0; //send command
LCD_putchar(LCD_IDLE); //function set - 8 bit interface
DelayMsec(5); //wait for 5 ms
LCD_putchar(LCD_IDLE); //function set - 8 bit interface
DelayUsec(100); //wait for 100 us
LCD_putchar(LCD_IDLE); //function set
DelayMsec(5);
LCD_putchar(LCD_IDLE);
DelayUsec(100);
LCD_putchar(LCD_2_LINE_4_BITS); // 2 lines, 4 bit interface
DelayUsec(40);
LCD_putchar(LCD_DSP_OFF);// turn off the display
DelayUsec(40);
LCD_putchar(LCD_CLR_DSP); // clear the display
DelayMsec(5);
LCD_putchar (LCD_CSR_INC);// cursor increment
DelayUsec(40);
LCD_putchar(LCD_DSP_CSR); // turn on the display
DelayUsec(40);
//LCD_putchar(LCD_SFT_MOV); // cursor move right
//DelayUsec(40);
//LCD_putchar(LCD_SFT_MOV); // cursor move right
//DelayUsec(40);
// Everytime you use LCD_SFT_MOV, you can see a blank block on the screen
// because there is one cursor move right without putting any char on that block
}
/* Send one byte c (instruction or data) to the LCD */
void LCD_putchar(uchar c) {
E = 1;
Data = c; //sending higher nibble
E = 0; //producing falling edge on E
//DelayUsec(40);
E = 1;
Data <<= 4; //sending lower nibble through higher 4 ports
E = 0; //producing falling edge on E
DelayUsec(40);
}
/* Display a string of characters *s by continuously calling LCD_putchar() */
void LCD_puts(const uchar *s) {

//NOTE: must wait for at least 40 us after sending each character to
//the LCD module.

	LCD_entry_right();
	
	RS=1;// read/write mode
	while(*s)
	{
		LCD_putchar(*s);
		s++;
		
	}
	RS=0;// instruction mode
}
/* go to a specific DDRAM address addr */
void LCD_goto(uchar addr) {
//send an address to the LCD
//NOTE: must wait for at least 40 us after the address is sent

	E=1;

	Data=addr|0x80;//set DB7=1;
	RS=0;
	
	E=0;
	E=1;
	Data <<= 4;
	E = 0;
	DelayUsec(40);
}
/* configure timer SFRs to generate num us delay*/
void DelayUsec(uchar num) {
	unsigned int udelay = num*USEC_DELAY;
	count = 1; // which would be cleared to 0 in ISR
	PR2=udelay-1;
	T2CONSET=0x8000;
	while(1)
		{
			if(!count)break;
		};
	T2CON=0x8;//32bit, prescale by 1
	TMR2=0;
}
/* configure timer SFRs to generate 1 ms delay*/
void GenMsec() {

	PR2=MSEC_DELAY-1;
	count = 1;  // which would be cleared to 0 in ISR
	T2CONSET=0x8000;
	while(1)
		{
			if(!count)break;
		};
	T2CON=0x8;//32bit, prescale by 1
	TMR2=0;

}
/* Call GenMsec() num times to generate num ms delay*/
void DelayMsec(uchar num) {
uchar i;
for (i=0; i<num; i++) {
GenMsec();
}
}

void LCD_entry_right(void)
{
	LCD_putchar(LCD_CSR_INC);//LCD_CSR_DCR 0x04
	//Interesting thing is that when you use 0x04 plus the cursor move right ONCE, 
	//you can see the string "Doiii" showed on screen as "oD_____" the place after D is all blank.  ^_^
	DelayUsec(40);
}



// Use polling instead of interrupt
/* timer interrupt handler */
#pragma interrupt ISR_T3 ipl7 vector 12

void ISR_T3(void)
{
	T2CON=0;//stop any operation of timer 2
	T2CONSET=0x8;//32bit, prescale by 1
	TMR2=0;
	count=0; // clear count bit
	IFS0CLR=0x00001000;//clear timer interrupt flag
}

void LCD_CLR_SCN(void)
{
	LCD_putchar(LCD_CLR_DSP);
}
/*************end of LCD.c**************/