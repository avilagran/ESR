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
#include <sys/time.h>
#include <phal_sw_api.h>

//Skeleton includes
#include "paramsSK15.h"
#include "skeletonSK15.h"

//Module includes
#include "demod16QAM.h"
#include "demod16QAM_functions.h"
#include "demod16QAM_params.h"


/* MODULE DEFINED PARAMETERS.///////////////////////////////////////*/
//#define DEBUG_STANDALONE

char mname[STR_LEN]="demod16QAM";
const int nof_parameters=NOF_PARAMS;
const int nof_extra_vars = NOF_EXTRA_VARS;

//Global Variables
unsigned long int numTS;	/* Time slot number */
_Complex float bufferA[2048];
float bufferB[2048];

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {
	int i;
	static char aux_string[256];

#ifndef DEBUG_STANDALONE
	sprintf(mname, "%s", GetObjectName());
#endif

//	printf("INITIALIZE  TEMPLATE3oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	/* Get control parameters value from modulename.params file*/
	// Params: local to module control parameters
//	param_get_int("opMODE", &oParam.opMODE);							//Initialized by hand or .params file
//	param_get_int("num_operations", &oParam.num_operations);			//Initialized by hand or .params file
//	param_get_float("constant", &oParam.constant);
//	param_get_string("datatext", &oParam.datatext[0]);

	/* Verify control parameters */
/*	if (oParam.num_operations > get_input_max_samples()) {
		//Include the file name and line number when printing
		moderror_msg("ERROR: p_num_operations=%d > INPUT_MAX_SAMPLES=%d\n", oParam.num_operations, INPUT_MAX_SAMPLES);
		moderror("Check your module_interfaces.h file\n");
		return -1;
	}
*/
	/*Include the file name and line number when printing*/
//	modinfo_msg("Parameter p_num_operations is %d\n",oParam.num_operations);	//Print message and parameter
//	modinfo("Parameter p_num_operations \n");							//Print only message

	/* Print Module Init Parameters */
	printf("INITIALIZEoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	printf("O--------------------------------------------------------------------------------------------O\n");
//	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s.Initialize()\033[0m\n", mname);
	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s.Initialize()\033[0m\n", GetObjectName());
	printf("O      Nof Inputs=%d, DataTypeIN=%s, Nof Outputs=%d, DataTypeOUT=%s\n", 
		       NOF_INPUT_ITF, IN_TYPE, NOF_OUTPUT_ITF, OUT_TYPE);
//	printf("O      opMODE=%d, num_operations=%d, constant=%3.3f\n", oParam.opMODE, oParam.num_operations, oParam.constant);
//	printf("O      datatext=%s, datatext=%s\n", oParam.datatext, oParam.datatext);
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
int work(void *inp, void *out) {
	int i;
	static int Tslot=0;
	input_t *input;
	output_t *output;

	input=inp;
	output=out;
	int snd_samples, rcv_samples = get_input_samples(0); /** number of samples at itf 0 buffer */

	/* PUT HERE YOUR DSP TASKS ///////////////////////////////*/
/*	printf("DEMOD \n");
	for(i=0; i<rcv_samples; i++){
		printf("%3.1f+i%3.1f, ", __real__ *(input+i), __imag__ *(input+i));
	}
*/
	norm16QAM(inp, rcv_samples);

	hard_qam16_demod(inp, out, rcv_samples);
	//SEND SAMPLES
	snd_samples=rcv_samples/2;
	/* SET NUMBER OF SAMPLES AT EACH OUTPUT */
	set_output_samples(0, snd_samples);				//	set_output_samples(Output_number, number_of_samples);
	
	// UPDATE TIME SLOT COUNTER
	Tslot++;
	return 1;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}

