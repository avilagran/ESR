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

#ifndef ITF_TYPES_H
#define ITF_TYPES_H

#ifndef SKELETON_H
#include "skeleton.h"
#endif

/* input and output data types */
typedef float _Complex  input_t;
typedef float _Complex  output_t;

#ifdef INTERFACE_CONFIG

/* Input and output buffer sizes (in number of samples) */
const int input_max_samples = 16*1024;
const int output_max_samples = 16*1024;

/* leave these two lines unmodified */
const int input_sample_sz = sizeof(input_t);
const int output_sample_sz = sizeof(output_t);

/* Number of I/O interfaces. All have the same maximum size */
const int nof_input_itf = 1;
const int nof_output_itf = 0;

#endif


#endif
