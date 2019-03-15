#ifndef BREATHALYZER_H
#define BREATHALYZER_H

#define BREATHALYZER_CHANNEL 0

void breathalyzer_init(unsigned int adc_channel_num);

unsigned int breathalyzer_read(void);

unsigned int get_bac_1000(void);

void breathalyzer_calibrate(unsigned int max_bac_reading);

unsigned int detect_drunk(void);

#endif
