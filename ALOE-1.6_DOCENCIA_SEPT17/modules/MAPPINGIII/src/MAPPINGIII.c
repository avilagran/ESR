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

#include "MAPPINGIII_interfaces.h"
#include "MAPPINGIII_functions.h"
#include "MAPPINGIII.h"

//ALOE Module Defined Parameters. Do not delete.
char mname[STR_LEN]="MAPPINGIII";
int run_times=1;
int block_length=72;
char plot_modeIN[STR_LEN]="DEFAULT";
char plot_modeOUT[STR_LEN]="DEFAULT";
float samplingfreqHz=48000.0;

//Module User Defined Parameters
#define LAB2	0
#define LAB3	1
int mode=LAB2;				//0: LAB2; 1:LAB3
int debugMODE=0;			//0: Normal Execution, 1: AA, 2: BB
#define BUFFER_SZ	2048*10
_Complex float buffer[BUFFER_SZ];
_Complex float bufferA[BUFFER_SZ];
_Complex float bufferB[BUFFER_SZ];
unsigned int PSS2SUBFRAMECTRL=252;	
unsigned int SUBFRAME[10]={468,504,504,504,504,468,504,504,504,504};
//unsigned int PSS2SUBFRAMECTRLLAB3=81;
//unsigned int SUBFRAMELAB3[10]={153,165,165,165,165,153,165,165,165,165};
unsigned int PSS2SUBFRAMECTRLLAB3=153;
unsigned int SUBFRAMELAB3[70] = {81,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 153,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0,
								 165,0,0,0,0,0,0};




//Global Variables
#define FFTSIZE	128
int pss_length=PSSLENGTH;
_Complex float PSSseq[PSSLENGTH];
int PSS_idx=0; 	//0, 1 or 2
int TxRxMode=1; //TX: 1, RX: -1
int FFTsize=FFTSIZE;
int DATAsize=72;
int phylayerID=0;
_Complex float PSSfreqseq[FFTSIZE];


// Circular buffer
#define HALFBUFF	32768
#define CBUFFER_SZ	HALFBUFF*2
_Complex float CBuffer[CBUFFER_SZ];
buffctrl buffCtrl;


