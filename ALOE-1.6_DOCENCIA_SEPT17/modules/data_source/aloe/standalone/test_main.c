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

/* Testsuite for testing DFT and IDFT */
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "phal_sw_api.h"
#include "skeleton.h"
#include "params.h"
#define INTERFACE_CONFIG
#include "data_source_interfaces.h"
#undef INTERFACE_CONFIG
#include "data_source_params.h"
#include "data_source.h"
#include "rtdal_datafile.h"
#include "test_generate.h"
#include "gnuplot_i.h"
#include "complexplot.h"
#include "graphs_mode.h"
#include "plot_mode.h"

//#define STRLENGTH	32

FILE *dat_input=NULL, *dat_output=NULL;
char *dat_input_name=NULL, *dat_output_name=NULL;

extern const int input_sample_sz;
extern const int output_sample_sz;
extern const int nof_input_itf;
extern const int nof_output_itf;
extern const int input_max_samples;
extern const int output_max_samples;
extern char mname[];


static input_t *input_data;
static output_t *output_data;
static void **input_ptr, **output_ptr;

static int *input_lengths;
static int *output_lengths;

//static double *plot_buff;

int use_gnuplot=0;		/*0: no plot; 1:plot*/
int use_binary_input=-1;	/*-1: No file, 0: text input; 1: binary input*/
int use_binary_output=-1;	/*-1: No file, 0: text input; 1: binary input*/
int show_help=0;

typedef struct {
	char *name;
	char *value;
} saparam_t;

saparam_t *parametersA;
int nof_params;

int parse_paramters(int argc, char**argv);
int param_setup(char *name, char *value, int nofdefparams);

inline int get_input_samples(int idx) {
	return input_lengths[idx];
}

inline void set_output_samples(int idx, int len) {
	output_lengths[idx] = len;
}

inline int get_input_max_samples() {
	return input_max_samples;
}
inline int get_output_max_samples() {
	return output_max_samples;
}

//CHANGE 10022015
void allocate_memory() {
	int aux;
	

	aux=nof_input_itf;
	if(nof_input_itf <= 0)aux=nof_input_itf+1;
	posix_memalign((void**)&input_data,64,input_max_samples*nof_input_itf*input_sample_sz);
	assert(input_data);
	input_lengths = calloc(sizeof(int),aux);
	assert(input_lengths);
	input_ptr = calloc(sizeof(void*),aux);
	assert(input_ptr);

	aux=nof_output_itf;
	if(nof_output_itf <= 0)aux=nof_output_itf+1;
	posix_memalign((void**)&output_data,64,output_max_samples*nof_output_itf*output_sample_sz);
	assert(output_data);
	output_lengths = calloc(sizeof(int),aux);
	assert(output_lengths);
	output_ptr = calloc(sizeof(void*),aux);
	assert(output_ptr);

/*	input_data = malloc(sizeof(input_t)*input_max_samples*nof_input_itf);
	assert(input_data);
	output_data = malloc(sizeof(output_t)*output_max_samples*nof_output_itf);
	assert(output_data);
	input_lengths = malloc(sizeof(int)*nof_input_itf);
	assert(input_lengths);
	output_lengths = malloc(sizeof(int)*nof_output_itf);
	assert(output_lengths);
*/
}

//CHANGE 10022015

void free_memory() {
	free(input_data);
	free(output_data);
	free(input_lengths);
	free(output_lengths);
	if (parametersA) {
		for (int i=0;i<nof_params;i++) {
			if (parametersA[i].name) free(parametersA[i].name);
			if (parametersA[i].value) free(parametersA[i].value);
		}
		free(parametersA);
	}
}

int get_time(struct timespec *x) {

	if (clock_gettime(CLOCK_MONOTONIC,x)) {
		return -1;
	}
	return 0;
}

void get_time_interval(struct timespec * tdata) {

	tdata[0].tv_sec = tdata[2].tv_sec - tdata[1].tv_sec;
	tdata[0].tv_nsec = tdata[2].tv_nsec - tdata[1].tv_nsec;
	if (tdata[0].tv_nsec < 0) {
		tdata[0].tv_sec--;
		tdata[0].tv_nsec += 1000000000;
	}
}

