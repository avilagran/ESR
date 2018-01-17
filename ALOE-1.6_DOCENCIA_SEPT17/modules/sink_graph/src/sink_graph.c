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
#include <complex.h>	// Includes complex types functions
#include <math.h>
#include <fftw3.h>
#include "skeleton.h"
#include "params.h"
#include "sink_graph_interfaces.h"
#include "sink_graph.h"
#include "gnuplot_i.h"
#include <sys/time.h>
#include <time.h>
//#define MAX_INPUT_DATA	(16*1024)

double plot_real[INPUT_MAX_DATA];
double plot_imag[INPUT_MAX_DATA];
double module[INPUT_MAX_DATA];
double arg[INPUT_MAX_DATA];
double x_axis[INPUT_MAX_DATA];

gnuplot_ctrl *realplot, *imagplot,*modulplot, *argplot, *constplot; 

//Module Defined Parameters
int block_length=2048;
int run_times=1;
char modname[128]="sink_graph";
float samplingfreqHz=48000.0;										
int plot_mode=0x04; 	/* 	0x0: None;							
							0x1: Time;
							0x2: Spectrum;
							0x4: Constellation
						*/
fftw_plan fft_plan;
int fftsize=1024*4;												
int numsampl2view;
int averagefft=0;		/* 0: FFT non averaged; 1: FFT averaged*/	
float mu=0.9;			/* Persistence: Contribution of history in current average*/
#define NUMCAPTURES		256
int numcaptures=NUMCAPTURES;	


fftw_complex fftbuff_IN[INPUT_MAX_DATA*NUMCAPTURES];
fftw_complex fftbuff_OUT[INPUT_MAX_DATA*NUMCAPTURES];
int logscale=1;			/* 0: Linear scale; 1: Log Scale*/		
double arg_scale;

