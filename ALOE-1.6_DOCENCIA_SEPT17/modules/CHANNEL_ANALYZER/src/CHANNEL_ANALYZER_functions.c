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

#include "CHANNEL_ANALYZER_functions.h"

/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param datalength: Number of samples
 * @param pulselength: Length of the pulse generated.
 * @param gain: Value of the pulse.
 * @return -1 if error, the number of output data if OK

 */
int init_pulse_COMPLEX(_Complex float *input, int datalength, int pulselength, float gain){
	int i;

	for (i=0;i<datalength;i++) {
		if(i < pulselength)__real__ input[i]=gain*1.0;
		else __real__ input[i]=0.0;
		__imag__ input[i]=0.0;
	}
	return(1);
}


/**
 * @brief Defines the function activity.
 * @param .
 *
 * @param lengths Save on n-th position the number of samples generated for the n-th interface
 * @return -1 if error, the number of output data if OK

 */
int bypass_COMPLEX(_Complex float *input, int inlength, _Complex float *output)
{
	int i,outlength;

	for (i=0;i<inlength;i++) {
		__real__ output[i] = __real__ input[i];
		__imag__ output[i] = __imag__ input[i];
	}
	outlength=inlength;
	return outlength;
}

int init_toneCOMPLEX(_Complex float *table, int length, float ref_freq, float gain, float sampl_freq){
	int i;
	double arg=PIx2/((float)length);

	for (i=0;i<length;i++) {
		__real__ table[i]=gain*(float)cos(arg*(float)i);
		__imag__ table[i]=gain*(float)sin(arg*(float)i);
	}
	return(1);
}