/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {
	int i;

	printf("INITIALIZEoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooP\n");
	/* Get control parameters*/
	param_get_float("samplingfreqHz", &samplingfreqHz);
	param_get_int("phylayerID", &phylayerID);
	param_get_int("FFTsize", &FFTsize);
	param_get_int("debugMODE", &debugMODE);
	param_get_int("mode", &mode);

	/* Verify control parameters */
	if (block_length > get_input_max_samples()) {
		/*Include the file name and line number when printing*/
		moderror_msg("ERROR: Block length=%d > INPUT_MAX_DATA=%d\n", block_length, INPUT_MAX_DATA);
		moderror("Check your MAPPINGII_interfaces.h file\n");
		return -1;
	}
	/*Include the file name and line number when printing*/
	modinfo_msg("Parameter block_length is %d\n",block_length);	//Print message and parameter 
	modinfo("Parameter block_length \n");				//Print only message

	/* Print Module Init Parameters */
	printf("O--------------------------------------------------------------------------------------------O\n");
	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s\033[0m\n", mname);
	printf("O      Nof Inputs=%d, DataTypeIN=%s, Nof Outputs=%d, DataTypeOUT=%s\n", 
		       NOF_INPUT_ITF, IN_TYPE, NOF_OUTPUT_ITF, OUT_TYPE);
	printf("O      sampligfreqHz=%3.3f\n", samplingfreqHz);
	printf("O      mode=%d [0: LAB2, 1:LAB3]\n", mode);
	printf("O      debugMODE=%d \n", debugMODE);
	printf("O      debugMODE=0: Normal execution\n");
	printf("O      debugMODE=1: Load 14 OFDM symbols real Data. Command sent to ask for required data\n");
	printf("O      debugMODE=2: Load 14 OFDM symbols Data from a pattern. Command sent to ask for no data\n");
	printf("O      phylayerID=%d, FFTsize=%d\n", phylayerID, FFTsize);
	printf("O--------------------------------------------------------------------------------------------O\n");

	/* do some other initialization stuff */
	//CREATE PSS SEQUENCE
	setPSS(PSS_idx, PSSseq, TxRxMode);
	//CREATE PSS SEQUENCE IN FREQ DOMAIN
	genPSSfreq_seq(phylayerID, FFTsize, PSSfreqseq, TxRxMode);
	//Number of carriers per FFTsize
	if(FFTsize==128)DATAsize=72;
	if(FFTsize==256)DATAsize=180;
	if(FFTsize==512)DATAsize=300;
	if(FFTsize==1024)DATAsize=600;
	if(FFTsize==1536)DATAsize=900;
	if(FFTsize==2048)DATAsize=1200;
	//CREATE TEST SEQUENCE
	if(debugMODE==2){
		for(i=0; i<2048; i++)bufferA[i]=(((float)(i%72))/72.0+0.0i);
	}
	if(debugMODE==3){
		for(i=0; i<2048; i++)bufferA[i]=0.0+0.0i;
	}
	
	//Initialize Circular Buffer
//	initCbuff(&buffCtrl, CBuffer, CBUFFER_SZ, 0);

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


#define TEST_DEMAPPINGII


int work(input_t *inp, output_t *out) {
	
	static int TslotDATA=0, TslotCTRL=0, COUNT7=0;
	int k, i;
	int rcv_samples = get_input_samples(0); /** number of samples at itf 0 buffer */
	// First output interface
	// Normal execution always the same output: 14xFFTsize
	int snd_samples=FFTsize*14;
	// Second output interface
	char *out_char;
	int snd_samples1=1;

	rcv_samples=72*14; //añadido por nosotros

	unsigned int *out_int;
	out_int=out(out,1);

	// Control actions 
	static int initPHASE=ZEROSUBFRAME;		 //O: NOT SEND DATA; 1: SEND 2 PSS; 2: NORMAL SEND
	static int action=0; 					//0: SEND ZEROS, 1: SEND PSS; 2: SEND DATA
	static int numSUBFRAME=0;
	static int firstSUBFRAME=0;
	int ACTIVE=0;

	_Complex float *DATAp;

//	for(i=0; i<2048; i++)inp[i]=bufferB[i];


	//SEND CTRL LAB3
	if(mode==LAB3){
		ACTIVE=0;
		*out_int=ZEROSUBFRAME;
		if(TslotCTRL%7==0)ACTIVE=1;
		COUNT7=(TslotCTRL+5);
		if(COUNT7>69)COUNT7=COUNT7-70;
		if(numSUBFRAME==1){
			if(firstSUBFRAME==0){
				//*out_int=FIRSTSUBFRAME;
				*out_int=SUBFRAMELAB3[COUNT7];
				SUBFRAMELAB3[0]=PSS2SUBFRAMECTRLLAB3;
				firstSUBFRAME=1;
				
			}
			else {
				*out_int=SUBFRAMELAB3[COUNT7];
			}
		}
		if(numSUBFRAME==0){
			if(COUNT7==69)numSUBFRAME=1;
		}
			
	}
	//SEND CTRL LAB2
	if(mode==LAB2){
		if(TslotCTRL==-1)TslotCTRL=3;
		*out_int=ZEROSUBFRAME;
		if(numSUBFRAME==1){
			if(firstSUBFRAME==0){
				//*out_int=FIRSTSUBFRAME;
				*out_int=PSS2SUBFRAMECTRL;
				firstSUBFRAME=1;
			}
			else *out_int=SUBFRAME[TslotCTRL];
		}
		if(numSUBFRAME==0)if(TslotCTRL==9)numSUBFRAME=1;
	}

//////////////////////// TEST //////////////////////////////////
	///TEST DEMAPPING
	if(debugMODE==1){
		*out_int=SUBFRAME[1];
		initPHASE=NORMALSUBFRAME;
		if(rcv_samples==0)snd_samples=0;
	}
////////////////////////////////////////////////////////////////

	//DECIDE ACTION
	if(ACTIVE)if(rcv_samples != (int)(initPHASE*DATAsize))printf("Tslot=%d || MAPPINGII.c() ERROR: rcv_samples=%d =! initPHASE=%d\n",
														TslotDATA, rcv_samples, initPHASE*DATAsize);

//	printf("MIG TslotCTRL=%d, TslotDATA=%d || MAPPINGII: rcv_samples:%d, initPHASE: %d, ACTIVE=%d\n",
//					TslotCTRL, TslotDATA,rcv_samples, initPHASE, ACTIVE);

	// SELECT THE DATA TO BE SEND
	if(debugMODE < 2)DATAp = inp;			// MODULE INPUT DATA
	else DATAp = bufferA;					// GENERATED DATA
	
	
	if(ACTIVE){ 

		switch(initPHASE){
			case ZEROSUBFRAME:
				//action=SENDZEROS;
				if(TslotDATA==0 || TslotDATA==5)sendzerosPSS(PSSseq,FFTsize, buffer);
				else sendzeros(FFTsize, buffer);	
				if(TslotDATA==9)initPHASE=FIRSTSUBFRAME;		
				break;
			case FIRSTSUBFRAME:			
				//action=SENDFIRST;
				send_firstSUBFRAME(PSSseq, DATAp, FFTsize, DATAsize, buffer);

//				if(debugMODE < 2)send_firstSUBFRAME(PSSseq, inp, FFTsize, DATAsize, buffer);
//				else send_firstSUBFRAME(PSSseq, bufferA, FFTsize, DATAsize, buffer);
						
				if(TslotDATA==0)initPHASE=NORMALSUBFRAME;
				break;
			case PSSSUBFRAME:
				//action=SENDPSS+DATA;
				send_pssSUBFRAME(PSSseq, DATAp, FFTsize, DATAsize, buffer);

//				if(debugMODE < 2)send_pssSUBFRAME(PSSseq, inp, FFTsize, DATAsize, buffer);
				///TEST MAPPING
//				else send_pssSUBFRAME(PSSseq, bufferA, FFTsize, DATAsize, buffer);

				initPHASE=NORMALSUBFRAME;
				break;
			case NORMALSUBFRAME:
				//action=SENDDATA;
				send_dataSUBFRAME(DATAp, FFTsize, DATAsize, buffer);

//				if(debugMODE < 2)send_dataSUBFRAME(inp, FFTsize, DATAsize, buffer);
				///TEST MAPPING
//				else send_dataSUBFRAME(bufferA, FFTsize, DATAsize, buffer);

				if(TslotDATA==4 || TslotDATA==9)initPHASE=PSSSUBFRAME;
				break;
			default:
				printf("MAPPING: ERROR: NON Valid initPHASE=%d value\n", initPHASE);
				break;  
		}
		TslotDATA++;
		if(TslotDATA==10)TslotDATA=0;

	}else snd_samples=0;

	// REPEAT CTRL EVERY 70 Tslots
	TslotCTRL++;
	if(TslotCTRL==70)TslotCTRL=0;

	// SEND CTRL OUTPUT
	set_output_samples(1,snd_samples1);
	// SEND DATA TO OUTPUT
	if(snd_samples>0)memcpy(out, buffer, sizeof(_Complex float)*snd_samples);  

	return snd_samples;		
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