struct timespec tdata[3];

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {
	int i;
	
//	param_get_string("modname", &modname[0]);
//	printf("MODNAME=%s\n", modname);
	param_get_int("numsampl2view", &numsampl2view);
	param_get_int("plot_mode", &plot_mode);
	param_get_float("samplingfreqHz", &samplingfreqHz);
	param_get_int("fftsize", &fftsize);
	param_get_int("averagefft", &averagefft);
	param_get_float("mu", &mu);
	param_get_int("logscale", &logscale);
	param_get_int("numcaptures", &numcaptures);




	//Check Configuration Values
	if(fftsize > (INPUT_MAX_DATA*NUMCAPTURES))
		printf("Module %s ERROR: fftsize=%d must be lower than INPUT_MAX_DATA*NUMCAPTURES=%d\n", 
				modname, fftsize, INPUT_MAX_DATA*NUMCAPTURES);


	//Plotting Time
	if((plot_mode & 0x01) == 0x01){
//		printf("Plotting Time: %x\n", plot_mode & 0x01);
		realplot = gnuplot_init() ;
		gnuplot_setstyle(realplot,"lines");
		gnuplot_set_background(realplot, "gold"); 
		gnuplot_set_xlabel(realplot, "Time [s]");

		imagplot = gnuplot_init() ;
		gnuplot_setstyle(imagplot,"lines");
		gnuplot_set_background(imagplot, "gold");
		gnuplot_set_xlabel(imagplot, "Time [s]");

		numsampl2view=fftsize;
		for(i=0; i<numsampl2view; i++)x_axis[i]=((double)i)/(samplingfreqHz); ///(double)fftsize)*(double)i;
	}

	//Plotting Spectrum
	if((plot_mode & 0x02) == 0x02){
//		printf("Plotting Spectrum: %x\n", plot_mode & 0x02);
		modulplot = gnuplot_init() ;
		gnuplot_setstyle(modulplot,"lines");
		if(averagefft==0)gnuplot_set_background(modulplot, "greenyellow"); 
		if(averagefft==1)gnuplot_set_background(modulplot, "#FFF8DC"); 
		
		gnuplot_set_xlabel(modulplot, "Frequency [Hz]");
		if(logscale==0)gnuplot_set_ylabel(modulplot, "Magnitude [Linear]");
		if(logscale==1)gnuplot_set_ylabel(modulplot, "Magnitude [dB]");	
		
		argplot = gnuplot_init() ;
		gnuplot_setstyle(argplot,"lines");
		if(averagefft==0)gnuplot_set_background(argplot, "greenyellow");
		if(averagefft==1)gnuplot_set_background(argplot, "#FFF8DC");
		gnuplot_set_ylabel(argplot, "Phase [rad]");
		gnuplot_set_xlabel(argplot, "Frequency [Hz]");

		for(i=0; i<fftsize; i++)x_axis[i]=(samplingfreqHz/(double)fftsize)*(double)i;
		
		//Create fftw3 plan
		fft_plan = fftw_plan_dft_1d(fftsize, fftbuff_IN, fftbuff_OUT, FFTW_FORWARD, FFTW_ESTIMATE);
		arg_scale = 180/PI;
	}

	//Plotting Constellation
	if((plot_mode & 0x04) == 0x04){
		constplot = gnuplot_init() ;
		gnuplot_setstyle(constplot,"dots");
		gnuplot_set_background(constplot, "#00FFFF"); 
		gnuplot_set_xlabel(constplot, "Inphase");
		gnuplot_set_ylabel(constplot, "Quadrature");
		numsampl2view=fftsize;
		for(i=0; i<numsampl2view; i++)x_axis[i]=((double)i)/(samplingfreqHz); 
	}


	/* Print Module Init Parameters */
	printf("O--------------------------------------------------------------------------------------------O\n");
	printf("O    SPECIFIC PARAMETERS SETUP: \033[1;34m%s\033[0m\n", modname);
	printf("O      Nof Inputs=%d, DataTypeIN=%s, Nof Outputs=%d, DataTypeOUT=%s\n", 
		       NOF_INPUT_ITF, IN_TYPE, NOF_OUTPUT_ITF, OUT_TYPE);
	printf("O      plot_mode=%d  ==> [0: None; 1: Time; 2: Spectrum; 4: Constellation]\n", plot_mode);
	printf("O      numsampl2view=%d, samplingfreqHz=%3.1f\n", numsampl2view, samplingfreqHz);
	printf("O      fftsize=%d, averagefft=%d, logscale=%d, mu=%3.1f, averagefft=%d,\n", 
				   fftsize, averagefft, logscale, mu, averagefft);
	printf("O      numcaptures=%d\n", numcaptures);
	
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
	int i;
	static int tslot=0, num_INTIMEdades=0, num_INFFTdades=0, FFTplotEnabled=0, TIMEplotEnabled=0;
	static int tslot2=0;
	//static int numcaptures=NUMCAPTURES;


//	printf("SINK IN pout0=%u\n", inp);
//	clock_gettime(CLOCK_MONOTONIC,&tdata[1]);

/*	if(tslot2 < 2){
		if(tslot2==0)get_time(&tdata[1]);
		if(tslot2==1){
			get_time(&tdata[2]);
			get_time_interval(tdata);
			printf("TSLOT = %d s, %d ns\n", tdata[0].tv_sec, tdata[0].tv_nsec);
			if(tdata[0].tv_sec) printf("WARNING!!! TimeSlot > 1 second. Maybe graph refresh fails\n");
			numcaptures=1000000/tdata[0].tv_nsec;
		}
		tslot2++;
	}
*/
//	printf("sink_graph.c: rcv-samples=%d, inp=%u\n", rcv_samples, inp);

	if (rcv_samples > INPUT_MAX_DATA) {
			modinfo_msg("Truncating input %d to %d samples\n",rcv_samples, INPUT_MAX_DATA);
			rcv_samples = INPUT_MAX_DATA;
	}
	
	// CAPTURE DATA IN
	// TO TIME PLOTS
	if(num_INTIMEdades < numsampl2view){
		for(i=0; i<rcv_samples;i++){
			plot_real[num_INTIMEdades+i] = (double) __real__ inp[i]; 
			plot_imag[num_INTIMEdades+i] = (double) __imag__ inp[i];
		}
		num_INTIMEdades += rcv_samples;
	}
	// TO FFT PLOTS
	if(num_INFFTdades < fftsize){
		for(i=0; i<rcv_samples;i++){
			fftbuff_IN[num_INFFTdades+i]=inp[i];
		}
		num_INFFTdades += rcv_samples;
	}
	// PLOTTING TIME
	if(TIMEplotEnabled==1){
		TIMEplotEnabled=0;
		// Plotting Time 
		if((plot_mode & 0x01) == 0x01){
//			printf("Tslot=%d, PLOT\n", tslot);
			gnuplot_resetplot(realplot);
			gnuplot_plot_xy(realplot, x_axis, plot_real, numsampl2view, "Real");
			gnuplot_resetplot(imagplot);
			gnuplot_plot_xy(imagplot, x_axis, plot_imag, numsampl2view, "Imag");
		}
		// Plotting Constellation 
		if((plot_mode & 0x4) == 0x4){
			//Plotting
			gnuplot_resetplot(constplot);
			gnuplot_plot_xy(constplot, plot_imag, plot_real, numsampl2view, "Constellation");
		}
	}

	if(FFTplotEnabled==1){
		FFTplotEnabled=0;
		num_INFFTdades=0;

		/// Plotting Spectrum 
		if((plot_mode & 0x02) == 0x02){
			// Perform FFT
			fftw_execute(fft_plan);

			//Process Magnitude of Data
			//Non Log scale
			if(logscale==0){
				for (i=0; i<fftsize; i++){
					if(averagefft==0)module[i] = (float)(cabs(fftbuff_OUT[i]));
					if(averagefft==1)module[i] = (1.0-(double)mu)*((float)(cabs(fftbuff_OUT[i]))) + (double)mu*module[i];
				}
			}
			//Log scale
			if(logscale==1){
				for (i=0; i<fftsize; i++){
					if(averagefft==0)module[i] = (float)(20.0*log10(cabs(fftbuff_OUT[i])));
					if(averagefft==1)module[i] = (1.0-(double)mu)*((float)(20.0*log10(cabs(fftbuff_OUT[i])))) + (double)mu*module[i];
				}
			}		
			//Process Arg of Data
			arg_scale=1.0;
			for (i=0; i<fftsize; i++){
				if(averagefft==0)arg[i] = (double)(arg_scale*cargf(fftbuff_OUT[i]));
				if(averagefft==1)arg[i] = (1.0-(double)mu)*((double)(arg_scale*cargf(fftbuff_OUT[i]))) + (double)mu*arg[i];
			}
			//Plotting
			gnuplot_resetplot(modulplot);
			gnuplot_plot_xy(modulplot, x_axis, module, fftsize, "Magnitude");
			gnuplot_resetplot(argplot);
			gnuplot_plot_xy(argplot, x_axis, arg, fftsize, "Phase");
		}
	}



	tslot++;
//	printf("SINKGRAPH.c: Tslot=%d, numcaptures=%d, num_INTIMEdades=%d, numsamples2view=%d\n", tslot, numcaptures, num_INTIMEdades, numsampl2view);
	if(tslot==numcaptures){
		tslot=0;
		if(num_INTIMEdades >= numsampl2view){
			TIMEplotEnabled=1;
			num_INTIMEdades=0;
//			printf("TIMEplotEnabled=%d\n", TIMEplotEnabled);
		}
		if(num_INFFTdades >= fftsize){
			FFTplotEnabled=1;
		}
	}
//	printf("num_INTIMEdades=%d, tslot=%d, TIMEplotEnabled=%d\n", num_INTIMEdades, tslot, TIMEplotEnabled);
	rcv_samples=0;
	return rcv_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}


