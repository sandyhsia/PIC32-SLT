#ifndef MATCH_H_
#define MATCH_H_
#include "ADC_Display.h"
#include "sample.h"
#include <stdio.h>
#define MAX_RIGHTHAND 12
#define MAX_ACC  3
#define MAX_LEFTHAND 2
#define RIGHT_THRESHOLD 50
#define LEFT_THRESHOLD 50
#define ACC_THRESHOLD 70
//unsigned int gestureRight[MAX_RIGHTHAND*5];
/*unsigned int gestureAcc[MAX_RIGHTHAND*MAX_ACC*3];
unsigned int gestureLeft[MAX_LEFTHAND*MAX_RIGHTHAND*MAX_ACC*5];*/

// Data structure: A tree
// right	   |  0  |  1  |          *5 data
// acc 	 | 0 | 1 | 2 | 0 | 1 | 2 |	  *3 data	
// left  |0|1|0|1|0|1|0|1|0|1|0|1|    *5 data
//void init_gesture_array();
int check_each_match(unsigned int* v1, unsigned int* v2,uchar size, int thresh);
unsigned int calc_distance(unsigned int* v1, unsigned int * v2,uchar size);
int check_gesture(unsigned int* g);
int match();
#endif