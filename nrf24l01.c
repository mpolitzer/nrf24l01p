#include <stdint.h>
#include "nrf24l01.h"
#include "nrf24l01_reg.h"

#define apply_bitmask(p, m, v) ((p) = ((p) & ~(m)) | ((v) & (m)))

#ifndef MIN
#define MIN(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#endif

#ifndef MAX
#define MAX(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#endif

/* ========================================================================== */
static void    nrf24l01h_w(uint8_t reg, uint8_t v);
static void    nrf24l01h_wm(uint8_t reg, uint8_t m, uint8_t v);
static uint8_t nrf24l01h_r(uint8_t reg);

void nrf24l01p_init(uint8_t istx, uint8_t power) {
	nrf24l01h_hw_init();
}

void nrf24l01_init(uint8_t tx) {
	uint8_t cfg = 1<<EN_CRC | 1<<CRCO | 1<<PWR_UP;

	nrf24l01_w(CONFIG, cfg);
}

#if 0
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
#endif

uint8_t nrf24l01_status(void) {
	return nrf24l01h_r(STATUS);
}

#if 0
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
#endif

/* ========================================================================== */
static void nrf24l01h_w(uint8_t reg, uint8_t v) {
	nrf24l01h_wv(reg, &v, 1);
}

static uint8_t nrf24l01h_r(uint8_t reg) {
	uint8_t v;
	nrf24l01h_rv(reg, &v, 1);
	return v;
}

static void nrf24l01h_wm(uint8_t reg, uint8_t m, uint8_t v) {
	nrf24l01h_w(reg, (nrf24l01h_r(reg) & ~m) | (v&m));
}
