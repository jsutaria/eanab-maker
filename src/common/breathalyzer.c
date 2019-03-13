#include "gpio.h"
#include "beathalyzer.h"

#define MAX_ALC 400
#define DRUNK_THRESHOLD 80
static unsigned int adc_channel;


void breathalyzer_init(unsigned int adc_channel_num)
{
    adc_channel = adc_channel_num;
}

unsigned int breathalyzer_read(void) {
  unsigned int total_signal = 0;

  unsigned int reading = mcp3008_read(adc_channel);
  unsigned int bac_1000 = MAX_ALC * reading / 1023;
  return bac_1000;
  // if (bac_1000 > DRUNK_THRESHOLD) {
  //   return 0;
  // }
  // return 1;
}
