#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sys/time.h"
#include <omp.h>

int main(){
	#pragma omp parallel
	{
		#pragma omp single
		printf("hi\n");
		#pragma omp parallel for
		for(int i = 0; i < 3; i++)
		{
			printf("hello\n");
		}
		#pragma omp single
		printf("hi\n");
	}
}