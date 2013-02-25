#include "nrf24l01p.h"
#include "nrf24l01p_reg.h"

/* receiver or transmiter?
 * 
 * Set CRC0 2bytes, AA, 5bytes address, auto retransmit, wait minimal, 2Mbps */
void nrf24l01p_init(uint8_t ptx, uint8_t power);

/* set speed in order: 250k, 1M, 2M */
void nrf24l01p_speed(uint8_t speed);

/* up to 32bytes. */
void nrf24l01p_send(const char *p, uint8_t size);

/* up to 32 bytes. */
uint8_t nrf24l01p_recv(uint8_t pipe, char *p);

/* even when in receiver mode, when AA is in used this must be set correctly. */
void nrf24l01p_set_tx_adress_pipe(const uint8_t address[5]);

/* pipe goes from 0 to 4. */
void nrf24l01p_set_rx_adress_pipe(uint8_t pipe, const uint8_t address[5]);
