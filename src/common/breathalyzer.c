#include "breathalyzer.h"
#include "gpio.h"
#include "mcp3008.h"
#include "timer.h"

#define DRUNK_THRESHOLD 80
#define NUM_AVERAGE 20
#define READ_TIME 5
static unsigned int max_alc = 125;
static unsigned int adc_channel;


void breathalyzer_init(unsigned int adc_channel_num)
{
    mcp3008_init();
    adc_channel = adc_channel_num;
}

unsigned int breathalyzer_single_read(void) {
  return mcp3008_read(adc_channel);
}

unsigned int breathalyzer_read(void) {
  unsigned int total_signal = 0;
  unsigned int wait = READ_TIME * 1000 / NUM_AVERAGE;
  for (int i = 0; i < NUM_AVERAGE; i++) {
    total_signal += breathalyzer_single_read();
    timer_delay_ms(wait);
  }

  return (total_signal / NUM_AVERAGE);
}

void breathalyzer_calibrate(unsigned int max_bac_reading) {
  max_alc = max_bac_reading;
}

unsigned int get_bac_1000(void) {
  unsigned int reading = breathalyzer_read();
  return (max_alc * reading / 1024);
}

unsigned int detect_drunk(void) {
  return (get_bac_1000() > DRUNK_THRESHOLD);
}
