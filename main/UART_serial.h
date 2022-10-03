/*
   UART_serial.h

    Created on: Nov 13, 2018
        Author: Carlos Pastor

    Controls an UART port to detected frames and calls "frameManager_frameAction"
    Also can send frames using "frameManager_sendFrame"
    The maximum frame size is defined on frameManager_bufferSize
    WORKS ON INTERRUPTS ONLY

 	TODO - Frame end detection by size and termination

*/

#ifndef UART_SERIAL_H_
#define UART_SERIAL_H_

#include "usart.h" // must be configured for the program to work


// uses SYSTICK for timings
// in case of using an alternative timebase, change the value
#define frameManager_TIMEBASE 1000 // in microseconds
#define frameManager_BUFFERSIZE 254 //bytes
// handler

typedef enum fm_stt
{
  FM_STOPPED,
  FM_BUSSY,
  FM_FRAMEREADY,
  FM_RUNNING,
  FM_ERROR
} fm_stt;

typedef struct frameManager {

  UART_HandleTypeDef *UART;
  uint8_t TX[frameManager_BUFFERSIZE];
  uint8_t RX[frameManager_BUFFERSIZE];
  uint8_t RXbyte;
  uint16_t RXpt;
  uint16_t size;
  uint32_t tcnt;
  uint32_t tout;
  fm_stt sttTX;
  fm_stt sttRX;
} frameManager;

// Call on main to initialize the internal components of the function
void frameManager_init(frameManager * fm, uint32_t tout, UART_HandleTypeDef * uart);
// Call on the receive interruption to process the input data
void frameManager_byteRX(frameManager * fm);
// Call on the receive interruption to process the input data
void frameManager_TXdone(frameManager * fm);
// Call on the control timer to check timeouts
void frameManager_timebase(frameManager * fm);

// Control functions
void frameManager_start(frameManager * fm);
void frameManager_stop(frameManager * fm);
void frameManager_reset(frameManager * fm);

// User functions
fm_stt frameManager_sendFrame(frameManager * fm, uint8_t* bytes, uint16_t size);
void frameManager_RXframeAction(frameManager * fm);
void frameManager_TXframeAction(frameManager * fm);
void frameManager_frameErrorAction(frameManager * fm);


// EXAMPLE CALLBACK IMPLEMENTATIONS

//define the handler
//	frameManager hframeManager; // example handler
//
//	// on main
//	// sendFrame can be called to transmit using serial
//
//	// define outside
//
//	void frameManager_RXframeAction(frameManager * fm){...}
//	void frameManager_TXframeAction(frameManager * fm){...}
//	void frameManager_frameErrorAction(frameManager * fm){...}
//
//	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//	{
//		if (hframeManager.UART->Instance == huart->Instance)
//			frameManager_TXdone(&hframeManager);
//	}
//	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//	{
//		if (hframeManager.UART->Instance == huart->Instance)
//			frameManager_byteRX(&hframeManager);
//	}
//	void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//	{
//		if (hframeManager.UART->Instance == huart->Instance)
//			frameManager_frameErrorAction(&hframeManager);
//	}
//	void HAL_SYSTICK_Callback(void)
//	{
//		frameManager_timebase(&hframeManager);
//	}

#endif /* UART_SERIAL_H_ */
