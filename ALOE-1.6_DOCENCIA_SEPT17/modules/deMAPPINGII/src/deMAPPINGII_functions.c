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

#include "deMAPPINGII_functions.h"

/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */
int init_functionA_COMPLEX(_Complex float *input, int length){
	int i;

	printf("INIT MY FUNCTION\n");

	for (i=0;i<length;i++) {
		if(i>20) __real__ input[i]=(float)(i*i);
		__imag__ input[i]=(length-i-1)%(length);
	}
	return(1);
}


int init_functionB_FLOAT(float *input, int length){
	int i;

	printf("INIT MY FUNCTION\n");

	for (i=0;i<length;i++) {
		if(i>20) input[i]=(float)(i*i);
	}
	return(1);
}

/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */
int functionA_COMPLEX(_Complex float *input, int inlength, _Complex float *output)
{
	int i,outlength;

//	printf("RUN MY FUNCTION\n");	
	for (i=0;i<inlength;i++) {
		__real__ output[i] = __real__ input[i];
		__imag__ output[i] = __imag__ input[i];
	}
	outlength=inlength;
	return outlength;
}

int functionB_FLOAT(float *input, int inlength,float *output)
{
	int i, outlength;

//	printf("RUN MY FUNCTION\n");	
//	for(k=0; k<length; k++)printf("Real input[%d]=%3.4f\n",k,  __real__ input[k]);
	for (i=0;i<inlength;i++) {
		output[i] = input[i];
	}
//	for(k=0; k<length; k++)printf("Real output[%d]=%3.4f\n",k,  __real__ output[k]);
	outlength=inlength;
	return outlength;
}

int get_LTEspectrum(_Complex float *in_spectrum, int FFTlength, int datalength, _Complex float *MQAMsymb, int trozo)
{	//					  fftlength=128 datalength=72	
	int i, j;
	//En la entreda tenemos:	
	//Tenemos datos primero de 92 hasta 128 y luego de 1 hasta 37
	//Tenemos vacio desde el 38 hasta el 91
	if(trozo = 0){	
		for(i=0; i<datalength*14; i++)
			*(MQAMsymb+i)=0.0+0.0i;
	}
	
	j=FFTlength-datalength/2; // j=92

	for(i=0; i<FFTlength; i++){
		*(MQAMsymb+i)=*(in_spectrum+j);//Conseguimos symbolos del 92 hasta el 128
		j++;
		if(j==FFTlength)
			j=1;	//En la posicion 0 de cada trama tiene que estar vacia por el LTE
	}
	return(1);
}

int send_dataSUBFRAME(_Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer){
	int k;
	//ADD DATA
	for(k = 0; k<14; k++){
		get_LTEspectrum(&inbuffer[k*fftlength], fftlength, datasize, &outbuffer[k*datasize], k);
	}	
	return(0);
}
//HAcer la inversa del create_LTEspectrum y llamarla 14 veces
