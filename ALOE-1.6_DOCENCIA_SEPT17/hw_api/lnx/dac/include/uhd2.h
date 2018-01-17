

#ifdef __cplusplus

extern "C" {
struct main_conf2 {
	char clock[16];
	char address[64];
	int RxTxmode; //1: TX; 2: RX; 3: TX & RX//int chain_is_tx;
};
}


extern "C" int uhd2_readcfg(struct dac_cfg *dac_cfg);

extern "C" int uhd2_init(struct dac_cfg *cfg, int *timeSlotLength, void (*sync)(void));

extern "C" void uhd2_close();

extern "C" void uhd2_setcfg(struct main_conf *main);


#else

struct main_conf2 {
	char clock[16];
	char address[64];
	int RxTxmode; //1: TX; 2: RX; 3: TX & RX//int chain_is_tx;
};


int uhd2_readcfg(struct dac_cfg *dac_cfg);

int uhd2_init(struct dac_cfg *cfg, int *timeSlotLength, void (*sync)(void));

void uhd2_close();

void uhd2_setcfg(struct main_conf *main);
#endif
