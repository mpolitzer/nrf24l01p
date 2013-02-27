#include <stdint.h>
#include <msp430.h>
#include "nrf24l01.h"

/* ========================================================================== */
/* PINS definition */
#define PDIR P1DIR
#define POUT P1OUT
#define PIN  P1IN

/* Module PINOUT */

/* GND */
/* VCC */
#define CE   (1<<1)
#define CSN  (1<<2)
#define SCK  (1<<3)
#define MOSI (1<<4)
#define MISO (1<<5)
#define IRQ  (1<<6)

/* ========================================================================== */

#define NOP 0xff
#define SPIHALF 1

#define apply_bitmask(p, m, v) ((p) = ((p) & ~(m)) | ((v) & (m)))

void nrf24l01h_delay_ms(uint8_t ms);
void nrf24l01h_abort(void);

uint8_t spi_rw(uint8_t w) {
	int i, r=0;

	for (i=0; i<8; i++, w<<=1) {
		if (w & 0x80)	POUT |= MOSI;
		else			POUT &=~MOSI;

		/* clock it */
		POUT |= SCK;
		//__delay_cycles(SPIHALF);

		r<<=1;
		if (PIN & MISO) r |= 1;

		POUT &=~SCK;
		//__delay_cycles(SPIHALF);
	}
	return r;
}

static void spi_ss(uint8_t en) {
	if (en) POUT &=~CSN; /* enable on low */
	else    POUT |= CSN;
}

/* ========================================================================== */

void nrf24l01h_hw_init(void) {
	apply_bitmask(PDIR,
			MISO|MOSI|SCK|IRQ|CE|CSN,
			0   |MOSI|SCK|0  |CE|CSN);
	apply_bitmask(POUT,
			MISO|MOSI|SCK|IRQ|CE|CSN,
			0   |0   |0  |0  |0 |CSN);
}

void nrf24l01h_wv(uint8_t reg, uint8_t *p, uint8_t size) {
	uint8_t i;
	spi_ss(1);
	spi_rw(reg);
	for (i=0; i<size; i++)
		spi_rw(p[i]);
	spi_ss(0);
}

void nrf24l01h_rv(uint8_t reg, uint8_t *p, uint8_t size) {
	uint8_t i;
	spi_ss(1);
	spi_rw(reg);
	for (i=0; i<size; i++)
		p[i] = spi_rw(NOP);
	spi_ss(0);
}

void nrf24l01h_delay_ms(uint8_t ms) {
	uint8_t i;
	for (i=0; i<ms; i++)
		__delay_cycles(F_CPU/1000);
}

void nrf24l01h_abort(void) { /* sleep forever */
	while (1) __bis_SR_register(LPM3_bits);
}
