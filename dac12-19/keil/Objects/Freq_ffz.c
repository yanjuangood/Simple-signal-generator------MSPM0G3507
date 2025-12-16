#include "ti_msp_dl_config.h"

double Freq_change(	uint32_t Freq)	//100-500KHz 	步进100
{

	double period = (32000000/(Freq*64)) - 1;
	return period;
}