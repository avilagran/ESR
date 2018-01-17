#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "str.h"
#include "cfg_parser.h"
#include "phal_hw_api_man.h"
#include "dac_cfg.h"
#include "uhd.h"

struct main_conf2 main_cfg;

char filepath[200];

int uhd2_readcfg(struct dac_cfg *dac_cfg) {

	int i, j, offset;
	char *buffer;
	cfg_o cfg;
	sect_o sect;
	key_o key;
	int error=0;
	char *name;
	float tmp;


	printf("UHD_READCFG_____________________________\n");
	
	hwapi_mem_silent(1);

	name=dac_cfg->filename;

	sprintf(filepath, "/usr/local/etc/%s", name);

	offset = hwapi_res_parseall_(filepath, &buffer,1);
	if (offset < 0) {
		printf ("Error reading file %s.\n", filepath);
		return 0;
	}

	cfg = cfg_new(buffer, offset);
	if (!cfg) {
		printf ("Error parsing file %s\n", filepath);
		free(buffer);
		return 0;
	}
	hwapi_mem_silent(0);
printf("uhd_paser:readcfg():MAIN\n");
	sect=Set_find(cfg_sections(cfg),"main",sect_findtitle);
	if (!sect) {
		printf("Error can't find section main\n");
		free(buffer);
		cfg_delete(&cfg);
		return 0;
	} else {
		if (!(key = Set_find(sect_keys(sect),"address",key_findname))) {
			printf("Error unspecified option address in uhd config file\n");
		} else {
			str_get(key->pvalue,main_cfg.address,64);
		}
		if (!(key = Set_find(sect_keys(sect),"clock",key_findname))) {
			printf("Error unspecified option clock in uhd config file\n");
		} else {
			str_get(key->pvalue,main_cfg.clock,64);
		}
		if (!(key = Set_find(sect_keys(sect),"chain",key_findname))) {
			printf("Error unspecified option chain in uhd config file\n");
			main_cfg.RxTxmode=0; 				//main_cfg.chain_is_tx=1;
		} else {
//			key_value(key,1,PARAM_INT,&tmp);
			key_value(key,1,PARAM_INT,&main_cfg.RxTxmode);
//			main_cfg.RxTxmode=(int) tmp;
			printf("main_cfg.RxTxmode %d\n", main_cfg.RxTxmode);
			
			//str_get(key->pvalue,main_cfg.RxTxmode,64);	//main_cfg.chain_is_tx=!str_scmp(key->pvalue,"tx");
		}
	}
printf("uhd_paser:readcfg():TX\n");
	sect=Set_find(cfg_sections(cfg),"tx",sect_findtitle);
	if (!sect) {
		printf("TX is disabled\n");
	} else {
		if (!(key = Set_find(sect_keys(sect),"samp_freq",key_findname))) {
			printf("Warning unspecified tx sample frequency. Setting to 1 MHz\n");
			dac_cfg->outputFreq = 1000000	;
		} else {
			key_value(key,1,PARAM_FLOAT,&tmp);
			dac_cfg->outputFreq=(double) tmp;
		}
		if (!(key = Set_find(sect_keys(sect),"freq",key_findname))) {
			printf("Warning unspecified tx frequency. Setting to default\n");
			dac_cfg->outputRFFreq = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&tmp);
			dac_cfg->outputRFFreq=(double) tmp;
		}
		if (!(key = Set_find(sect_keys(sect),"blocksize",key_findname))) {
			printf("Warning unspecified tx blocksize. Setting to 10 K\n");
			dac_cfg->NsamplesOut = 10000;
		} else {
			key_value(key,1,PARAM_INT,&dac_cfg->NsamplesOut);
		}
		if (!(key = Set_find(sect_keys(sect),"gain",key_findname))) {
			printf("Warning unspecified tx gain. Setting to default\n");
			dac_cfg->tx_gain = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&dac_cfg->tx_gain);
		}
		if (!(key = Set_find(sect_keys(sect),"bw",key_findname))) {
			printf("Warning unspecified bw. Setting to default\n");
			dac_cfg->tx_bw = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&dac_cfg->tx_bw);
		}

	}
