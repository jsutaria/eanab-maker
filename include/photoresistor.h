#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#define PHOTORESISTOR_CHANNEL 1

void photoresistor_init(unsigned int adc_channel_num);

unsigned int photoresistor_single_read(void);

unsigned int photoresistor_read(void);

int laser_break(void);

#endif
