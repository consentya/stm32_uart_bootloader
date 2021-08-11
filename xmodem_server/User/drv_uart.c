
#include "drv_uart.h"


extern UART_HandleTypeDef huart2;

#define  DRV_UART_INSTANCE_PTR  (&huart2)

/**
 * @brief   Receives data from UART.
 * @param   *data: Array to save the received data.
 * @param   length:  Size of the data.
 * @return  status: Report about the success of the receiving.
 */
uart_status uart_receive(uint8_t *data, uint16_t length)
{
  uart_status status = UART_ERROR;

  if (HAL_OK == HAL_UART_Receive(DRV_UART_INSTANCE_PTR, data, length, UART_TIMEOUT))
  {
    status = UART_OK;
  }

  return status;
}

/*TODO: implement timeout */
uint8_t     uart_receive_ch(uint16_t timeout)
{
  uint8_t data = 0;
  
  uart_status status = UART_ERROR;

  if (HAL_OK == HAL_UART_Receive(DRV_UART_INSTANCE_PTR, &data, 1, UART_TIMEOUT))
  {
    status = UART_OK;
  }

  return data;
}
/**
 * @brief   Transmits a string to UART.
 * @param   *data: Array of the data.
 * @return  status: Report about the success of the transmission.
 */
uart_status uart_transmit_str(uint8_t *data)
{
  uart_status status = UART_ERROR;
  uint16_t length = 0u;

  /* Calculate the length. */
  while ('\0' != data[length])
  {
    length++;
  }

  if (HAL_OK == HAL_UART_Transmit(DRV_UART_INSTANCE_PTR, data, length, UART_TIMEOUT))
  {
    status = UART_OK;
  }

  return status;
}

/**
 * @brief   Transmits a string to UART.
 * @param   *data: Array of the data.
 * @return  status: Report about the success of the transmission.
 */
uart_status uart_transmit_buffer(uint8_t *data, uint16_t length)
{
  uart_status status = UART_ERROR;

  if (HAL_OK == HAL_UART_Transmit(DRV_UART_INSTANCE_PTR, data, length, UART_TIMEOUT))
  {
    status = UART_OK;
  }

  return status;
}

/**
 * @brief   Transmits a single char to UART.
 * @param   *data: The char.
 * @return  status: Report about the success of the transmission.
 */
uart_status uart_transmit_ch(uint8_t data)
{
  uart_status status = UART_ERROR;

  /* Make available the UART module. */
  if (HAL_UART_STATE_TIMEOUT == HAL_UART_GetState(DRV_UART_INSTANCE_PTR))
  {
    HAL_UART_Abort(DRV_UART_INSTANCE_PTR);
  }

  if (HAL_OK == HAL_UART_Transmit(DRV_UART_INSTANCE_PTR, &data, 1u, UART_TIMEOUT))
  {
    status = UART_OK;
  }
  return status;
}
