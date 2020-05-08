/*
 * File: medianFilter.h
 */

#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#define BUF_SIZE 64U
#define MAD_SCALE 3.0F
#define MIN_OUTLIER_LEN 0U
#define MAX_OUTLIER_LEN 10U
#define FACTOR 1.48260224F

/* Function Declarations */
extern void medianFilter(int const * const restrict x, int * const restrict x_new);

#endif

/*
 * File trailer for medianFilter.h
 *
 * [EOF]
 */