/**
 * Testsuite:
 * Generates random symbols and calls the module ....
 * @param ...
 * @param ...
 * @param ... */
int main(int argc, char **argv)
{
	struct timespec tdata[3];
	gnuplot_ctrl *plot;
	/* Complex plot structure definition */
//	SComplexplot complexplot_ctrl;
	char tmp[64];
	int ret, i, j, n;
	float *tmp_f;
	char *aux_c;
	_Complex float *tmp_c;
	double *plot_buff_r;
	double *plot_buff_c;
	char *datatype="BIN";
	char *inputmode="FILE";
	int file_read_sz=0;
	int out_length0;

	parametersA = NULL;

	allocate_memory();
	param_init(parameters,nof_parameters);
	parse_paramters(argc, argv);

	if(show_help==0)printf("Put -h for standalone execution options!!!\n");
	if(show_help==1){
		printf("o STANDALONE EXECUTION ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
		printf("o Two alternatives: a)Autogenerated input data, b)Load input file.                   o\n");
		printf("o Options: -p:  plot input/output                                                    o\n");
		printf("o          -it: input text file according module input data type format.             o\n");
		printf("o          -ib: input binari file according module input data type format.           o\n");
		printf("o          -ot: output text file according module output data type format.           o\n");
		printf("o          -ob: output binari data file according module output data type format.    o\n");
		printf("o          block_length=XX: Number of samples sent to input each time slot           o\n");
		printf("o          run_times=XX: Number of time slot executed                                o\n");
		printf("o          plot_mode=XX: plotting options                                            o\n");
		printf("o Autogenerated input data example:                                                  o\n");
		printf("o     ./module_name -p block_length=10 run_times=2                                   o\n");
		printf("o Load input data example:                                                           o\n");
		printf("o     ./module_name -p -ib ../../../LTEcaptures/aloe.dat block_length=10 run_times=1 o\n");
		printf("o Plotting Options  (XX=IN o XX=OUT)                                                 o\n");
		printf("o          plot_modeXX=C1WLS: Real-Imaginary, 1 Window, Linear, Single frame         o\n");
		printf("o          plot_modeXX=C2WLS: Real + Imaginary (2 Windows); Linear, Single frame.    o\n");
		printf("o          plot_modeXX=CFFTMF: Complex, FFT, Multiple Frames.                        o\n");
		printf("o          plot_modeXX=CFFTSF: Complex, FFT, Magnitude & Argument, Single Frame.     o\n");
		printf("o          plot_modeXX=CCSF: Complex, Constellation. Single Frame.                   o\n");
		printf("o          plot_modeXX=DEFAULT: User defined during module init phase                o\n");
		printf("o Plot example with autogenerated data:                                              o\n");
		printf("o     ./module_name -p block_length=10 run_times=2 plot_mode=CFFTSF                  o\n");
		printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
		return(0);
	}
	printf("Capture command line parameteroooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	//Capture command line parameters
	for(i=0; i<nof_params; i++){
//		printf("names=%s\n", parametersA[i].name);
		param_setup(parametersA[i].name, parametersA[i].value, nof_parameters);
	}
	//Call Initialize() function
//	printf("Initialize module oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	if (initialize()) {
		printf("Error initializing\n");
		exit(1); /* the reason for exiting should be printed out beforehand */
	}
	printf("Generate Input Signalsoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	//OPEN INPUT AND OUTPUT FILES, if needed
	if (dat_input_name) {
//		printf("dat_input_name = %s\n",dat_input_name);			/*LINE ADDED*/
//		printf("0____dat_input=%d\n", dat_input);
		dat_input = rtdal_datafile_open(dat_input_name, "r");
//		printf("1____dat_input=%d\n", dat_input);
		if (!dat_input) {
			printf("Error opening mat file %s\n",dat_input_name);
			exit(1);
		}
	}
	if (dat_output_name) {
		//printf("dat_output_name = %s\n",dat_output_name);			/*LINE ADDED*/
		dat_output = rtdal_datafile_open(dat_output_name, "w");
		if (!dat_output) {
			printf("Error opening mat file %s\n",dat_output_name);
			exit(1);
		}
	}
	//CHECK INPUT MODE: Generate signal or read signal file
//	printf("0___file_read_sz=%d, block_length=%d, run_times=%d\n", file_read_sz, block_length,run_times);
	if (param_get_int("block_length",&file_read_sz)) {
		if(run_times>1)file_read_sz*=run_times;
		if(run_times<1){
			moderror_msg("ERROR: run_times=%d < 1\n", run_times);
			exit(1);
		}	
	} else {
		file_read_sz=input_max_samples;
		run_times=1;
	}
	if(file_read_sz > input_max_samples){
		printf("ERROR: %s.test_main() input data=%d > INPUT_MAX_DATA=%d\n", 
							mname, file_read_sz, INPUT_MAX_DATA);
		exit(1);
	}
//	printf("1___file_read_sz=%d, block_length=%d, run_times=%d\n", file_read_sz, block_length,run_times);

	if (dat_input) {
		inputmode="INPUT FILE";
//		printf("INPUT MODE=%s, input_sample_sz=%d\n", inputmode, input_sample_sz);
		if(use_binary_input==1){ //Binari
			printf("Reading input file as chars: TO BE CHECKED\n");
			n = rtdal_datafile_read_bin(dat_input,input_data,file_read_sz*input_sample_sz);
			input_lengths[0] = n/input_sample_sz;
//			read_length0=input_lengths[0];
			if(input_sample_sz==sizeof(char))datatype="BIN CHAR";
			if(input_sample_sz==sizeof(float))datatype="BIN FLOAT";
			if(input_sample_sz==sizeof(_Complex float))datatype="BIN COMPLEX FLOAT";
		}
		if(use_binary_input==0){ //Text 
			if(input_sample_sz==sizeof(char)){
				printf("Reading input file as chars: TO BE CHECKED\n");
				n = rtdal_datafile_read_bin(dat_input,(char *)input_data,file_read_sz);
				//printf("Char n=%d\n", n);
				datatype="TEXT CHAR";
			}
			if(input_sample_sz==sizeof(float)){
//				printf("Reading input file as floats: TO BE CHECKED\n");
				n = rtdal_datafile_read_real(dat_input,(float *)input_data,file_read_sz);
				//printf("Float n=%d\n", n);
				datatype="TEXT FLOAT";
			}
			if(input_sample_sz==sizeof(_Complex float)){
				printf("Reading input file as Complex floats: CHECKED\n");
//				printf("file_read_sz=%d\n", file_read_sz);
				n = rtdal_datafile_read_complex(dat_input,(_Complex float *)input_data,file_read_sz);
//				printf("Complex n=%d, pointer input_data=%u\n", n, input_data);
				datatype="TEXT COMPLEX FLOAT";
			}
			input_lengths[0] = n;
			//CHECK IF ENOUGH DATA AVAILABLE
//			read_length0=input_lengths[0];
		}
		if(file_read_sz > input_lengths[0])file_read_sz = input_lengths[0];

		if (n == 0) {
			printf("Error reading file %s: Data length readed = %d\n",dat_input_name, n);
			exit(1);
		}
		if (n == -1) {
			printf("Error reading file %s\n",dat_input_name);
			exit(1);
		}
		printf("Readed %d bytes or %d samples from file %s\n",input_lengths[0],file_read_sz,dat_input_name);
	} else {
		inputmode="GENERATE INPUT";
//		printf("INPUT MODE=%s\n", inputmode);
		input_lengths[0]=file_read_sz;
//		printf("TEST_GENERATE: IN input_lengths[0]=%d\n", input_lengths[0]);
//CHANGE 10022015_1
		if(nof_input_itf > 0){
			if (generate_input_signal(input_data, input_lengths)) {
				printf("Error generating input signal\n");
				exit(1);
			}
		}
//CHANGE 10022015_2
//		printf("TEST_GENERATE: OUT input_lengths[0]=%d\n", input_lengths[0]);
	}
	printf("o Captured Config oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	printf("o dat_input_name = %s, dat_output_name = %s\n", dat_input_name,dat_output_name);
	printf("o inputmode=%s, datatype=%s, fileread_sz=%d \n", inputmode, datatype, file_read_sz);
	printf("o block_length=%d, run_times=%d\n", block_length, run_times);
	printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");

	printf("Check Input Buffers oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	for (i=0;i<nof_input_itf;i++) {
		if (!input_lengths[i]) {
			printf("Warning, input interface %d has zero length\n",i);
		}
	}
	printf("Clock Init ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	clock_gettime(CLOCK_MONOTONIC,&tdata[1]);

	printf("Execute STANDALONE Work oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
//	printf("block_length=%d, run_times=%d,input_sample_sz=%d\n", block_length, run_times, input_sample_sz);
	out_length0=0;
	input_lengths[0]=block_length;
	for (i=0;i<run_times;i++) {	
		ret = work(&input_data[i*block_length], &output_data[out_length0]);
		out_length0+=ret;
	}
	input_lengths[0] = block_length*run_times;
	output_lengths[0] = out_length0;

	printf("Capture time wasted oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");

	clock_gettime(CLOCK_MONOTONIC,&tdata[2]);
	stop();
	if (ret == -1) {
		printf("Error running\n");
		exit(-1);
	}
	get_time_interval(tdata);
	printf("Execution time: %d ns.\n", (int) tdata[0].tv_nsec);
	printf("Check Output Buffers ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
//	printf("Nof nof_output_itf=%d\n", nof_output_itf);
	for (i=0;i<nof_output_itf;i++) {
		if (!output_lengths[i]) {
			output_lengths[i] = ret;
		}
		if (!output_lengths[i]) {
			printf("Warning output interface %d has zero length\n",i);
		}
	}
	printf("Input/Output Plot oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	printf("Only Valid for one output and one input oooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	if (use_gnuplot) {
		for (i=0;i<nof_input_itf;i++) {
			if(input_lengths[i]==0)printf("WARNING!!! test_main(): input_lengths[%d]=%d\n", i, input_lengths[i]);
			plot_buff_r = malloc(sizeof(double)*input_lengths[i]);
			plot_buff_c = malloc(sizeof(double)*input_lengths[i]);
//			printf("input_length[%d]=%d\n", i, input_lengths[i]);
			if (input_sample_sz == sizeof(char)) {
				
				aux_c = (char*) &input_data[i*input_max_samples*input_sample_sz];
				for (j=0;j<input_lengths[i];j++) {
					plot_buff_r[j] = (double) (aux_c[j]);// & (char)0xFF);
				}
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"input_%d",i);
		        	gnuplot_plot_x(plot, plot_buff_r, get_input_samples(i), tmp);
		        	free(plot_buff_r);

			}else if (input_sample_sz == sizeof(float)) {
				tmp_f = (float*) &input_data[i*input_max_samples*input_sample_sz];
				for (j=0;j<input_lengths[i];j++) {
					plot_buff_r[j] = (double) tmp_f[j];
				}
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"input_%d",i);
		        	gnuplot_plot_x(plot, plot_buff_r, get_input_samples(i), tmp);
		        	free(plot_buff_r);
			}else if (input_sample_sz == sizeof(_Complex float)) {
				tmp_c = (_Complex float*) &input_data[i*input_max_samples*input_sample_sz];
///////////////////////////////////
//				printf("samplingfreqHz=%3.3f, block_length=%d, runtimes=%d\n", samplingfreqHz, block_length, run_times);
//				printf("plot_mode=%s, datalen=%d\n", plot_mode, input_lengths[i]);

				if(strcmp(plot_modeIN, "DEFAULT") != 0){
					plotcomplex(plot_modeIN, tmp_c, input_lengths[i], plot, "Input", &complexplot_ctrl, samplingfreqHz, run_times);
				}else {
					printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
					printf("WARNING!!!: Plot default configuration defined at 'graphs_mode.h' setup.                     o\n");
					printf("Please, modify according your needs.                                                         o\n");
					printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
				}
				complexplot(tmp_c, input_lengths[i], plot, "Input", &complexplot_ctrl);
//////////////////////////////////
/*ISMAEL			for (j=0;j<input_lengths[i];j++) {
					plot_buff_r[j] = (double) __real__ tmp_c[j];
					plot_buff_c[j] = (double) __imag__ tmp_c[j];
				}
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"input_real_%d",i);
		       	 	gnuplot_plot_x(plot, plot_buff_r, get_input_samples(i), tmp);
		       		plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"input_imag_%d",i);
		        	gnuplot_plot_x(plot, plot_buff_c, get_input_samples(i), tmp);
*/
		        	free(plot_buff_r);
		        	free(plot_buff_c);

			}

		}
		for (i=0;i<nof_output_itf;i++) {
			if(output_lengths[i]==0)printf("WARNING!!! test_main(): output_lengths[%d]=%d\n", i, output_lengths[i]);
//			printf("nof_outputs=%d\n", nof_output_itf);
			plot_buff_r = malloc(sizeof(double)*output_lengths[i]);
			plot_buff_c = malloc(sizeof(double)*output_lengths[i]);
			if (output_sample_sz == sizeof(float)) {
//				printf("PLOT FLOATS\n");
				tmp_f = (float*) &output_data[i*output_max_samples*output_sample_sz];
				for (j=0;j<output_lengths[i];j++) {
					plot_buff_r[j] = (double) __real__ tmp_f[j];
				}
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"output_%d",i);
				gnuplot_plot_x(plot, plot_buff_r,
				output_lengths[i], tmp);
				free(plot_buff_r);

			} else if (output_sample_sz == sizeof(_Complex float)) {
//				printf("PLOT COMPLEX FLOATS, output_lengths[%d]=%d\n", i, output_lengths[i]);
				tmp_c = (_Complex float*) &output_data[i*output_max_samples*output_sample_sz];
///////////////////////////////////
//				printf("samplingfreqHz=%3.3f, block_length=%d, runtimes=%d\n", samplingfreqHz, block_length, run_times);
//				printf("plot_mode=%s, datalen=%d\n", plot_mode, input_lengths[i]);

				if(strcmp(plot_modeOUT, "DEFAULT") != 0){
					plotcomplex(plot_modeOUT, tmp_c, input_lengths[i], plot, "Output", &complexplot_ctrl, samplingfreqHz, run_times);
				}else {
					printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
					printf("WARNING!!!: Plot default configuration defined at 'graphs_mode.h' setup.                     o\n");
					printf("Please, modify according your needs.                                                         o\n");
					printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
				}
				complexplot(tmp_c, input_lengths[i], plot, "Output", &complexplot_ctrl);
//////////////////////////////////
/*				for (j=0;j<output_lengths[i];j++) {
					plot_buff_r[j] = (double) __real__ tmp_c[j];
					plot_buff_c[j] = (double) __imag__ tmp_c[j];
				}
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"output_real_%d",i);
				gnuplot_plot_x(plot, plot_buff_r,
						output_lengths[i], tmp);
				plot = gnuplot_init() ;
				gnuplot_setstyle(plot,"lines");
				snprintf(tmp,64,"output_imag_%d",i);
				gnuplot_plot_x(plot, plot_buff_c,
				output_lengths[i], tmp);
*/
				free(plot_buff_r);
				free(plot_buff_c);

	        	}
		}

		printf("Type ctrl+c to exit\n");fflush(stdout);
		free_memory();
		pause();
		/* make sure we exit here */
		exit(1);
	}

	free_memory();

	return 0;

}



/* Define test environment functions here */
int parse_paramters(int argc, char**argv)
{
	int i;
	char *key,*value;
	int k = 0;

	nof_params = argc-1;

//printf("nof_params=%d\n", nof_params);
	for (i=1;i<argc;i++) {
		if (!strcmp(argv[i],"-p")) {		//Plot input/output option
			use_gnuplot = 1;
			nof_params--;
//			printf("-p\n");
		} else if (!strcmp(argv[i],"-it")) {	//Input text file
			dat_input_name = argv[i+1];
			i++;
			nof_params-=2;
			use_binary_input=0;
//			printf("-it\n");
		} else if (!strcmp(argv[i],"-ib")) {	//Input binari file
			dat_input_name = argv[i+1];
			use_binary_input = 1;
			i++;
			nof_params-=2;
//			printf("-ib\n");
		}else if (!strcmp(argv[i],"-ot")) {	//Output text file
			dat_output_name = argv[i+1];
			i++;
			nof_params-=2;
			use_binary_output=0;
//			printf("-ot\n");
		}else if (!strcmp(argv[i],"-ob")) {	//Output binari file
			dat_output_name = argv[i+1];
			i++;
			nof_params-=2;
			use_binary_output=1;
//			printf("-ob\n");
		}else if (!strcmp(argv[i],"-h")) {	//Show Help
			nof_params--;
			show_help=1;
//			printf("-help\n");
		}
	
	}
	if (!nof_params) {
		return 0;
	}

	parametersA = calloc(sizeof(saparam_t),nof_params);

	for (i=1;i<argc;i++) {
		if (strcmp(argv[i],"-p")) {
			key = argv[i];
			value = index(argv[i],'=');
			if (value) {
				*value = '\0';
				value++;
				parametersA[k].name = strdup(key);
				parametersA[k].value = strdup(value);
/*				printf("name=%s\n", parametersA[k].name);
				printf("value=%s\n", parametersA[k].value);
				printf("value=%d\n", atoi(parametersA[k].value));
				printf("value=%3.3f\n", atof(parametersA[k].value));
*/
				k++;
			}
		}
	}
	return 0;
}

/*INITIALIZE params with captured values*/
int param_setup(char *name, char *value, int nofcaptparams){

	int i, type, idx;
	void *param=NULL;

	//Check if captured params were defined in data_source_params.h
//	printf("param_setup(): name=%s\n", name);
	param=param_get_addr(name);

	idx=param_get_idx(name);
	
//	printf("param_adr0=%u, idx=%d, para_adr1=%u\n", (unsigned int)param, idx, param_get_addr_i(idx));

	if(param==NULL){
		printf("ERROR: Param '%s' not included in parameters[] list of data_source_params.h      :-Ç!\n", name);
		for(i=0; i<3; i++){
			printf("Please, do not use param '%s' in command line                                    :-Ç!\n", name);
		}
		return(0);
	}
	else{
//		printf("Param %s initialized by command line\n", name);
	}
//	printf("param_adr=%d\n", param);
	type=param_get_aloe_type(name);
//	printf("type=%d\n", type);
	switch(type) {
		case STAT_TYPE_INT:
//			printf("INT\n");
			*((int*)param)=atoi(value);
//			printf("*param=%d\n", *((int*)param));
			return 1;
		case STAT_TYPE_FLOAT:
//			printf("FLOAT\n");
			*((float*)param)=atof(value);
//			printf("*param=%3.3f\n", *((float*)param));
			return 1;
		case STAT_TYPE_COMPLEX:
//			printf("COMPLEX\n");
			printf("Warning complex parameters not yet supported\n");
			return 1;
		case STAT_TYPE_STRING:
//			printf("STRING\n");
//			printf("*param=%d\n", param);	
//			len=strlen(value);
//			printf("len=%d\n", len);
			strcpy((char *)param, value);
//			printf("*param=%s\n", param);			
			return 1;
		default:
			printf("DEFAULT\n");
			printf("%s.params_setup(): Error. Default ", "sink_plot");
			return -1;
	}
	return(1);
}





