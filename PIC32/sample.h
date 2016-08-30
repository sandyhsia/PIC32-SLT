#ifndef SAMPLE_H_
#define SAMPLE_H_
#define SAMPLE_NUM 16

#include "ADC_Display.h"

float Conv_RealVolt(int);
uchar* number_to_char(float);
void ADC_init();
void get_sensors();
void initUART();
char ResultString[5]; //converted string's head
unsigned int finger[10];
unsigned int acc[6];


int receivedASCII;
char ASCIIinStr[5];
#endif