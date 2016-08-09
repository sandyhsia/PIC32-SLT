/************************************************************************
* LCD.h
* Header file for the LCD Driver
************************************************************************/
#ifndef LCD_H_
#define LCD_H_
#include <p32xxxx.h>

/* define macros for LCD instructions*/
#define LCD_IDLE 0x33
#define LCD_2_LINE_4_BITS 0x28
#define LCD_2_LINE_8_BITS 0x38
#define LCD_DSP_OFF 0x08 //display off
#define LCD_DSP_CSR 0x0c  // display om
#define LCD_CLR_DSP 0x01 // clear display
#define LCD_CSR_INC 0x06 // cursor increment
#define LCD_SFT_MOV 0x14 // cursor move right
// sysclk=pbclk=4MHz, prescale=1
#define MSEC_DELAY 4000
#define USEC_DELAY 4
/* define macros for interfacing ports*/
// changed a bit to accomodate LED
#define RS PORTDbits.RD0
//#define RW PORTEbits.RE1
//RW is grounded
// changed a bit to accomodate LED
#define E PORTDbits.RD1

#define Data PORTE // we only use the lowest 8 bits
typedef unsigned char uchar;
/*struct bits {
unsigned timer2_done : 1;
...other user defined flags may go here
} flags;*/
/* define constant strings for display */
//const uchar startStr1[] = "Voltage is";
//Sandy: till now I still don't know how to shift the cursor to the end of the string when the string is too long
//const uchar startStr2[] = "SJTU JI - LAB3";
//const uchar teststr[]="t";
/* Function prototypes */
void MCU_init(void);
void LCD_init(void);
void LCD_putchar(uchar c);
void LCD_puts(const uchar *s);
void LCD_goto(uchar addr);
void GenMsec(void);
void DelayUsec(uchar num);
void DelayMsec(uchar num);
void LCD_entry_right(void);
void LCD_CLR_SCN(void);

/*****************end of LCD.h**********************/
#endif