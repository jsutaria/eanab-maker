#ifndef BREATHALYZER_H
#define BREATHALYZER_H


#include "gpio.h"
#include "mcp3008.h"

void breathalyzer_init(unsigned int adc_channel_num);

unsigned int breathalyzer_read(void);

unsigned int detect_drunk(void);

#endif
