#include "stdafx.h"
#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <ostream.h>
#include <string.h>
#include <time.h>

#define ITERATION 100
#define DATA_SIZE 0x00100000

int DataTransferTypical(int* piDst, int* piSrc, unsigned long SizeInBytes);
int DataTransferOptimised(int* piDst, int* piSrc, unsigned long SizeInBytes);

int main(int argc, char* argv[])
{
	unsigned long dwTimeStart = 0;
	unsigned long dwTimeEnd = 0;

	// temporary variable
	int *piSrc = NULL;
	int *piDst = NULL;

	int i = 0;
	char cKey = 0;

	unsigned long dwDataSizeInBytes = sizeof(int) * DATA_SIZE;

	piSrc = (int *)_aligned_malloc(dwDataSizeInBytes,dwDataSizeInBytes);
	piDst = (int *)_aligned_malloc(dwDataSizeInBytes,dwDataSizeInBytes);

	do
	{
		// initialise
		memset(piSrc, 1, dwDataSizeInBytes);
		memset(piDst, 0, dwDataSizeInBytes);

		dwTimeStart = clock();
		for(i = 0; i < ITERATION; i++)
			DataTransferTypical(piDst, piSrc, dwDataSizeInBytes);

		dwTimeEnd = clock();
		printf("== Typical Transfer of %d * %d times of %d bytes data ==\nTime Elapsed = %d msec\n\n", ITERATION, DATA_SIZE, sizeof(int), dwTimeEnd - dwTimeStart);

		// initialise
		memset(piSrc, 1, dwDataSizeInBytes);
		memset(piDst, 0, dwDataSizeInBytes);

		dwTimeStart = clock();
		for(i = 0; i < ITERATION; i++)
			DataTransferOptimised(piDst, piSrc, dwDataSizeInBytes);

		dwTimeEnd = clock();
		printf("== Optimised Transfer of %d * %d times of %d bytes data ==\nTime Elapsed = %d msec\n\n", ITERATION, DATA_SIZE, sizeof(int), dwTimeEnd - dwTimeStart);

		printf("Rerun? (y/n) ");
		cKey = getche();
		printf("\n\n");
	}while(cKey == 'y');

	_aligned_free(piSrc);
	_aligned_free(piDst);

	return 0;
}

#pragma warning(push)
#pragma warning(disable:4018 4102)

int DataTransferTypical(int* piDst, int* piSrc, unsigned long SizeInBytes )
{
	unsigned long dwNumElements = SizeInBytes / sizeof(int);

	for(int i = 0; i < dwNumElements; i++)
		*(piDst + i) = *(piSrc + i);

	return 0;
}

int DataTransferOptimised(int* piDst, int* piSrc, unsigned long SizeInBytes)
{
	unsigned long dwNumElements = SizeInBytes / sizeof(int);
	unsigned long dwNumPacks = dwNumElements / (128/(sizeof(int)*8));

	_asm
	{
		pusha;
begin:
		mov		ecx,SizeInBytes;
		mov		edi,piDst;
		mov		esi,piSrc;
begina:
		cmp		ecx,0;
		jz		end;
body:
		mov		ebx,SizeInBytes;
		sub		ebx,ecx;
		movdqa	xmm1,[esi+ebx];
		movdqa	[edi+ebx],xmm1;

bodya:
		sub		ecx,16;
		jmp		begina;
end:
		popa;
	}

	return 0;
}

#pragma warning(pop)
