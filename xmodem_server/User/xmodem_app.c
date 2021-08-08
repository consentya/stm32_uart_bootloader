/*
 * xmodem_app.c
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#include "xmodem_app.h"
#include "xmodem.h"


static struct serial  xmodem_ser_inst;

static void _xmodem_serial_init(struct serial* ser);

void xmodem_app_init( void )
{
	_xmodem_serial_init(&xmodem_ser_inst);
}

void xmodem_app_loop( void )
{

}


static void _xmodem_serial_init(struct serial* ser)
{

}
