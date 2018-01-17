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

#include "DUCt16_interfaces.h"
#include "DUCt16_functions.h"
#include "DUCt16.h"

//ALOE Module Defined Parameters. Do not delete.
char mname[STR_LEN]="DUCt16";
int run_times=1;
int block_length=111;
char plot_modeIN[STR_LEN]="DEFAULT";
char plot_modeOUT[STR_LEN]="DEFAULT";
float samplingfreqHz=1.0;

//Module User Defined Parameters
float gain=1.0;
float f0_freqHz=3500.0;
int Ninterpol=8;

//Global Variables
// Sinus cosinus generation
#define TABLESZ		48000
_Complex float tableA[TABLESZ];

// Filter
#define MAXNUMCOEFFS	2048
char filtercoefffile[STR_LEN]="FIR.coeff";
int filterlength=0;
_Complex float COEFFs[MAXNUMCOEFFS];
_Complex float aux[INPUT_MAX_DATA];

//#define STANDALONE
/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {

	printf("INITIALIZEoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooP\n");
	/* Get control parameters*/
	param_get_float("samplingfreqHz", &samplingfreqHz);
	param_get_float("f0_freqHz", &f0_freqHz);
	param_get_float("gain", &gain);
	param_get_int("Ninterpol", &Ninterpol);
	param_get_string("filtercoefffile", &filtercoefffile[0]);

	/* do some other initialization stuff */
	// INIT TABLE SIN COS
	init_toneCOMPLEX(tableA, TABLESZ, 1.0);
	// READ FILTER COEFF
//	readCPLXfilecoeff(COEFFs, &filterlength, filtercoefffile);
	readREALfilecoeff(COEFFs, &filterlength, filtercoefffile);


	/* Print Module Init Parameters */
#ifndef STANDALONE
	strcpy(mname, GetObjectName());
#endif
	printf("O--------------------------------------------------------------------------------------------O\n");
	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s\033[0m\n", mname);
	printf("O      Nof Inputs=%d, DataTypeIN=%s, Nof Outputs=%d, DataTypeOUT=%s\n", 
		       NOF_INPUT_ITF, IN_TYPE, NOF_OUTPUT_ITF, OUT_TYPE);
	printf("O      sampligfreqHz=%3.3f\n", samplingfreqHz);
	printf("O      fo_freqHz=%3.1f, gain=%3.1f, Ninterpol=%d\n", f0_freqHz, gain, Ninterpol);
	printf("O      filtercoefffile=%s, filterlength=%d\n", filtercoefffile, filterlength);
	printf("O--------------------------------------------------------------------------------------------O\n");

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
	_Complex float sentcos[INPUT_MAX_DATA];
	
	// Check if data received
	if (rcv_samples == 0)return(0);

	// Interpolate input signal
	snd_samples=interpolate_C(inp, aux, rcv_samples, Ninterpol);
	// Filter Low Pass
	snd_samples = stream_conv_2REALS(aux, snd_samples, COEFFs, filterlength, out);
	// Move spectrum fo Hz
	// 1) Generate phasor f0 Hz
	gen_toneCOMPLEX(sentcos, tableA, TABLESZ, snd_samples, f0_freqHz, samplingfreqHz);
	// 2) Modulate IQ carrier fo
	IQ_modulator_carrier_f0(sentcos, out, out, snd_samples);
	// ADD GAIN
	addgain(out, snd_samples, gain);

	// Indicate the number of samples at output 0 with return value
	return snd_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