printf("uhd_paser:readcfg():RX\n");
	sect=Set_find(cfg_sections(cfg),"rx",sect_findtitle);
	if (!sect) {
		printf("RX is disabled\n");
	} else {
		if (!(key = Set_find(sect_keys(sect),"samp_freq",key_findname))) {
			printf("Warning unspecified rx sample frequency. Setting to 1 MHz\n");
			dac_cfg->inputFreq = 1000000;
		} else {
			key_value(key,1,PARAM_FLOAT,&tmp);
			dac_cfg->inputFreq = (double) tmp;
		}
		if (!(key = Set_find(sect_keys(sect),"freq",key_findname))) {
			printf("Warning unspecified rx frequency. Setting to default\n");
			dac_cfg->inputRFFreq = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&tmp);
			dac_cfg->inputRFFreq = (double) tmp;
		}
		if (!(key = Set_find(sect_keys(sect),"blocksize",key_findname))) {
			printf("Warning unspecified rx blocksize. Setting to 10 K\n");
			dac_cfg->NsamplesIn = 10000;
		} else {
			key_value(key,1,PARAM_INT,&dac_cfg->NsamplesIn);
		}
		if (!(key = Set_find(sect_keys(sect),"gain",key_findname))) {
			printf("Warning unspecified rx gain. Setting to default\n");
			dac_cfg->rx_gain = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&dac_cfg->rx_gain);
		}
		if (!(key = Set_find(sect_keys(sect),"bw",key_findname))) {
			printf("Warning unspecified rx bw. Setting to default\n");
			dac_cfg->rx_bw = 0;
		} else {
			key_value(key,1,PARAM_FLOAT,&dac_cfg->rx_bw);
		}
	}
	
	// Inactive DAC until "run"
	dac_cfg->DAC_Active=0;

	printf("UHD PARSER__XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX_INIT\n");
	printf("X USRP.conf: address=%s                                                     X\n", main_cfg.address);
	printf("X USRP.conf: clock=%s                                                       X\n", main_cfg.clock);
	printf("X USRP.conf: chain=%d                                                       X\n", main_cfg.RxTxmode);
	printf("X   Transmitter                                                             X\n");
	printf("X USRP.conf: samp_freq=%3.2lf                                               X\n", dac_cfg->outputFreq);
	printf("X USRP.conf: freq=%3.2lf                                                    X\n", dac_cfg->outputRFFreq);
	printf("X USRP.conf: blocksize=%d                                                   X\n", dac_cfg->NsamplesOut);
	printf("X USRP.conf: tx_gain=%3.2f                                                  X\n",dac_cfg->tx_gain);
	printf("X USRP.conf: bw=%3.2f                                                       X\n",dac_cfg->tx_bw);
	printf("X dac_cfg->dacoutbuff[0][0]=0x%.12X                                         X\n",&dac_cfg->dacoutbuff[0][0]);
	printf("X   Receiver                                                                X\n");
	printf("X USRP.conf: samp_freq=%3.2lf                                               X\n", dac_cfg->inputFreq);
	printf("X USRP.conf: freq=%3.2lf                                                    X\n", dac_cfg->inputRFFreq);
	printf("X USRP.conf: blocksize=%d                                                   X\n", dac_cfg->NsamplesIn);
	printf("X USRP.conf: tx_gain=%3.2f                                                  X\n",dac_cfg->rx_gain);
	printf("X USRP.conf: bw=%3.2f                                                       X\n",dac_cfg->rx_bw);
	printf("X dac_cfg->dacinbuff[0][0]=0x%.12X                                          X\n",&dac_cfg->dacinbuff[0][0]);
	printf("X DAC_Active=%3.2f                                                          X\n",dac_cfg->DAC_Active);
	printf("UHD PARSER__XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX_END\n");



printf("uhd_paser:readcfg():END RX\n");
	dac_cfg->nof_channels=1;
	dac_cfg->sampleType=DAC_SAMPLE_COMPLEXFLOAT;
	free(buffer);
printf("uhd_paser:readcfg():1\n");
	cfg_delete(&cfg);
printf("uhd_paser:readcfg():2\n");
	uhd_setcfg(&main_cfg);
printf("uhd_paser:readcfg():3\n");
printf("uhd_paser:readcfg():RETURN=1\n");
	return 1;

}
