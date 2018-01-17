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

#include <swapi_utils.h>
#include <phal_sw_api.h>

#define SLENGTH		32
#define DEBUGG		0
#define BYPASS		1
#define NORMAL		2


struct MODparams{
    int opMODE;
    int num_operations;
    float constant;
    char datatext[SLENGTH];
}oParam;


/*////////////////////////////////////////////////////////////////////////////////////////////////////*/
/* PARAMS: Set of variables that can be setup from external configuration file during Init phase /////*/
#define NOF_PARAMS	1
extern param_t parameters[] = {
/*		{"opMODE", 				// Parameter name
		 STAT_TYPE_INT,			// Parameter type (see skeleton.h)
		  1},					// Number of samples (of size the type)
		{"num_operations",STAT_TYPE_INT,1},
		{"datatext",STAT_TYPE_STRING,1},
		{"constant",STAT_TYPE_FLOAT,1},
*/
		{NULL, 0, 0} /* need to end with null name */
};
/*////////////////////////////////////////////////////////////////////////////////////////////////////*/



/*////////////////////////////////////////////////////////////////////////////////////////////////////*/
/* STATS: Set of variables that can be accessed from other modules or external entities during        */
/* execution phase. */
#define NOF_EXTRA_VARS	5			//Define the current number of Stats published by this module
/* STATS----------------------------------------------------------------------------------------------------------------INIT*/
struct MODstats{
	char charctrl;
	int intctrl;
    float floatctrl;
    char stringctrl[SLENGTH];
}oStat={0x57, 11, 7.9, "BABY"};


// Stats Idx: They give direct access to Public Stats
int INTCTRL_Idx=0;
int CHARCTRL_Idx=0;
int FLOATCTRL_Idx=0;
int STRINGCTRL_Idx=0;
int oSTAT_Idx=0;
/* STATS----------------------------------------------------------------------------------------------------------------END*/
#endif
/*////////////////////////////////////////////////////////////////////////////////////////////////////*/

