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


/* Configure the control variables */
extern int run_times;
extern int block_length;
extern int numsampl2view;
extern int plot_mode;
extern float samplingfreqHz;
extern int fftsize;
//extern int averagedfft;
extern float mu;
extern int logscale;
extern int averagefft;
extern char modname[];
extern int numcaptures;


param_t parameters[] = {
//		{"modname",STRING,1},
		{"numsampl2view",INT,1},
		{"plot_mode",INT,1},
		{"samplingfreqHz",FLOAT,1},
		{"fftsize",INT,1},
//		{"averagedfft",INT,1},
		{"averagefft",INT,1},
		{"mu",FLOAT,1},
		{"logscale",INT,1},
		{"numcaptures",INT,1},
		{NULL, 0, 0} /* need to end with null name */
};

/* This number MUST be equal or greater than the number of parameters*/
const int nof_parameters = 9;
