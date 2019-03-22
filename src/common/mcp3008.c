#include "mcp3008.h"
#include "spi.h"

/**
 * Initialize the mcp3008 ADC
 */
void mcp3008_init(void)
{
    spi_init(SPI_CE0, 1024); /* 250KHz */
}

/**
 * Reads the current analog value of the ADC at the given pin
 * @param  channel The channel that the ADC is at
 * @return         A value from 0-1023, indicating the voltage of the pin in relation to VRef 
 */
unsigned int mcp3008_read( unsigned int channel ) {
    unsigned char tx[3];
    unsigned char rx[3];

    // "Start bit", wakes up the ADC chip.
    tx[0] = 1;
    // "Configuration byte", single mode + channel
    tx[1] = 0x80 | ((channel & 0x7) << 4);
    tx[2] = 0;

    spi_transfer(tx, rx, 3);
    return ((rx[1] & 0x3) << 8) + rx[2];
}
