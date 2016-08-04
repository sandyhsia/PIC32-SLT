#include "match.h"


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
char check_gesture(unsigned int* g)
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
	uchar r_min_idx=0,l_min_idx=0,a_min_idx=0;
	unsigned int r_dist=0,r_min_dist=-1;	
	unsigned int l_dist=0,l_min_dist=-1;
	unsigned int a_dist=0,a_min_dist=-1;

	// match right fingers
	for(r_idx=0;r_idx<MAX_RIGHTHAND;r_idx++)
	{
		// check if valid gesture
		if(!check_gesture(&gestureRight[r_idx*5]))
			continue;
		// each right hand gesture has a stride of 5 in the gestureRight array
		r_dist = calc_distance(&gestureRight[r_idx*5],&finger[5],5);
		if(r_dist<r_min_dist)
		{
			r_min_dist=r_dist;	
			r_min_idx=r_idx;
		}
	}

	if(r_min_dist<RIGHT_THRESHOLD)
	{

		// match acc
		for(a_idx=0;a_idx<MAX_ACC;a_idx++)
		{
			if(!check_gesture(&gestureAcc[(r_min_idx*MAX_ACC+a_idx)*3]))
				continue;
			a_dist = calc_distance(&gestureAcc[(r_min_idx*MAX_ACC+a_idx)*3],&acc[3],3);
			if(a_dist<a_min_dist)
			{
				a_min_dist=a_dist;
				a_min_idx=a_idx;
			}
		}
			
		if(a_min_idx<ACC_THRESHOLD)	
		{
			// match left 
			for(l_idx=0;l_idx<MAX_LEFTHAND;l_idx++)
			{
				if(!check_gesture(&gestureLeft[(r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx*MAX_LEFTHAND+l_idx)*5]))
					continue;
				l_dist=calc_distance(&gestureLeft[(r_min_idx*MAX_ACC*MAX_LEFTHAND+a_min_idx*MAX_LEFTHAND+l_idx)*5],&finger[0],5);
				if(l_dist<l_min_dist)
				{
					l_min_dist=l_dist;
					l_min_idx=l_idx;
				}

			}
			
			if(!(l_min_dist<LEFT_THRESHOLD))// default value when left hand not match 
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


