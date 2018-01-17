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

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#define FILTERLENGTH	2048
#define CCINIT			0
#define CCSTREAM		1

#define PI		3.14159265359	/* pi approximation */
#define PIx2	6.28318530718

void readCPLXfilecoeff(_Complex float *filtercoeff, int *filterlength, char *filtername);
void readREALfilecoeff(_Complex float *filtercoeff, int *filterlength, char *filtername);

int bypass(_Complex float *input, int datalength,_Complex float *output);
int stream_conv_CPLX(_Complex float *ccinput, int datalength,_Complex float *filtercoeff, int filterlength, _Complex float *ccoutput);
int stream_conv_2REALS(_Complex float *ccinput, int datalength,_Complex float *filtercoeff, int filterlength, _Complex float *ccoutput);



int init_toneCOMPLEX(_Complex float *table, int length, float gain);
int gen_toneCOMPLEX(_Complex float *func_out, _Complex float *tablel, int tablesz,
					int datalen, float tone_freq, float sampl_freq);


int interpolate_C(_Complex float *in, _Complex float *out, int length, int N);
int multiply_CPLX_vectors(_Complex float *phasor_f0, _Complex float *in, _Complex float *out, int length);
int IQ_modulator_carrier_f0(_Complex float *phasor_f0, _Complex float *in, _Complex float *out, int length);

int addgain(_Complex float *inout, int datalength, float gain);
#endif
