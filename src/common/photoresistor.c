#include "photoresistor.h"
#include "mcp3008.h"

#define READINGS 10

static unsigned int channel;

void photoresistor_init(unsigned int adc_channel_num) {
  mcp3008_init();
  channel = adc_channel_num;
}

unsigned int photoresistor_single_read(void) {
  return mcp3008_read(channel);
}

unsigned int photoresistor_read(void) {
  unsigned int total = 0;
  for(int i = 0; i < READINGS; i++) {
    total += photoresistor_single_read();
  }
  return total / READINGS;
}
