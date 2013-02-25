#include <stdint.h>
#include "nrf24l01p.h"
#include "nrf24l01p_reg.h"

#ifndef MIN
#define MIN(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#endif

#ifndef MAX
#define MAX(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#endif

/* ========================================================================== */
/* platform specific functions. */

/* SS enable | write/status | write | SS disable. */
void
nrfw(uint8_t reg, uint8_t value);

/* SS enable |  status |  reg | SS disable. */
uint8_t
nrfr(uint8_t reg);

/* SS enable |  write[size]  | SS disable */
void
nrfwv(uint8_t reg, const uint8_t *buf, uint8_t size);

/* SS enable |   read[size]  | SS disable */
void
nrfrv(uint8_t reg, uint8_t *buf, uint8_t size);

/* gpio edge IRQ. */
void nrf24l01p_service_irq(void);

/* ========================================================================== */

/* receiver or transmiter? */
void nrf24l01p_init(uint8_t istx, uint8_t power) {
	/* Enable CRC with 2 bytes and power device up */
	uint8_t cfg = 1<<EN_CRC | 1<<CRC0 | 1<<PWR_UP;

	/* enable IRQs on the IRQ pin */
	if (istx) cfg |= 1<<MASK_TX_DS | 1<<MASK_MAX_RT;
	else	  cfg |= 1<<MASK_RX_DS | 1<<PRIM_RX;

	nrfw(CONFIG, cfg);
	nrfw(ENAA, (1<<ENAA_P0)); /* Pipe0 Auto Ack. */
	nrfw(EN_RXADDR, (1<<ERX_P0)); /* Pipe0 Enable. */
	nrfw(SETUP_AW, (1<<AW)); /* 5bytes address. */
}

uint8_t nrf24l01p_status(void) {
	return nrfr(STATUS);
}

/* up to 32bytes. */
void nrf24l01p_send(const uint8_t *p, uint8_t size) {
	nrfwv(W_TX_PAYLAOD, p, MIN(size, 32));
}

/* return bytes read up to 32. */
uint8_t nrf24l01p_recv(char *p, uint8_t size) {
	if (!(nrf24l01p_status() & ST_RX_DR)) return 0;
	nrfrv(W_RX_PAYLAOD, p, MIN(size, 32));
	return 32;
}

/* even when in receiver mode, when AA is in used this must be set correctly. */
void nrf24l01p_set_tx_adress_pipe(const uint8_t address[5]) {
	nrfwv(TX_ADDR, adress, 5);
}

/** num goes from 0 to 5.
 * @return fails for invalid pipe (vaid: [0-5]) */
uint8_t nrf24l01p_set_rx_adress_pipe(uint8_t num, const uint8_t address[5]) {
	if (num > 5) return 1;
	nrfwv(RX_ADDR_P0 + num, adress, 5);
	return 0;
}
