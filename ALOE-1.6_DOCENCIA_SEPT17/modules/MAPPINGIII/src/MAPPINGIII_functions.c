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
#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MAPPINGIII_functions.h"

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


/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */
int create_LTEspectrum(_Complex float *MQAMsymb, int FFTlength, int datalength, _Complex float *out_spectrum){
	int i, j;

	for(i=0; i<FFTlength; i++)*(out_spectrum+i)=0.0+0.0i;
	j=FFTlength-datalength/2;
	for(i=0; i<datalength; i++){
		*(out_spectrum+j)=*(MQAMsymb+i);
		j++;
		if(j==FFTlength) j=1;
	}
	return(1);
}

/** PRIMARY SYNCH SIGNALS: PSS*/
/**
 * @brief Function documentation: setPSS()
 * This function calculates the Zadoff-Chu sequence.
 * @params
 * @params int phylayerID:(0, 1, 2) Physical Layer Identity within the
 * Physical Layer cell-Identity Group.
 * @params  _Complex float *PSSsymb: Output array.
 * @params int TxRxMode: -1 (Tx Mode), 1 (Rx Mode) .
 *
 * @return On success returns 1.
 * On error returns -1.
 */

int setPSS(int phylayerID, _Complex float *PSSsymb, int TxRxMode)
{
	int i;
	double arg, rootidx;

	if(phylayerID == 0)rootidx = PSSCELLID0;
	if(phylayerID == 1)rootidx = PSSCELLID1;
	if(phylayerID == 2)rootidx = PSSCELLID2;

	for(i=0; i<PSSLENGTH/2; i++){
		//arg=((float)TxRxMode)*PI*rootidx*((float)i*((float)i+1.0))/63.0;
		arg=(((double)TxRxMode)*PI*rootidx*((double)i*((double)i+1.0)))/63.0;
		//printf("arg(%d)=%lf\n", i, arg);
		__real__ PSSsymb[i]=(float)cos(arg);
		__imag__ PSSsymb[i]=(float)sin(arg);
//		printf("__real__ PSSsymb[%d]=%lf\n", i, __real__ PSSsymb[i]);

	}
	for(i=PSSLENGTH/2; i<PSSLENGTH; i++){
		arg=(((double)TxRxMode)*PI*rootidx*(((double)i+2.0)*((double)i+1.0)))/63.0;
		__real__ PSSsymb[i]=(float)cos(arg);
		__imag__ PSSsymb[i]=(float)sin(arg);
	}
	return 1;
}


/**@ingroup genPSStime_seq
 * This module generate the PSS time sequence for the different FFT size
 * \param cellID: Identifies the sequence number: 0, 1, 2
 * \param FFTsize: define the size of the OFMD symbols: 128, 256, 512, 1024, 1536 o 2048
 * \param TxRxmode: defines if the sequence generate is for Tx or Rx side
 */


int genPSStime_seq(int phylayerID, int FFTsize, fftwf_complex  *PSS_time, int TxRxmode){

	int s, i;
	_Complex float PSS_ID[PSSLENGTH+2];
	/**FFT*/
	fftwf_complex PSS_freq[2048];
	fftwf_plan plan128genPSS;

	/**Select cellID: 0, 1, 2*/
	setPSS(phylayerID, PSS_ID, TxRxmode);
	//TX PSS: ROTATE
	memset(PSS_freq, 0, sizeof(_Complex float)*FFTsize);
	s=1;	//DC at position O
	for(i=PSSLENGTH/2; i<PSSLENGTH; i++){
			PSS_freq[s] = PSS_ID[i];
			s++;
	}
	s=(FFTsize-(PSSLENGTH/2));
	for(i=0; i<PSSLENGTH/2; i++){
			PSS_freq[s] = PSS_ID[i];
			s++;
	}
	if(FFTsize==128){
		plan128genPSS = fftw_plan_dft_1d(128, PSS_freq, PSS_time, FFTW_BACKWARD, FFTW_ESTIMATE);
		fftw_execute(plan128genPSS);
	}

	return 0;
}

int genPSSfreq_seq(int phylayerID, int FFTsize, _Complex float  *PSS_freq, int TxRxmode){
	int s, i;
	_Complex float PSS_ID[PSSLENGTH+2];

	/**Select cellID: 0, 1, 2*/
	setPSS(phylayerID, PSS_ID, TxRxmode);
	//TX PSS: ROTATE
	memset(PSS_freq, 0, sizeof(_Complex float)*FFTsize);
	s=1;	//DC at position O
	for(i=PSSLENGTH/2; i<PSSLENGTH; i++){
			PSS_freq[s] = PSS_ID[i];
			s++;
	}
	s=(FFTsize-(PSSLENGTH/2));
	for(i=0; i<PSSLENGTH/2; i++){
			PSS_freq[s] = PSS_ID[i];
			s++;
	}
	return(0);
}



int sendzeros(int fftlength, _Complex float *outbuffer){
	int k;
	for(k=0; k<fftlength*14; k++){
		*(outbuffer+k)=0.0+0.0i;
	}
	return(0);
}

int sendzerosPSS(_Complex float *PSS, int fftlength, _Complex float *outbuffer){
	int k;

	//printf("MAPPINGII_functions.c: sendzerosPSS()\n");
	//ALL ZEROS
	for(k=0; k<fftlength*14; k++){
		*(outbuffer+k)=0.0+0.0i;
	}	
	//ADD PSS
	create_LTEspectrum(PSS, fftlength, PSSLENGTH, &outbuffer[6*fftlength]);

	return(0);
}

int send_firstSUBFRAME(_Complex float *PSS, _Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer){
	int k, j;

	for(k=0; k<fftlength*5; k++){
		*(outbuffer+k)=0.0+0.0i;
	}
	//ADD first PSS
	create_LTEspectrum(PSS, fftlength, PSSLENGTH, &outbuffer[5*fftlength]);
	//ADD second PSS
	create_LTEspectrum(PSS, fftlength, PSSLENGTH, &outbuffer[6*fftlength]);
	//ADD DATA
	j=0;
	for(k=7; k<14; k++){
		create_LTEspectrum(&inbuffer[j*datasize], fftlength, datasize, &outbuffer[k*fftlength]);
		j++;
	}		
	return(0);
}


int send_dataSUBFRAME(_Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer){
	int k;
	//ADD DATA	
	//ADD HERE YOUR CODE

	for(k=7; k<14; k++){
		create_LTEspectrum(&inbuffer[k*datasize], fftlength, datasize, &outbuffer[k*fftlength]);
	
	}	

	return(0);
}

int send_pssSUBFRAME(_Complex float *PSS, _Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer){
	int k,j;
	//ADD DATA
	//ADD PSS
	//ADD DATA

	for(k=0; k<6; k++){
		create_LTEspectrum(&inbuffer[k*datasize], fftlength, datasize, &outbuffer[k*fftlength]);
		
	}	
	j=6;
	
	for(k=7; k<14; k++){
		create_LTEspectrum(&inbuffer[j*datasize], fftlength, datasize, &outbuffer[k*fftlength]);
		j++;
	}	
	
	return(0);
}




