/*
 * File: medianFilter.c
 */

/* Include Files */
#include "medianFilter.h"
#include <string.h>

#define FMUL(a,b,q) (((a)*(b))>>(q))
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )
#define ABS(N) ((N<0)?(-N):(N))

#define ELEM_SWAP(a,b) { register int t=(a);(a)=(b);(b)=t; }

static int inline quick_select(int arr[])
{
	int low, high ;
	int median;
	int middle, ll, hh;

	low = 0 ; high = BUF_SIZE-1 ; median = (low + high) / 2;

	for (;;) {

		if (high <= low) /* One element only */
			return arr[median] ;
		if (high == low + 1) { /* Two elements only */
			if (arr[low] > arr[high])
				ELEM_SWAP(arr[low], arr[high]) ;
			return arr[median] ;
		}

		/* Find median of low, middle and high items; swap into position low */
		middle = (low + high) / 2;
		if (arr[middle] > arr[high]) ELEM_SWAP(arr[middle], arr[high]) ;
		if (arr[low] > arr[high]) ELEM_SWAP(arr[low], arr[high]) ;
		if (arr[middle] > arr[low]) ELEM_SWAP(arr[middle], arr[low]) ;

		/* Swap low item (now in position middle) into position (low+1) */
		ELEM_SWAP(arr[middle], arr[low+1]) ;

		/* Nibble from each end towards middle, swapping items when stuck */
		ll = low + 1;
		hh = high;
		for (;;) {
			do ll++; while (arr[low] > arr[ll]) ;
			do hh--; while (arr[hh] > arr[low]) ;
			if (hh < ll)
				break;
			ELEM_SWAP(arr[ll], arr[hh]) ;
		}

		/* Swap middle item (in position low) back into correct position */
		ELEM_SWAP(arr[low], arr[hh]) ;

		/* Re-set active partition */
		if (hh <= median)
			low = ll;
		if (hh >= median)
			high = hh - 1;
	}
}
#undef ELEM_SWAP

/* The main function "medianFilter",
 *
 * Arguments: int * const restrict x - the original signal
 *  	      int * const restrict x_new - the filtered siganl
 *
 * Return Type: void
 *
 * The function based on the equation:
 * Outlier = | x[n] - median(x[n]) | > 3 * Sigma
 *
 * MAD =  | x[n] - median( x[n] ) | 
 * y[n] = median( MAD )
 * sigma =  MAD * 1.48260224 
 * y[n] > 3 * sigma ==> Outlier
 */

void medianFilter(int const * const restrict x, int * const restrict x_new)
{
	int mad[BUF_SIZE],mad_copy[BUF_SIZE],x_copy[BUF_SIZE],median1,median2;
	unsigned char b0;
	unsigned char idx[BUF_SIZE];

	memcpy(&x_copy,x,sizeof(int) << 6);

	median1 = quick_select(x_copy);

	unsigned int i;
	for (i = 0; i < BUF_SIZE; ++i) {
		mad[i] = ABS((x[i] - median1));
		mad_copy[i] = mad[i];
	}

	median2 = quick_select(mad_copy);

	float thresh = TOFLT(FMUL(TOFIX(MAD_SCALE,7),			//Fixed-Point multiplication -> MAD_SCALE * FACTOR     * median2
		       FMUL(TOFIX(FACTOR,7),TOFIX(median2,7),7),7),7);	//                              3.0 	  * 1.48260224 * Median_Absolute_Deviation
		 

	/*  Identify points that bigger then 3 sigmas */
	unsigned char iOutlier = 0;
	for (i = 0; i < BUF_SIZE; ++i) {
		x_new[i] = x[i];
		b0 = (mad[i] > thresh);
		idx[i] = b0;
		if (b0) {
			++iOutlier;
		}
	}
	
	/* Replace only outliers(spikes) that less than 10 samples width */
	if (iOutlier > MIN_OUTLIER_LEN && iOutlier < MAX_OUTLIER_LEN) { 
		for (i = 0; i < BUF_SIZE; ++i) {
			if (idx[i]) {
				x_new[i] = median1;
			}
		}
	}
}

/*
 * File trailer for medianFilter.c
 */
