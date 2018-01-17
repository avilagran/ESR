/* 
 * Copyright (c) 2012
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


#ifndef _MOD_PARAMS_H
#define _MOD_PARAMS_H

/* oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo */
/* Define the module control parameters & control structures and configure default values*/
/* Do not initialize here the extern variables*/
extern int run_times;
extern int block_length;
extern char plot_modeIN[];
extern char plot_modeOUT[];
extern float samplingfreqHz;
extern _Complex float bufferA[2048];
extern float pulse[2048*4];
extern int opmode;
extern char filtercoefffile[];
extern float Afreq; 
extern int modephase;
/* oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo */
/* Configure the control parameters */
#define NOF_PARAMS	11

param_t parameters[] = {
		{"block_length", 	/* Parameter name*/
		  INT,			/* Parameter type (see skeleton.h) */
		  1},			/* Number of samples (of size the type) */
		{"run_times",INT,1},
		{"plot_modeIN",STRING,1},
		{"plot_modeOUT",STRING,1},
		{"pulselength",INT,1},
		{"gain",FLOAT,1},
		{"samplingfreqHz",FLOAT,1},
		{"opmode",INT,1},
		{"threshold",FLOAT,1},
		{"freq_step",FLOAT,1},
		{"modephase",INT,1},
		{NULL, 0, 0} /* need to end with null name */
};


/* This number MUST be equal or greater than the number of parameters*/
const int nof_parameters = NOF_PARAMS;
/* oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo */


#endif
