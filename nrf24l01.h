#ifndef NRF24L01_H
#define NRF24L01_H

#include <stdint.h>

/* ========================================================================== */
/* user implemented */
void nrf24l01h_hw_init(void);
void nrf24l01h_wv(uint8_t reg, uint8_t *p, uint8_t size);
void nrf24l01h_rv(uint8_t reg, uint8_t *p, uint8_t size);
void nrf24l01h_delay_ms(uint8_t ms);
void nrf24l01h_abort(void);
/* ========================================================================== */

/* receiver or transmiter?
 * 
 * Set CRC0 2bytes, AA, 5bytes address, auto retransmit, wait minimal, 2Mbps */
void nrf24l01_init(uint8_t ptx, uint8_t power);

/* speeds are: 250k (0), 1M (1), 2M (2) */
void    nrf24l01_set_speed(uint8_t speed);
uint8_t nrf24l01_get_speed(void);

/* 0, 1, or 3 */
void nrf24l01_set_power(uint8_t power);

/* up to 32bytes. */
void nrf24l01_send(const char *p, uint8_t size);

/* up to 32 bytes. */
uint8_t nrf24l01_recv(uint8_t pipe, char *p);

/* even when in receiver mode, when AA is in used this must be set correctly. */
void nrf24l01_set_tx_adress_pipe(const uint8_t address[5]);

/* pipe goes from 0 to 4. */
void nrf24l01_set_rx_adress_pipe(uint8_t pipe, const uint8_t address[5]);

#endif
