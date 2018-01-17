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
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "TURBODUMMY_functions.h"

/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */
void bytes2bits(char *in, char *out, int inlength){
	int i, j;
	char mask=0;
	j=0;
	for(i=0; i<inlength; i++){
		for(j=0; j<8; j++){
			mask=*(in+i);
			*(out+i*8+j)=(mask>>(7-j))&0x01;
		}
	}
}

int bits2bytes(char *in, char *out, int inlength){
	int i, j, warn=0;
	char byte=0, bit;
	if(inlength%8 != 0){
		printf("bits2bytes()| Warning: Number of input data = %d not multiple of 8\n", inlength);
		warn=-1;
	}
	for(i=0; i<inlength/8; i++){
		byte=0;
		for(j=0; j<8; j++){
			bit=*(in+i*8+j);
			byte=byte|(bit<<(7-j));
		}
		*(out+i)=byte;
	}
	return warn;
}

int encoderDUMMY(char *bitsIN, char *bitsOUT, int numbitsIN, int RATE){
	int i, k;
	int aux=0;

	for(i=0; i<numbitsIN; i++){
		aux=i*RATE;
		for(k=0; k<RATE; k++){
			*(bitsOUT+aux+k)=*(bitsIN+i);
		}
	}
	return 1;
}

int decoderDUMMY(char *bitsIN, char *bitsOUT, int numbitsIN, int RATE){

	int i, k;
	char a0=0x00, a1=0x01;
	int num0, num1;
	int aux;

	for(i=0; i<numbitsIN/RATE; i++){
		num0=0;
		num1=0;
		aux=i*RATE;
		for(k=0; k<RATE; k++){
			if(*(bitsIN+aux+k) == a0)num0++;
			if(*(bitsIN+aux+k) == a1)num1++;
		}
		if(num0 >= num1)*(bitsOUT+i)=a0;
		else *(bitsOUT+i)=a1;
	}
	return 1;


}








