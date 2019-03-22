#include "photoresistor.h"
#include "mcp3008.h"

#define READINGS 4

static unsigned int channel;

/**
 * Initializes the photoresistor
 * @param adc_channel_num the channel number for the ADC that photoresistor is connected to
 */
void photoresistor_init(unsigned int adc_channel_num) {
  mcp3008_init();
  channel = adc_channel_num;
}

/**
 * Reads the photoresistor from the ADC
 * @return the read from ADC
 */
unsigned int photoresistor_single_read(void) {
  return mcp3008_read(channel);
}

/**
 * Takes an average reading of the photoresistor
 * @return  the average reading of the photoresistor
 */
unsigned int photoresistor_read(void) {
  unsigned int total = 0;
  for(int i = 0; i < READINGS; i++) {
    total += photoresistor_single_read();
  }
  return total / READINGS;
}
