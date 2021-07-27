/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/**
  * This file configures the system clock as follows:
  *-----------------------------------------------------------------------------
  * System clock source | 1- USE_PLL_HSE_EXTC (external 25/8 MHz clock) | DEVICE_USBDEVICE=1
  *                     | 2- USE_PLL_HSE_XTAL (external 25/8 MHz xtal)  |
  *                     | 3- USE_PLL_HSI (internal 16 MHz)              |
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)         | 100                                           | 96
  * AHBCLK (MHz)        | 100                                           | 96
  * APB1CLK (MHz)       |  50                                           | 48
  * APB2CLK (MHz)       | 100                                           | 96
  * USB capable         |  NO                                           | YES
  *-----------------------------------------------------------------------------
  *
  * HSE values is defined by HSE_VALUE macros.
**/

#include "stm32f4xx.h"
#include "mbed_error.h"

// clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC     0x8  // Use external clock (ST Link MCO)
#define USE_PLL_HSE_XTAL     0x4  // Use external xtal
#define USE_PLL_HSI          0x2  // Use HSI internal clock

// assume that we have 25 MHz external oscillator by default
#ifndef HSE_VALUE
#define HSE_VALUE 25000000
#endif

#if (((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC))
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet. */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Get the Clocks configuration according to the internal RCC registers */
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

    /* PLL could be already configured by bootlader */
    if (RCC_OscInitStruct.PLL.PLLState != RCC_PLL_ON) {

        // Enable HSE oscillator and activate PLL with HSE as source
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        if (bypass == 0) {
            RCC_OscInitStruct.HSEState = RCC_HSE_ON; // External 8/25 MHz xtal on OSC_IN/OSC_OUT
        } else {
            RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS; // External 8/25 MHz clock on OSC_IN
        }

        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

#if (HSE_VALUE == 8000000) // 8 MHz

#if (DEVICE_USBDEVICE)
        RCC_OscInitStruct.PLL.PLLM = 4; // 2 Mhz (PPL input)
        RCC_OscInitStruct.PLL.PLLN = 192; // 384 MHz (VCO)
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4; // 96 MHz (SYSCLK)
        RCC_OscInitStruct.PLL.PLLQ = 8 // 48 MHz (USB)
#else /* DEVICE_USBDEVICE */
        RCC_OscInitStruct.PLL.PLLM = 4; // 2 Mhz (PPL input)
        RCC_OscInitStruct.PLL.PLLN = 200; // 400 MHz (VCO)
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4; // 100 MHz (SYSCLK)
        RCC_OscInitStruct.PLL.PLLQ = 8 // 50 MHz (USB)
#endif /* DEVICE_USBDEVICE */

#elif (HSE_VALUE == 25000000) // 25 MHz

#if (DEVICE_USBDEVICE)
        RCC_OscInitStruct.PLL.PLLM = 25; // 1 Mhz (PPL input)
        RCC_OscInitStruct.PLL.PLLN = 192; // 192 MHz (VCO)
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // 96 MHz (SYSCLK)
        RCC_OscInitStruct.PLL.PLLQ = 4 // 48 MHz (USB)
#else /* DEVICE_USBDEVICE */
        RCC_OscInitStruct.PLL.PLLM = 25; // 1 Mhz (PPL input)
        RCC_OscInitStruct.PLL.PLLN = 200; // 200 MHz (VCO)
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // 100 MHz (SYSCLK)
        RCC_OscInitStruct.PLL.PLLQ = 4; // 50 MHz (USB)
#endif /* DEVICE_USBDEVICE */

#else  /* HSE_VALUE */
#error Unsupported externall clock value, check HSE_VALUE define
#endif /* HSE_VALUE */

        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
            return 0; // FAIL
        }
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 100/96 MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // 100/96 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // 50/48 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 100/96 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        return 0; // FAIL
    }

    /* Output clock on MCO1 pin(PA8) for debugging purpose */
    //if (bypass == 0)
    //  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2); // 4 MHz with xtal
    //else
    //  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); // 8 MHz with external clock

    return 1; // OK
}

#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */
