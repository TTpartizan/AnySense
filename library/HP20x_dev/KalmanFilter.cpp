/*
 * File name  : KalmanFilter.cpp
 * Description: Kalman Filter class 
 * Author     : Oliver Wang from Seeed studio
 * Version    : V0.1
 * Create Time: 2014/04
 * Change Log :
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <Arduino.h>
#include <KalmanFilter.h>
//#include <inttypes.h>
//#include <time.h>
//#include <stdlib.h>
//#include <stdio.h>

/* random number table */
float Rand_Table[100]={
0.5377,1.8339,-2.2588,0.8622,0.3188,-1.3077,-0.4336,0.342,3.5784, 
2.7694,-1.3499,3.0349,0.7254,-0.0631,0.7147,-0.2050,-0.1241,1.4897, 
1.4090,1.4172,0.6715,-1.2075,0.7172,1.6302,0.4889,1.0347,0.7269, 
-0.3034,0.2939,-0.7873,0.8884,-1.1471,-1.0689,-0.8095,-2.9443,1.4384, 
0.3252,-0.7549,1.3703,-1.7115,-0.1022,-0.2414,0.3192,0.3129,-0.8649, 
-0.0301,-0.1649,0.6277,1.0933,1.1093,-0.8637,0.0774,-1.2141,-1.1135, 
-0.0068,1.5326,-0.7697,0.3714,-0.2256,1.1174,-1.0891,0.0326,0.5525, 
1.1006,1.5442,0.0859,-1.4916,-0.7423,-1.0616,2.3505,-0.6156,0.7481, 
-0.1924,0.8886,-0.7648,-1.4023,-1.4224,0.4882,-0.1774,-0.1961,1.4193, 
0.2916,0.1978,1.5877,-0.8045,0.6966,0.8351,-0.2437,0.2157,-1.1658, 
-1.1480,0.1049,0.7223,2.5855,-0.6669,0.1873,-0.0825,-1.9330,-0.439, 
-1.7947};

/* Extern variables */
KalmanFilter kalmanFilter;

KalmanFilter::KalmanFilter()
{
    X_pre = 0;
	P_pre = 0;	 
	X_post = 0;
	P_post = 0;
	K_cur = 0;
}

float KalmanFilter::Gaussian_Noise_Cov(void)
{
    int index = 0;
	float tmp[10]={0.0};
	float average = 0.0;
	float sum = 0.0;
	/* Initialize random number generator */
	srand((int)analogRead(0));
	
	/* Get random number */
	for(int i=0; i<10; i++)
	{
	    index = (int)rand()%100;
        tmp[i] = Rand_Table[index];
        sum += tmp[i];		
	}
	
	/* Calculate average */
	average = sum/10;
	
	/* Calculate Variance */
	float Variance = 0.0;		
	for(int j = 0; j < 10; j++)
	{
	    Variance += (tmp[j]-average)*(tmp[j]-average);
	}
	Variance/=10.0;
	
	return Variance;
}

float KalmanFilter::Filter(float origin)
{
    float modelNoise = 0.0;
	float observeNoise = 0.0;
	
	/* Get model and observe Noise */
	modelNoise = Gaussian_Noise_Cov();
	observeNoise = Gaussian_Noise_Cov();
	
	/* Algorithm */
	X_pre = X_post;
	P_pre = P_post + modelNoise;
	K_cur = P_pre/(P_pre + observeNoise);
	P_post = (1 - K_cur)*P_pre;
	X_post = X_pre + K_cur*(origin - X_pre);
	
	return X_post;
}