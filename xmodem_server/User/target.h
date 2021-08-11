/*
 * target.h
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <stdint.h>

//typedef unsigned char my_uint8_t;

typedef uint8_t (* serial_getc_fn)(uint16_t);
typedef int (* serial_putc_fn)(uint8_t *, uint16_t ); // serial_putc_fn = function pointer

struct serial
{
	serial_putc_fn   send_func;
	serial_getc_fn   recv_func;
};

#endif /* TARGET_H_ */
