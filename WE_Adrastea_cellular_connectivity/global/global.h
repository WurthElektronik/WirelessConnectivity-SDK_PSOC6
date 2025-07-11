/*
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 *
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 */

/**
 * @file
 * @brief This is the main header file of the WE Wireless Connectivity SDK.
 */

#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "global_types.h"
#include "debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WE_PIN(PIN_DEF_POINTER)                                                                                                                                                                                                                                                                                                                                                                                \
    ((WE_Pin_t){                                                                                                                                                                                                                                                                                                                                                                                               \
        .pin_def = PIN_DEF_POINTER,                                                                                                                                                                                                                                                                                                                                                                            \
    })

#define WE_PIN_UNDEFINED()                                                                                                                                                                                                                                                                                                                                                                                     \
    ((WE_Pin_t){                                                                                                                                                                                                                                                                                                                                                                                               \
        .pin_def = NULL,                                                                                                                                                                                                                                                                                                                                                                                       \
    })

#define IS_WE_PIN_UNDEFINED(PIN) (PIN.pin_def == NULL)

#ifndef UNUSED
#define UNUSED(X) (void)X /* To avoid gcc/g++ warnings */
#endif

/**
 * @brief Driver version
 */
#define WE_WIRELESS_CONNECTIVITY_SDK_VERSION {1, 0, 0}

#if defined(WE_DEBUG) || defined(WE_DEBUG_INIT)
#define WE_DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define WE_DEBUG_PRINT(...)
#endif /* WE_DEBUG */

/**
 * @brief Priority for UART interrupts (used for communicating with radio module)
 */
#define WE_PRIORITY_UART_RX 0

/**
 * @brief Priority for (asynchronous) processing of data received from radio module.
 */
#define WE_PRIORITY_RX_DATA_PROCESSING 1

/**
 * @brief Priority for UART interface used for debugging.
 */
#define WE_PRIORITY_UART_DEBUG 2

    /**
 * @brief Initializes the platform (peripherals, flash interface, Systick, system clock, interrupts etc.)
 */
    extern void WE_Platform_Init(void);

    /**
 * @brief Is called in case of a critical HAL error.
 */
    extern void WE_Error_Handler(void);

    /**
 * @brief Request the 3 byte driver version
 *
 * @param[out] version Pointer to the 3 byte driver version.
 * @return true if request succeeded, false otherwise
 */
    extern bool WE_GetDriverVersion(uint8_t* version);

    /**
 * @brief Initialize GPIO pins.
 *
 * @param[in] pins Array of pins to configure. Entries that pin definition as NULL are ignored.
 * @param[in] numPins Number of elements in pins array.
 * @return true if request succeeded, false otherwise
 */
    extern bool WE_InitPins(WE_Pin_t pins[], uint8_t numPins);

    extern bool WE_Reconfigure(WE_Pin_t pin);

    /**
 * @brief Deinitialize a pin.
 *
 * @param[in] pin Pin to be deinitialized
 * @return true if request succeeded, false otherwise
 */
    extern bool WE_DeinitPin(WE_Pin_t pin);

    /**
 * @brief Switch pin to output high/low
 *
 * @param[in] pin Output pin to be set
 * @param[in] out Output level to be set
 * @return true if request succeeded, false otherwise
 */

    extern bool WE_SetPin(WE_Pin_t pin, WE_Pin_Level_t out);

    /**
 * @brief Gets the pin level
 *
 * @param[in] pin: the pin to be checked
 *
 * @param[out] pin_levelP: the pin level
 *
 * @return true if request succeeded,
 *         false otherwise
 *
 */
    extern bool WE_GetPinLevel(WE_Pin_t pin, WE_Pin_Level_t* pin_levelP);

    /**
 * @brief Sleep function.
 *
 * @param[in] sleepForMs Delay in milliseconds
 */
    extern void WE_Delay(uint16_t sleepForMs);

    /**
 * @brief Sleep function.
 *
 * Note that WE_MICROSECOND_TICK needs to be defined to enable microsecond timer resolution.
 *
 * @param[in] sleepForUsec Delay in microseconds
 */
    extern void WE_DelayMicroseconds(uint32_t sleepForUsec);

    /**
 * @brief Returns current tick value (in milliseconds).
 *
 * @return Current tick value (in milliseconds)
 */
    extern uint32_t WE_GetTick();

    /**
 * @brief Returns current tick value (in microseconds).
 *
 * Note that WE_MICROSECOND_TICK needs to be defined to enable microsecond timer resolution.
 *
 * @return Current tick value (in microseconds)
 */
    extern uint32_t WE_GetTickMicroseconds();
/**
 * @brief Initialize and start the UART.
 *
 * @param[in] baudrate Baud rate of the serial interface
 * @param[in] flowControl Enable/disable flow control
 * @param[in] parity Parity bit configuration
 * @param[in] rxByteHandlerP Pointer to the handle rx byte function inside the driver. (this function should be called by the ISR for uart on data reception)
 */
extern bool WE_UART1_Init(uint32_t baudrate, WE_FlowControl_t flowControl, WE_Parity_t parity, WE_UART_HandleRxByte_t *rxByteHandlerP);

/**
 * @brief Deinitialize and stop the UART.
 */
extern bool WE_UART1_DeInit();

/**
 * @brief Transmit data via UART.
 *
 * @param[in] data Pointer to data buffer (data to be sent)
 * @param[in] length Number of bytes to be sent
 */
extern bool WE_UART1_Transmit(const uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* GLOBAL_H_INCLUDED */
