/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
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

#include <stdio.h>
#include <math.h>
#include "skeleton.h"
#include "params.h"
#include "sink_plot_interfaces.h"
#include "sink_plot.h"
#include "gnuplot_i.h"
#define MAX_INPUT_DATA	(16*1024)

double plot_real[MAX_INPUT_DATA];
double plot_imag[MAX_INPUT_DATA];
double plot_mod[MAX_INPUT_DATA];

gnuplot_ctrl *realimag,*modul;

//Module Defined Parameters
int run_times=100;
int block_length=111;
int mocs=111;
float floatp=111.11;
char stringp[128]="MY_DEFAULT_INIT";

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {


	realimag = gnuplot_init() ;
//	gnuplot_set_background(realimag, "gold"); //"#999999");//"lemonchiffon");//"#ccffcc");
	gnuplot_setstyle(realimag,"lines");
	modul = gnuplot_init() ;
	gnuplot_setstyle(modul,"lines");

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
	int i;
	static int tslot=0;

	if(tslot==0){
		if (rcv_samples > MAX_INPUT_DATA) {
			modinfo_msg("Truncating input %d to %d samples\n",rcv_samples, MAX_INPUT_DATA);
			rcv_samples = MAX_INPUT_DATA;
		}

		for (i=0;i<rcv_samples;i++) {
			plot_real[i] = (double) __real__ inp[i];
			plot_imag[i] = (double) __imag__ inp[i];
		}
		gnuplot_resetplot(realimag);
		gnuplot_plot_x(realimag, plot_real, rcv_samples, "Real");
		gnuplot_resetplot(modul);
		gnuplot_plot_x(modul, plot_imag, rcv_samples, "Imag");

	}

//	set_output_samples(0, rcv_samples);
	tslot++;
	if(tslot==30)tslot=0;
	return rcv_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


