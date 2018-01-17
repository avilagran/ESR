/* 
 * Copyright (c) 2012.
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

#include <complex.h>
#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <phal_sw_api.h>
#include "skeleton.h"
#include "params.h"

#include "TURBODUMMY_interfaces.h"
#include "TURBODUMMY_functions.h"
#include "TURBODUMMY.h"

//ALOE Module Defined Parameters. Do not delete.
char mname[STR_LEN]="TURBODUMMY";
int run_times=1;
int block_length=111;
char plot_modeIN[STR_LEN]="DEFAULT";
char plot_modeOUT[STR_LEN]="DEFAULT";
float samplingfreqHz=1.0;

//Module User Defined Parameters
#define CODER	0
#define DECODER 1
int mode=0; //0: CODER, 1: DECODER
int rate=3;
int debug=0; //0: NO DEBUGG, 1: DEBUGG

//Global Variables
unsigned long Tslot=0;
char bitsIN[INPUT_MAX_DATA*MAXRATE*8];
char bitsOUT[INPUT_MAX_DATA*MAXRATE*8];

/*char bitsIN_ENC[INPUT_MAX_DATA*8];
char bitsOUT_ENC[INPUT_MAX_DATA*MAXRATE];
char bitsIN_DEC[INPUT_MAX_DATA*8];
char bitsOUT_DEC[INPUT_MAX_DATA*MAXRATE];
*/
/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {

	printf("INITIALIZEoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooP\n");
	/* Get control parameters*/
	strcpy(mname, GetObjectName());
	param_get_int("mode", &mode);
	param_get_int("rate", &rate);
	param_get_int("debug", &debug);

	/* Verify control parameters */

	/* Print Module Init Parameters */
	printf("O--------------------------------------------------------------------------------------------O\n");
	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s\033[0m\n", mname);
	printf("O      Nof Inputs=%d, DataTypeIN=%s, Nof Outputs=%d, DataTypeOUT=%s\n", 
		       NOF_INPUT_ITF, IN_TYPE, NOF_OUTPUT_ITF, OUT_TYPE);
	printf("O      mode=%d [0: CODER, 1: DECODER], rate=%d\n", mode, rate);
	printf("O      debug=%d [0: NO DEBUG, 1: DEBUG]\n", debug);

	printf("O--------------------------------------------------------------------------------------------O\n");

	/* do some other initialization stuff */


	return 0;
}



/**
 * @brief Function documentation
 *
 * @param inp Input interface buffers. Data from other interfaces is stacked in the buffer.
 * Use in(ptr,idx) to access the address. To obtain the number of received samples use the function
 * int get_input_samples(int idx) where idx is the interface index.
 *
 * @param out Input interface buffers. Data to other interfaces must be stacked in the buffer.
 * Use out(ptr,idx) to access the address.
 *
 * @return On success, returns a non-negative number indicating the output
 * samples that should be transmitted through all output interface. To specify a different length
 * for certain interface, use the function set_output_samples(int idx, int len)
 * On error returns -1.
 *
 * @code
 * 	input_t *first_interface = inp;
	input_t *second_interface = in(inp,1);
	output_t *first_output_interface = out;
	output_t *second_output_interface = out(out,1);
 *
 */
int work(input_t *inp, output_t *out) {
	int rcv_samples = get_input_samples(0); /** number of samples at itf 0 buffer */
	int snd_samples=0;
	int i,k;
	int warn=0;
	
	// Check if data received
	if (rcv_samples == 0)return(0);

	//Print input
	if(debug==1){
		printf("\n%s: rcv_samples=%d\n", mname, rcv_samples);
		printf("INPUT %s: ", mname);
		for(i=0; i<rcv_samples; i++){
			printf("%02x", (unsigned char)*(inp+i));
		}
		printf("\n");
	}
	//Bytes to bits
	bytes2bits(inp, bitsIN, rcv_samples);
	rcv_samples=rcv_samples*8;

	//ENCODER
	if(mode==CODER){
		encoderDUMMY(bitsIN, bitsOUT, rcv_samples, rate);
		snd_samples=rcv_samples*rate;
	}
	//DECODER
	if(mode==DECODER){
		decoderDUMMY(bitsIN, bitsOUT, rcv_samples, rate);
		snd_samples=rcv_samples/rate;
	}
	//Bits to bytes
	warn=bits2bytes(bitsOUT, out, snd_samples);
	if(warn == -1)printf("Tslot=%d |%s.bits2bytes(): Warning snd_samples=%d not multiple of 8\n", Tslot, mname, snd_samples);
	snd_samples=snd_samples/8;

	//Print Output
	if(debug==1){
		printf("OUTPUT %s: ", mname);
		for(i=0; i<snd_samples; i++)printf("%x", (unsigned char)*(out+i));
		printf("\n");

		printf("Tslot=%d |B %s:snd_samples=%d \n", Tslot, mname,snd_samples);
		printf("\n");
	}

	Tslot++;
	return snd_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


/*
	rcv_samples=20;
	printf("%s: rcv_samples=%d\n", mname, rcv_samples);


	//Print input
	for(i=0; i<rcv_samples; i++){
		*(inp+i)=(char)i;
		printf("%x", *(inp+i));
	}
	printf("\n");
	//Bytes 2 bits
	bytes2bits(inp, bitsIN_ENC, rcv_samples);
	//Print input bits
	for(i=0; i<rcv_samples*8; i++)printf("%x", (unsigned char)*(bitsIN_ENC+i));
	printf("\n");

//BYPASS
//	for(i=0; i<rcv_samples*8; i++)bitsOUT_DEC[i]=bitsIN_ENC[i];
//	snd_samples=rcv_samples*8;

	//ENCODER
	encoderDUMMY(bitsIN_ENC, bitsOUT_ENC, rcv_samples*8, rate);
	snd_samples=rcv_samples*8*rate;


	//DECODER
	decoderDUMMY(bitsOUT_ENC, bitsOUT_DEC, snd_samples, rate);
	snd_samples=snd_samples/rate;



	//Bits 2 bytes
	warn=bits2bytes(bitsOUT_DEC, out, snd_samples);
	if(warn == -1)printf("Tslot=%d |%s.bits2bytes(): Warning snd_samples=%d not multiple of 8\n", Tslot, mname, snd_samples);
	snd_samples=snd_samples/8;
	//Print Output
	printf("\n");
	for(i=0; i<snd_samples; i++)printf("%x", (unsigned char)*(out+i));
	printf("\n");

	printf("Tslot=%d |B %s:snd_samples=%d \n", Tslot, mname,snd_samples);

 */


