/*
 * target.h
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <stdint.h>

typedef int (* serial_getc_fn)(unsigned long timeout);
typedef int (* serial_putc_fn)(unsigned char *packet, int len);

struct serial
{
	serial_getc_fn  readchar;
	serial_putc_fn  serial_write;
};

#endif /* TARGET_H_ */
