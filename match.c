#include "match.h"
unsigned int gestureRight[MAX_RIGHTHAND*5]={
	0x31f,0x267,0x2b0,0x2f1,0x2dd, // number 1
	0x325, 0x25c, 0x238, 0x2d1, 0x2d1, 	//number 2
	0x339, 0x259, 0x227, 0x279, 0x2ab, 	//number 3
	0x341, 0x25a, 0x228, 0x267, 0x243, 	//number 4
	0x2cd, 0x257, 0x226, 0x26a, 0x245,	//number 5
	0x2d9, 0x313, 0x299, 0x2d6, 0x267, 	//number 6
	0x2bf, 0x2b2, 0x243, 0x29b, 0x293,	//number 7
	0x2ca, 0x271, 0x2bb, 0x308, 0x2fd,	//number 8
	0x353, 0x306, 0x2bf, 0x305, 0x2f0, 	//number 9
	0x2ff,0x32e,0x2a9,0x2fe,0x2f4		// diao/good/thumb up
	//0x2f3, 0x26e, 0x283, 0x2c7, 0x2ca,	//number 10
	//0x2de,0x306,0x292,0x2eb,0x27a // yang rou
};
//buffer[B]~buffer[F]

unsigned int gestureAcc[MAX_RIGHTHAND*MAX_ACC*3]={
	0, 0, 0, 					// default, 1 
	0x1ff, 0x1c7, 0x0cc,		// ni, you
	0, 0, 0,
	
	0, 0, 0,				//number 2
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,				//number 3
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,				//number 4
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,				//number 5
	0x1d4, 0x1b6, 0x0d2,	// bu, no
	0, 0, 0,

	0, 0, 0,				//number 6
	0x239, 0x134, 0x1fe,
	0, 0, 0,

	0, 0, 0,				//number 7
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,				//number 8
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,				//number 9
	0, 0, 0,
	0, 0, 0,

	0x23f,0x127,0x1de, 		// diao
	0x2c4,0x239,0x193,		// wo, me
	0,0,0
	
};
//buffer[8]+buffer[9]+buffer[A]

unsigned int gestureLeft[MAX_LEFTHAND*MAX_RIGHTHAND*MAX_ACC*5]={
	0,0,0,0,0,							// number 1
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,



	0, 0, 0, 0, 0,						// number 2
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 3
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 4
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 5
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 6
	0x25c,0x2fc,0x2c0,0x28a,0x165, 		// yang rou
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 7
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 8
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// number 9
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,						// diao
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

};
//buffer[0]~bufer[4]
/*
unsigned int gestureRight[MAX_RIGHTHAND*5]={
	0x31f,0x267,0x2b0,0x2f1,0x2dd, // 1
	0x2de,0x306,0x292,0x2eb,0x27a // yang rou

};
unsigned int gestureAcc[MAX_RIGHTHAND*MAX_ACC*3]={
	0x2dd,0x1ab,0x1a0, // default, 1 
	0, 0, 0,
	0, 0, 0,
	
	0x2f9,0x1ed,0x1bb, // default yangrou
	0,0,0,
	0,0,0


};
unsigned int gestureLeft[MAX_LEFTHAND*MAX_RIGHTHAND*MAX_ACC*5]={
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,

	0x25c,0x2fc,0x2c0,0x28a,0x165, // yang rou
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0

};
*/
int check_each_match(unsigned int* v1, unsigned int* v2,uchar size, int thresh)
{
	uchar i;
	for(i=0;i<size;i++)
	{
		if(abs(v1[i]-v2[i])>thresh)
			return 0;
	}
	return 1;
}

unsigned int calc_distance(unsigned int* v1, unsigned int* v2,uchar size)
{
	uchar i; 
	unsigned int dist=0;
	for(i=0;i<size;i++)
	{
		dist+=abs(v1[i]-v2[i]);
	}
	return dist;
}

// check if the gesture in the array is valid gesture
// if the first value of the gesture is 0, return false
int check_gesture(unsigned int* g)
{
	if(g[0]==0)
		return 0;
	else
		return 1;
}

// Data structure: A tree
// right	   |  0  |  1  |          *5 data
// acc 	 | 0 | 1 | 2 | 0 | 1 | 2 |	  *3 data	
// left  |0|1|0|1|0|1|0|1|0|1|0|1|    *5 data
// the returning result will be the number of the leaves in this tree.
// when not matched, return -1
int match()
{







	
	uchar r_idx,l_idx,a_idx; // right hand, left hand, acc idx
	int r_flag,l_flag,a_flag;
	uchar r_min_idx=0,l_min_idx=0,a_min_idx=0;
	//unsigned int r_dist=0,r_min_dist=-1;	
	//unsigned int l_dist=0,l_min_dist=-1;
	//unsigned int a_dist=0,a_min_dist=-1;

	// match right fingers
	for(r_idx=0;r_idx<MAX_RIGHTHAND;r_idx++)
	{
		// check if valid gesture
		if(!check_gesture(&gestureRight[r_idx*5]))
			continue;
		// each right hand gesture has a stride of 5 in the gestureRight array
		r_flag=check_each_match(&gestureRight[r_idx*5],&finger[5],5,RIGHT_THRESHOLD);
		if(r_flag){
			r_min_idx=r_idx;
			break;
		}
	}

	if(r_flag)
	{

		// match acc
		for(a_idx=0;a_idx<MAX_ACC;a_idx++)
		{
			if(!check_gesture(&gestureAcc[(r_min_idx*MAX_ACC+a_idx)*3]))
				continue;
			a_flag=check_each_match(&gestureAcc[(r_min_idx*MAX_ACC+a_idx)*3],&acc[3],3,ACC_THRESHOLD);
			if(a_flag)
			{
				a_min_idx=a_idx;
				break;
			}
			
		}


		if(a_flag)	
		{
			// match left 
			for(l_idx=0;l_idx<MAX_LEFTHAND;l_idx++)
			{
				if(!check_gesture(&gestureLeft[(r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx*MAX_LEFTHAND+l_idx)*5]))
					continue;
				l_flag=check_each_match(&gestureLeft[(r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx*MAX_LEFTHAND+l_idx)*5],&finger[0],5,LEFT_THRESHOLD);
				if(l_flag)
				{
					l_min_idx=l_idx;
					break;
				}

			}
			
			if(!(l_flag))// default value when left hand not match 
				l_min_idx=0;

			return r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx*MAX_LEFTHAND+l_min_idx;
				
			


			
		}
		else 
		{
			// default value when acc not matched
			a_min_idx=0;
			return r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx;
		}




	}
	else 
	{
		// not matched when right hand not matched
		return -1;
	}

}


