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
#include <math.h>

#include "DUCt16_functions.h"




void readCPLXfilecoeff(_Complex float *filtercoeff, int *filterlength, char *filtername){

	char str[128];
	float auxf;
	int num, k=0;
	float real=0.0, imag=0.0;
	char sign;

	//strcpy(str, "../../APPs/");
	//strcat(str, filtername);
	//printf("%s\n", filtername);

	//Reading the file from .txt file
    FILE *hFile;
   // hFile = fopen("/home/antoni/DADES/NOU_DADES/DOCENCIA/ASSIGNATURES/ESR/ALOE_DOCENCIA/ALOE-1.6_DOCENCIA_WORKING_OCT16/APPs/WAVEForms/FILTER/paramscfg/FIR.coeff", "r");
	// OK hFile = fopen("../../../APPs/WAVEForms/FILTER/paramscfg/FIR.coeff", "r");
	//hFile = fopen("paramscfg/FIR.coeff", "r");
	hFile = fopen(filtername, "r");

    if (hFile == NULL){
        printf("\033[1m\033[31mERROR!!!. FILE %s NOT FOUND\033[0m\n", filtername);// Error, file not found
    }
    else{
        fscanf(hFile, "%f", &auxf);
		*filterlength=(int)auxf;
		while( (num = fscanf( hFile, "%f %c %fi\n", &real, &sign, &imag)) > 0 ) {
			if( sign == '-' )imag *= -1;
	    	filtercoeff[k] = real + imag*I;
//			printf("readCPLXfilecoeff(): real=%3.6f, imag=%3.6f\n", __real__ filtercoeff[k], __imag__ filtercoeff[k]);
			k++;
		}
     	if(k != *filterlength){
			printf("\033[1m\033[31mWARNING!!!!. Please, verify the %s file. The filter length do not match the captured coefficients\033[0m\n", filtername);
         	printf("\033[1m\033[31mfilterlength=%d, number of coeefs read=%d\033[0m\n", *filterlength, k);
		}
    }
	printf("\033[1m\033[31mfilterlength=%d, number of coeefs read=%d\033[0m\n", *filterlength, k);
    fclose(hFile);
}


void readREALfilecoeff(_Complex float *filtercoeff, int *filterlength, char *filtername){

	char str[128];
	float auxf;
	int num, k=0;
	double real=0.0, imag=0.0;
	char sign;

	//strcpy(str, "../../APPs/");
	//strcat(str, filtername);
	//printf("%s\n", filtername);

	//Reading the file from .txt file
    FILE *hFile;
   // hFile = fopen("/home/antoni/DADES/NOU_DADES/DOCENCIA/ASSIGNATURES/ESR/ALOE_DOCENCIA/ALOE-1.6_DOCENCIA_WORKING_OCT16/APPs/WAVEForms/FILTER/paramscfg/FIR.coeff", "r");
	// OK hFile = fopen("../../../APPs/WAVEForms/FILTER/paramscfg/FIR.coeff", "r");
	//hFile = fopen("paramscfg/FIR.coeff", "r");
	hFile = fopen(filtername, "r");

    if (hFile == NULL){
        printf("\033[1m\033[31mERROR!!!. FILE %s NOT FOUND\033[0m\n", filtername);// Error, file not found
    }
    else{
        fscanf(hFile, "%f", &auxf);
		*filterlength=(int)auxf;
		while( (num = fscanf( hFile, "%lf\n", &real)) > 0 ) {
	    	filtercoeff[k] = (float)real + ((float)real)*I;
	    	//			printf("readCPLXfilecoeff(): real=%3.6f, imag=%3.6f\n", __real__ filtercoeff[k], __imag__ filtercoeff[k]);
			k++;
		}
     	if(k != *filterlength){
			printf("\033[1m\033[31mWARNING!!!!. Please, verify the %s file. The filter length do not match the captured coefficients\033[0m\n", filtername);
         	printf("\033[1m\033[31mfilterlength=%d, number of coeefs read=%d\033[0m\n", *filterlength, k);
		}
    }
	printf("\033[1m\033[31mfilterlength=%d, number of coeefs read=%d\033[0m\n", *filterlength, k);
    fclose(hFile);
}

