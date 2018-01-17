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

#define BPSK    1
#define QPSK    2
#define QAM16   4
#define QAM64   6


/* The symbols are specified in [3GPP TS 36.211 version 10.5.0 Release 10, Section 7] */
/* from which the symbols levels follow: */
#define BPSK_LEVEL      1/sqrt(2)

#define QPSK_LEVEL      1/sqrt(2)

#define QAM16_LEVEL_1	1/sqrt(10)
#define QAM16_LEVEL_2	3/sqrt(10)

#define QAM64_LEVEL_1	1/sqrt(42)
#define QAM64_LEVEL_2	3/sqrt(42)
#define QAM64_LEVEL_3	5/sqrt(42)
#define QAM64_LEVEL_4	7/sqrt(42)

#define QAM64_LEVEL_x	2/sqrt(42)		/* this is not an QAM64 level, but, rather, an auxiliary value that can be used for computing the symbol from the bit sequence */

/* Thresholds for Demodulation */
/* Assume perfect amplitude and phase alignment.
 *  Check threshold values for real case
 *  or implement dynamic threshold adjustent as a function of received symbol amplitudes */
#define QAM16_THRESHOLD		2/sqrt(10)
#define QAM64_THRESHOLD_1	2/sqrt(42)
#define QAM64_THRESHOLD_2	4/sqrt(42)
#define QAM64_THRESHOLD_3	6/sqrt(42)


/* Moduation Mapper/Demapper functions
 * INIT phase */
//void set_BPSKtable(void);
//void set_QPSKtable(void);
void set_16QAMtable(void);
//void set_64QAMtable(void);

int get_bits_per_symbol(int modulation);

//void modulate_BPSK_real(char *bits, float *S_out);
//void modulate_BPSK(char *bits, float *S_out);
//void modulate_QPSK(char *bits, _Complex float *S_out);
int modulate_16QAM(char *bytes, _Complex float *S_out, int num_bytes);
//void modulate_64QAM(char *bits, _Complex float *S_out);
#endif
