#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_hal.h"
#include "usart.h"


/* Timeout for HAL. */
#define UART_TIMEOUT ((uint16_t)0xFFFFFFFFu)

/* Status report for the functions. */
typedef enum {
  UART_OK     = 0x00u, /**< The action was successful. */
  UART_ERROR  = 0xFFu  /**< Generic error. */
} uart_status;

uart_status uart_receive(uint8_t *data, uint16_t length);
uint8_t     uart_receive_ch(uint16_t timeout);

uart_status uart_transmit_ch(uint8_t data);
uart_status uart_transmit_str(uint8_t *data);
uart_status uart_transmit_buffer(uint8_t *data, uint16_t length);


#endif /* UART_H_ */
