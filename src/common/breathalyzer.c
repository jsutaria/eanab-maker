#include "gpio.h"
#include "breathalyzer.h"
#include "mcp3008.h"
#include "timer.h"

#define MAX_ALC 400
#define DRUNK_THRESHOLD 80
#define NUM_AVERAGE 5
#define READ_TIME 5
static unsigned int adc_channel;

// int readVals(void) {
//     int total = 0;
//     for(int i = 0; i < 100; i++) total+= mcp3008_read(adc_channel);
//     return total/100;
// }

void breathalyzer_init(unsigned int adc_channel_num)
{
    mcp3008_init();
    adc_channel = adc_channel_num;
}

unsigned int breathalyzer_read(void) {
  unsigned int total_signal = 0;
  unsigned int wait = READ_TIME * 1000 / NUM_AVERAGE;
  for (int i = 0; i < NUM_AVERAGE; i++) {
    total_signal += mcp3008_read(adc_channel);
    timer_delay_ms(wait);
  }

  unsigned int reading = total_signal / NUM_AVERAGE;

  unsigned int bac_1000 = MAX_ALC * reading / 1024;
  return bac_1000;
}

unsigned int detect_drunk(void) {
  return (breathalyzer_read() > DRUNK_THRESHOLD);
}
