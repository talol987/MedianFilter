/*
 * File: main.c
 * */

#include "medianFilter.h"
#include <stdio.h>

static FILE * fp;

static void main_medianFilter(void)
{
	int signal[BUF_SIZE], new_signal[BUF_SIZE];

	while(!feof(fp)) {
		for (int i = 0; i < BUF_SIZE; i++)
			fscanf(fp, "%d,", &signal[i]);

		medianFilter(signal, new_signal);

		for(int i = 0; i < BUF_SIZE; i++)
			printf("%d\n",new_signal[i]);
	}
}

int main(int argc, const char * const argv[])
{
	(void)argc;
	(void)argv;

	fp = fopen("Samples", "r");

	main_medianFilter();

	fclose(fp);
	
	return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
