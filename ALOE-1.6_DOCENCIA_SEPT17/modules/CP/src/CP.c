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

#include "CP_interfaces.h"
#include "CP_functions.h"
#include "CP.h"

//ALOE Module Defined Parameters. Do not delete.
char mname[STR_LEN]="addCP";
int run_times=1;
int block_length=111;
char plot_modeIN[STR_LEN]="DEFAULT";
char plot_modeOUT[STR_LEN]="DEFAULT";
float samplingfreqHz=1.0;

//Module User Defined Parameters
float floatp=111.11;
char stringp[STR_LEN]="MY_DEFAULT_INIT";

//Global Variables
_Complex float bufferA[2048];
float bufferB[2048];

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {

	printf("INITIALIZEoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooP\n");
	/* Get control parameters*/
	param_get_int("block_length", &block_length);		//Initialized by hand or config file
	param_get_int("run_times", &run_times);			//Initialized by hand or config file
	param_get_string("plot_modeIN", &plot_modeIN[0]);	//Initialized by hand or config file
	param_get_string("plot_modeOUT", &plot_modeOUT[0]);
	param_get_float("samplingfreqHz", &samplingfreqHz);
	param_get_float("floatp", &floatp);
	param_get_string("stringp", &stringp[0]);

	/* Verify control parameters */
	if (block_length > get_input_max_samples()) {
		/*Include the file name and line number when printing*/
		moderror_msg("ERROR: Block length=%d > INPUT_MAX_DATA=%d\n", block_length, INPUT_MAX_DATA);
		moderror("Check your addCP_interfaces.h file\n");
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
	printf("O      block_length=%d, run_times=%d, sampligfreqHz=%3.3f\n", block_length, run_times, samplingfreqHz);
	printf("O      plot_modeIN=%s, plot_modeOUT=%s\n", plot_modeIN, plot_modeOUT);
	printf("O      floatp=%3.3f, stringp=%s\n", floatp, stringp);
	printf("O--------------------------------------------------------------------------------------------O\n");

	/* do some other initialization stuff */
	init_functionA_COMPLEX(bufferA, 1024);
//	init_functionB_FLOAT(bufferB, 1024);

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
	printf("CP_work:\n");
	int rcv_samples = get_input_samples(0); /** number of samples at itf 0 buffer */
//	int i,k;
	static int Tslot=-1;
	Tslot++;
	int numnormalCP=12;
	int numextCP=2;
	// Check if data received
	if (rcv_samples == 0)return(0);
	// Check if data exceed maximum expected data
	int snd_samples=128*14+numextCP*10+numnormalCP*9;
	int i=0;
	for(i=0;i<snd_samples;i++) *(out)=0+0i;
	int extendedCP=-1;
	int normalCP=0;
	int CPlen;
	int offset=0;
	for(i=0;i < 14;i++){
		if(i==0 || i==7){
			 extendedCP++;
			 CPlen=10;
		}
		else{
			 normalCP++;
			 CPlen=9;
		}
		if(i==1) offset++;
		if(i==7) offset--;
		if(i==8) offset=2;
		memcpy(out+(i*128)+(10*extendedCP)+(9*normalCP)+offset,inp+(i*128)+128-CPlen,sizeof(_Complex float)*CPlen);
		memcpy(out+(i*128)+(10*extendedCP)+(9*normalCP)+CPlen+offset,inp+(i*128),sizeof(_Complex float)*128);
	printf("CP samples: %d\n", snd_samples);
	}
	printf("CP samples: %d\n", snd_samples);
	return snd_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


