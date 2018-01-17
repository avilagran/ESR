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

#define PI 3.14159265358979323846
#define PSSLENGTH 	62	//Number of PSS Symbols in Zadoff-Chu sequence
#define PSSCELLID0 	25.0
#define PSSCELLID1 	29.0
#define PSSCELLID2 	34.0

/*int create_LTEspectrum(_Complex float *MQAMsymb,
					int in_length,
					_Complex float *out_spectrum,
					int out_length,
					float carrierfreq,
					float sampligfreq);
*/
int setPSS(int CellID, _Complex float *PSSsymb, int TxRxMode);
int genPSStime_seq(int cellID, int FFTsize, fftwf_complex  *PSS_time, int TxRxmode);
int genPSSfreq_seq(int phylayerID, int FFTsize, fftwf_complex  *PSS_freq, int TxRxmode);
int create_LTEspectrum(_Complex float *MQAMsymb, int FFTlength, int datalength, _Complex float *out_spectrum);
int sendzeros(int fftlength, _Complex float *outbuffer);
int sendzerosPSS(_Complex float *PSS, int fftlength, _Complex float *outbuffer);
int send_firstSUBFRAME(_Complex float *PSS, _Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer);
int send_dataSUBFRAME(_Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer);
int send_pssSUBFRAME(_Complex float *PSS, _Complex float *inbuffer, int fftlength, int datasize, _Complex float *outbuffer);

int init_functionA_COMPLEX(_Complex float *input, int length);
int functionA_COMPLEX(_Complex float *input, int lengths, _Complex float *output);
int init_functionB_FLOAT(float *input, int length);
int functionB_FLOAT(float *input, int lengths, float *output);


typedef struct buffer{
	int readIndex;
	int writeIndex;
	int buffsize;
	int occuplevel;	//Occupancy level of buffer for reading
	int roomlevel;	//Available room in buffer for writing
}buffctrl;

void initCbuff(buffctrl *bufferCtrl, _Complex float *bufferC, int buffersz, int firstpcketsz);
int writeCbuff(buffctrl *buffer, _Complex float *buffdata, _Complex float *in, int length);
int readCbuff(buffctrl *buffer,_Complex float *buffdata, _Complex float *out, int length);
int lookCbuff(buffctrl* buffer,_Complex float *buffdata, _Complex float *out, int length);



#endif
