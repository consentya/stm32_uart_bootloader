/*
 * xmodem_app.c
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#include "xmodem_app.h"
#include "xmodem.h"
#include "drv_uart.h"
#include "flash.h"

static struct serial  xmodem_ser_inst;

void xmodem_app_init( void )
{
	xmodem_ser_inst.send_func = uart_transmit_buffer;
	xmodem_ser_inst.recv_func = uart_receive_ch;
}

/**
 * \brief Send the files through XMODEM protocol
 *
 * \param usart  Base address of the USART instance.
 * \param p_buffer  Pointer to send buffer
 * \param ul_length transfer file size
 */
void xmodem_send_file(const uint8_t *p_buffer, uint32_t ul_length)
{
	uint8_t c_char;
	uint8_t uc_sno = 1; // uc_sno = sequence number
	int32_t l_done;
	uint8_t* p_send_buffer = p_buffer;
	uint32_t send_length = ul_length;

	/* STEP 1: Startup synchronization... */
	/* Wait to receive a NAK or 'C' from receiver. */
	l_done = 0;
	while(!l_done) {
		// TODO: implement timeout checking
		l_done = xmodem_init_xfer(&xmodem_ser_inst);
	}

	/* STEP 2: Transfer data ... */
	l_done = 0;
	uc_sno = 1;
	while (!l_done) {
		c_char = send_xmodem_packet(&xmodem_ser_inst, (uint8_t *)p_send_buffer, uc_sno);
		switch(c_char) {
			case ACK:
				++uc_sno;
				send_length-= XMODEM_DATASIZE;
				p_send_buffer += XMODEM_DATASIZE;
				break;
			case NAK:
				break;
			case CANCEL:
			case EOT:
			default:
				l_done = -1;
				break;
		}

		/* STEP 3: End of Transfer ... */
		if (!send_length) {
			xmodem_finish_xfer(&xmodem_ser_inst);
			/* Flush the ACK */
			xmodem_flush(&xmodem_ser_inst);
			break;
		}
	}
}

void xmodem_app_unit_test( void )
{
	xmodem_send_file((uint8_t*)FLASH_APP_START_ADDRESS, USER_FLASH_SIZE);
}
