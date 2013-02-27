#include <msp430.h>
#include "nrf24l01.h"

#define _delay_ms(ms) __delay_cycles(ms*(F_CPU/1000L));

uint8_t spi_rw(uint8_t w);
uint8_t status;

int main(int argc, char *argv[]) {
	WDTCTL = WDTPW + WDTHOLD;

	nrf24l01h_hw_init();

	status = nrf24l01_status();

	P1DIR |= 1;
	P1OUT |= 1;
	while (1);

	return 0;
}
