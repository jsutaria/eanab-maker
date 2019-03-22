#include "breathalyzer.h"
#include "gpio.h"
#include "mcp3008.h"
#include "timer.h"

#define DRUNK_THRESHOLD 80
#define NUM_AVERAGE 20
#define READ_TIME 5
static unsigned int max_alc = 125;
static unsigned int adc_channel;

/**
 * Initializes the breathalyzer for use
 * @param adc_channel_num the channel of the ADC that we are using for the breathalyzer
 */
void breathalyzer_init(unsigned int adc_channel_num)
{
    mcp3008_init();
    adc_channel = adc_channel_num;
}

/**
 * Read a single value from the breathalyzer
 * @return returns the analog value the breathalyzer is at
 */
unsigned int breathalyzer_single_read(void) {
  return mcp3008_read(adc_channel);
}

/**
 * Take an average reading of the breathalyzer
 * @return the analog value the breathalyzer is at, averaged to account for fluctuations
 */
unsigned int breathalyzer_read(void) {
  unsigned int total_signal = 0;
  unsigned int wait = READ_TIME * 1000 / NUM_AVERAGE;
  for (int i = 0; i < NUM_AVERAGE; i++) {
    total_signal += breathalyzer_single_read();
    timer_delay_ms(wait);
  }

  return (total_signal / NUM_AVERAGE);
}

/**
 * Calibrate the breathalyzer
 * @param max_bac_reading the maximum BAC reading 
 */
void breathalyzer_calibrate(unsigned int max_bac_reading) {
  max_alc = max_bac_reading;
}

/**
 * Get the blood alcohol content of the user, multiplied by 1000
 * @return user BAC * 1000
 */
unsigned int get_bac_1000(void) {
  unsigned int reading = breathalyzer_read();
  return (max_alc * reading / 1024);
}

/**
 * Get information as to whether the user is drunk or not
 * @return a boolean indicating whether or not the user is drunk
 */
unsigned int detect_drunk(void) {
  return (get_bac_1000() > DRUNK_THRESHOLD);
}
