#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define apply_bitmask(p, m, v) ((p) = ((p) & ~(m)) | ((v) & (m)))

#define NRF_PORT PORTA
#define NRF_PIN	PINA
#define NRF_DDR	DDRA

#define NRF_SCK	(1<<4) //Output
#define NRF_MISO (1<<5) //Input
#define NRF_MOSI (1<<6) //Output
#define NRF_CSN	(1<<2) //Output

#define NRF_IRQ (1<<0) //Input
#define NRF_CE	(1<<1) //Output

#define NOP 0xff

#define RF_DELAY	5
//#define RF_DELAY	55

/* Bit-banged */
uint8_t spi_rw(uint8_t outgoing) {
	uint8_t i, incoming;
	incoming = 0;

	//Send outgoing byte
	for(i=0; i<8; i++) {
		if(outgoing & 0b10000000)
			NRF_PORT |= NRF_MOSI;
		else
			NRF_PORT &=~NRF_MOSI;

		NRF_PORT |= NRF_SCK;
		_delay_us(RF_DELAY);

		//MISO bit is valid after clock goes going high
		incoming <<=1;
		if( NRF_PIN & NRF_MISO )
			incoming |= 0x01;

		NRF_PORT &=~ NRF_SCK;
		_delay_us(RF_DELAY);

		outgoing <<= 1;
	}

	return incoming;
}

static void nrf24l01h_ss(uint8_t on) {
	apply_bitmask(NRF_PORT, NRF_CSN, on ? NRF_CSN : 0);
}

/* ========================================================================== */
/* user implemented */

void abort(void) {
	cli();
	while(1);
}

void nrf24l01h_hw_init(void) {
	/* SPI */
	apply_bitmask(NRF_DDR,
			NRF_SCK|NRF_MISO|NRF_MOSI,
			NRF_SCK|       0|NRF_MOSI);
	NRF_DDR |= NRF_CE | NRF_CSN;
}

void nrf24l01h_ce(uint8_t on) {
	apply_bitmask(NRF_PORT, NRF_CE, on ? NRF_CE : 0);
}

void nrf24l01h_wv(uint8_t reg, const uint8_t *p, uint8_t size) {
	uint8_t i;
	nrf24l01h_ss(1);
	spi_rw(reg);
	for (i=0; i<size; i++)
		spi_rw(p[i]);
	nrf24l01h_ss(0);
}

void nrf24l01h_rv(uint8_t reg, uint8_t *buf, uint8_t size) {
	uint8_t i;
	nrf24l01h_ss(1);
	spi_rw(reg);
	for (i=0; i<size; i++)
		buf[i]=spi_rw(NOP);
	nrf24l01h_ss(0);
}

void delay_ms(uint8_t ms) {
	uint8_t i;
	for (i=0; i<ms; i++)
		_delay_ms(1);
}

/* ========================================================================== */