int addgain(_Complex float *inout, int datalength, float gain){
	int i;
	for(i=0; i<datalength; i++){
		*(inout+i)=(*(inout+i))*gain;
	}
	return datalength;
}




int bypass(_Complex float *input, int datalength,_Complex float *output){
	int i;
	for(i=0; i<datalength; i++){
		*(output+i)=*(input+i);
	}
	return datalength;
}


int stream_conv_CPLX(_Complex float *ccinput, int datalength,_Complex float *filtercoeff, int filterlength, _Complex float *ccoutput){

	int i, j;
	static int first=0;
	static _Complex float aux[FILTERLENGTH];

	if(first==0){
		for(j=0; j<filterlength; j++){
			aux[j]=0.0+0.0i;
			//printf("FIRST CCONV real=%3.6f, imag=%3.6f\n", __real__ filtercoeff[j], __imag__ filtercoeff[j]);
		}
		first=1;
	}

	for (i=0;i<datalength;i++) {
		for (j=filterlength-2;j>=0;j--) {
			aux[j+1]=aux[j];
		}
		aux[0]=ccinput[i];
		ccoutput[i]=0.0;
		for (j=0;j<filterlength;j++) {
			ccoutput[i]+=aux[j]*filtercoeff[j];
		}
	}
	return datalength;
}

int stream_conv_2REALS(_Complex float *ccinput, int datalength,_Complex float *filtercoeff, int filterlength, _Complex float *ccoutput){

	int i, j;
	static int first=0;
	static _Complex float aux[FILTERLENGTH];

	if(first==0){
		for(j=0; j<filterlength; j++){
			aux[j]=0.0+0.0i;
			//printf("FIRST CCONV real=%3.6f, imag=%3.6f\n", __real__ filtercoeff[j], __imag__ filtercoeff[j]);
		}
		first=1;
	}

	for (i=0;i<datalength;i++) {
		for (j=filterlength-2;j>=0;j--) {
			aux[j+1]=aux[j];
		}
		aux[0]=ccinput[i];
		ccoutput[i]=0.0;
		for (j=0;j<filterlength;j++) {
			__real__ ccoutput[i]+=__real__ aux[j]*__real__ filtercoeff[j];
			__imag__ ccoutput[i]+=__imag__ aux[j]*__imag__ filtercoeff[j];

		}
	}
	return datalength;
}

int init_toneCOMPLEX(_Complex float *table, int length, float gain){
	int i;
	double arg=PIx2/((float)length);

	for (i=0;i<length;i++) {
		__real__ table[i]=gain*(float)cos(arg*(double)i);
		__imag__ table[i]=-gain*(float)sin(arg*(double)i);
	}
	return(1);
}

int gen_toneCOMPLEX(_Complex float *func_out, _Complex float *tablel, int tablesz,
					int datalen, float tone_freq, float sampl_freq){
	int i, k=1;
	static int j=0;
	float ref_freq_c;

	ref_freq_c=sampl_freq/(float)tablesz;
	k=(int)(tone_freq/ref_freq_c);
	for (i=0;i<datalen;i++) {
		func_out[i] = tablel[j];
		j += k;
		if(j>=tablesz)j-=tablesz;
	}
	return(1);
}


int interpolate_C(_Complex float *in, _Complex float *out, int length, int N){
	int i, k;

	for(i=0; i<length; i++){
		for(k=0; k<N; k++){
			if(k==0)*(out+i*N+k)=*(in+i);
			else *(out+i*N+k)=0.0+0.0i;
		}
	}
	return(length*N);
}


int multiply_CPLX_vectors(_Complex float *phasor_f0, _Complex float *in, _Complex float *out, int length){
	int i;

	for(i=0; i<length; i++){
		*(out+i)=*(phasor_f0+i) * *(in+i);
	}
	return 1;
}

int IQ_modulator_carrier_f0(_Complex float *phasor_f0, _Complex float *in, _Complex float *out, int length){
	int i;

	for(i=0; i<length; i++){
		__real__ *(out+i)=(__real__*(phasor_f0+i)) * (__real__ *(in+i)) + (__imag__*(phasor_f0+i)) * (__imag__ *(in+i));
		__imag__ *(out+i)=0.0;
	}
	return 1;
}