int gen_toneCOMPLEX(_Complex float *func_out, _Complex float *tablel, int tablesz, 
					int datalen, float ref_freq, float tone_freq, float sampl_freq){
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

int gen_cosinus_REAL_COMPLEX(_Complex float *func_out, _Complex float *tablel, int tablesz,
					int datalen, float ref_freq, float tone_freq, float sampl_freq){
	int i, k=1;
	static int j1=0;
	float ref_freq_c;

	ref_freq_c=sampl_freq/(float)tablesz;
	k=(int)(tone_freq/ref_freq_c);
	for (i=0;i<datalen;i++) {
		__real__ func_out[i]=__real__  tablel[j1];
		__imag__ func_out[i]=__imag__  tablel[j1];
		j1 += k;
		if(j1>=tablesz)j1-=tablesz;
	}
	return(1);
}


// Detect if signal is over some threshold
int detect_rcv_data(_Complex float *in, int length, float threshold){

	int i;
	int numsample=0;
	for(i=0; i<length; i++){
		if((__real__ *(in+i)) >= threshold)numsample=1+i;	// if signal has been detected
		if((__imag__ *(in+i)) >= threshold)numsample=1+i;
		if(numsample>0){
			return(numsample);
		}
	}
	return 0; 	// If no signal detected
}


int delay(_Complex float *in, int length, float threshold, int Tslot, float samplingfreq){

	static int detected=0;

	if(detected==0){
		detected=detect_rcv_data(in, length, threshold);
		printf("\nNumber of Tslot delayed=%d\n", Tslot);
		printf("Number of samples delayed from Tslot origin=%d\n", detected);
		printf("Total Delay=%3.6f sec\n", ((float)(length*Tslot) + detected)/samplingfreq);
		if(detected > 0)return(Tslot);
	}
	return(0);
}

float get_CH_gaindBs(_Complex float *in, int length){
	int i;
	static int TSLOT=0;
	float MAX=-1000000.0,MIN=1000000.0, gaindBs, Amplitude;

	for(i=200; i<length; i++){
		if(MAX < __real__ in[i]){
			MAX=__real__ in[i];
		}
		if(MIN > __real__ in[i]){
			MIN=__real__ in[i];
		}
	}
	Amplitude=(MAX-MIN)/2.0;
	if(Amplitude >= 2.0){
		printf("WARNING!!! MICROPHONE IN SATURATION. RECEIVED LEVEL TOO HIGH\n");
		printf("MEASURE PERIOD=%d. PLEASE, REDUCE THE OUTPUT OR INPUT GAIN IN YOUR LAPTOP AUDIO CONTROLS\n", TSLOT);
	}
	gaindBs=(float)(20.0*log10(Amplitude));

	TSLOT++;
	return(gaindBs);
}




#define MAXSLOPE	0.01	

void correct_phase(float *phasein, int length, float *cphaseout, int modephase, float Afreq){

	int i;
	float slope=0.0;
	int signNN[1204*8];
	float group_delay=0.0;
	float Gdelay[1204*8];
	float addPI=3.141592;//modificat

	signNN[0]=1.0;
	for(i=1; i<length; i++){
		//printf("slope=%3.6f\n", phasein[i]-phasein[i-1]);
		if((phasein[i]-phasein[i-1]) > MAXSLOPE)signNN[i]=-1.0;
		else signNN[i]=1.0;
	}
	// Correct Phase
	cphaseout[0]=phasein[0];
	for(i=1; i<length; i++){
		cphaseout[i]=phasein[i]*signNN[i];
		cphaseout[i]=cphaseout[i]-(addPI/2);//modificat
	}

	if(modephase==1){
		// Calculate Group Delay for frequency step = 50Hz
		//ADD HERE GROUP DELAY CODE
	}

}

#define MAXPHASESZ	1204*8

void computeGroupDelay(float *phasein, int length, float *Gdelay, float Afreq){

	int i, k, n;
	float cphaseout[MAXPHASESZ];
	float addPI=0.0;



	if(length > MAXPHASESZ)printf("ERROR!!!: CHANNEL_ANALYZER_FUNCTIONS.computeGroupDelay() length > MAXPHASESZ\n");
	// Calculate Group Delay for frequency step = 50Hz
	//cphaseout[0]=0.0;
	//ADD HERE GROUP DELAY CODE
	for(i=1;i<length;i++){
	Gdelay[i]=-(1/(2*addPI))*(phasein[i]-phasein[i-1])/(Afreq);
	}
}



/* Calcula la fase derivada del valor en DC contingut
 * en les components I/Q obtingudes amb "complex_base_band"
 */
float get_phase(double dc_i, double dc_q)
{
	float ph;
//	double dc_i = (double)I, dc_q = (double)Q;
	
	if (dc_i==0)
	{
		if (dc_q>=0)
			ph=PI/2.0;
			
		else
			ph=-PI/2.0;
			
	}
	else if (dc_q==0)
	{
		if (dc_i>=0)
			ph=0.0;
		else
			ph=-PI;
	}
	else
	{
		ph=(float)atan((double)dc_q/(double)dc_i);
		if (dc_i<0)
		{
			if (dc_q<0)
				ph-=PI;
			else
				ph+=PI;
		}
	}
	return(ph);
}


float get_CH_phase_radA(_Complex float *localcos, _Complex float *input, int length, _Complex float *Cprod){

	int i;
	double prod_I[2048*10], prod_Q[2048*10];
	double aver_I = 0.0, aver_Q = 0.0;
	float phase;
	double a=0.0;
	double b=0.0;


	// Compute IQ componets of received signal (real signal)
	for(i=0; i<length;i++){
		prod_I[i] = (double)__real__ localcos[i]*(double)__real__ input[i];
		prod_Q[i] = ((double)(-(__imag__ localcos[i]))*(double)__real__ input[i]);

	}
	//ADD HERE YOUR PHASE COMPUTE CODE
	
	for(i=0; i<length; i++){
	a = a + prod_I[i];
	b = b + prod_Q[i];
	}

	aver_I = (a/(double)length);
	aver_Q = (b/(double)length);

	phase = get_phase(aver_I, aver_Q);

	return(phase);
}



