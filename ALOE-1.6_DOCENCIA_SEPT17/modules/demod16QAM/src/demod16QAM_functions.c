/* 
 * Copyright (c) 2012
 * This file is part of ALOE (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Functions that generate the test data fed into the DSP modules being developed */
#include "demod16QAM_functions.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>





/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */

/**
 * @ingroup Hard 16QAM demodulator
 *
 * LTE-16QAM constellation:
 *               Q
 *  1011    1001  |  0001    0011
 *  1010    1000  |  0000    0010
 *---------------------------------> I
 *  1110    1100  |  0100    0110
 *  1111    1101  |  0101    0111
 *
 * \param in input symbols (_Complex float)
 * \param out output symbols (chars)
 * \param N Number of input symbols
 * \param modulation Modulation type
 */
void hard_qam16_demod(_Complex float *in, char *out, int N)
{
	int s, j=0;
	char byte;

	for (s=0; s<N/2; s++) {
		byte=0x00;
		if ((float)(__real__ in[j]) > 0.0) byte = byte | 0x0;
		else byte = byte | 0x8;
		if ((__real__ in[j] > QAM16_THRESHOLD) || (__real__ in[j] < -QAM16_THRESHOLD))byte = byte | 0x02;
		if (__imag__ in[j] > 0) byte = byte | 0x0;
		else byte = byte | 0x04;
		if ((__imag__ in[j] > QAM16_THRESHOLD) || (__imag__ in[j] < -QAM16_THRESHOLD))byte = byte | 0x01;
		if (__real__ in[j+1] > 0)byte = byte | 0x0;
		else byte = byte | 0x80;
		if ((__real__ in[j+1] > QAM16_THRESHOLD) || (__real__ in[j+1] < -QAM16_THRESHOLD)) byte = byte | 0x20;
		if (__imag__ in[j+1] > 0)byte = byte | 0x0;
		else byte = byte | 0x40;
		if ((__imag__ in[j+1] > QAM16_THRESHOLD) || (__imag__ in[j+1] < -QAM16_THRESHOLD)) byte = byte | 0x10;
		out[s] = byte;
		j=j+2;
	}
}

/*
void hard_qam16_demod(_Complex float *in, char *out, int N)
{
	int s, j=0;
	char byte;

//	printf("hard_qam16_demod()\n");
	for (s=0; s<N/2; s++) {
	//	__real__ *(in+j)=0.3;
	//	__imag__ *(in+j)=0.9;
	
//		printf("%3.1f+i%3.1f, \n", __real__ *(in+j), __imag__ *(in+j));
		byte=0x00;
//printf("0 %x\n", (int)(byte&0xFF));
		if ((float)(__real__ in[j]) > 0.0) byte = byte | 0x0;
		else byte = byte | 0x8;

//printf("1 %x\n", (int)(byte&0xFF)); 
		if ((__real__ in[j] > QAM16_THRESHOLD) || (__real__ in[j] < -QAM16_THRESHOLD))byte = byte | 0x02;
//printf("2 %x\n", (int)(byte&0xFF)); 
		if (__imag__ in[j] > 0) byte = byte | 0x0;
		else byte = byte | 0x04;

//printf("3 %x\n", (int)(byte&0xFF));
		if ((__imag__ in[j] > QAM16_THRESHOLD) || (__imag__ in[j] < -QAM16_THRESHOLD))byte = byte | 0x01;
//printf("4 %x\n", (int)(byte&0xFF));



//		printf("%3.1f+i%3.1f, \n", __real__ *(in+j+1), __imag__ *(in+j+1));
		if (__real__ in[j+1] > 0)byte = byte | 0x0;
		else byte = byte | 0x80;

//printf("5 %x\n", (int)(byte&0xFF));
		if ((__real__ in[j+1] > QAM16_THRESHOLD) || (__real__ in[j+1] < -QAM16_THRESHOLD)) byte = byte | 0x20;
//printf("6 %x\n", (int)(byte&0xFF));
		if (__imag__ in[j+1] > 0)byte = byte | 0x0;
		else byte = byte | 0x40;
//printf("7 %x\n", (int)(byte&0xFF));
		if ((__imag__ in[j+1] > QAM16_THRESHOLD) || (__imag__ in[j+1] < -QAM16_THRESHOLD)) byte = byte | 0x10;
//printf("8 %x\n", (int)(byte&0xFF));

		out[s] = byte;
//		printf("%x,", (int)(byte&0xFF)); //((int)*(out+s))&0xFF);
		j=j+2;
	}
}
 */


int norm16QAM(_Complex float *inout, int length){
	int i;
	float maxval, auxR, auxI, ratio;

	maxval = 0.00000001;
	for(i=0; i<length; i++){
		auxR=fabs(__real__ inout[i]);
		auxI=fabs(__imag__ inout[i]);
		if(maxval < auxR){
			maxval = auxR;
		}
		if(maxval < auxI){
			maxval = auxI;
		}
	}
	ratio = QAM16_LEVEL_2/maxval;

	for(i=0; i<length; i++){
		inout[i] = inout[i]*ratio;
	}
	return(1);
}


