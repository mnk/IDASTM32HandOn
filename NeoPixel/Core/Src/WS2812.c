/*
 * WS2812.c
 *
 *  Created on: May 26, 2024
 *      Author: gert
 */
#include "WS2812.h"
#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef htim2;
#define NEOPIXEL_ZERO 34 //(ARR+1)(0,32)=(102*0,33)=33,6
#define NEOPIXEL_ONE 67  //(ARR+1)(0,64)=(102*0,66)=67,2
#define DMA_BUFF_SIZE (WS2812_NUM_PIXELS*24)+1
  PixelRGB_t pixel[WS2812_NUM_PIXELS] = {0};
  uint32_t dmaBuffer[DMA_BUFF_SIZE] = {0};
  uint32_t *pBuff;
  int i, j, k;
  uint16_t stepSize;

  void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
  {
    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
  }


  void WS2812_UpdatePixel() {
//	  k = 0;
	  stepSize = 4;
	  for (i = (WS2812_NUM_PIXELS - 1); i > 0; i--)
	      {
	        pixel[i].data = pixel[i-1].data;
	      }

	      if (k < 255)
	      {
	        pixel[0].color.g = 254 - k; //[254, 0]
	        pixel[0].color.r =  k + 1;  //[1, 255]
	        pixel[0].color.b = 0;
	      }
	      else if (k < 510)
	      {
	        pixel[0].color.g = 0;
	        pixel[0].color.r = 509 - k; //[254, 0]
	        pixel[0].color.b = k - 254; //[1, 255]
	        j++;
	      }
	      else if (k < 765)
	      {
	        pixel[0].color.g = k - 509; //[1, 255];
	        pixel[0].color.r = 0;
	        pixel[0].color.b = 764 - k; //[254, 0]
	      }
	      k = (k + stepSize) % 765;

	      // not so bright
	      pixel[0].color.g >>= 2;
	      pixel[0].color.r >>= 2;
	      pixel[0].color.b >>= 2;

	      pBuff = dmaBuffer;
	      for (i = 0; i < WS2812_NUM_PIXELS; i++)
	      {
	         for (j = 23; j >= 0; j--)
	         {
	           if ((pixel[i].data >> j) & 0x01)
	           {
	             *pBuff = NEOPIXEL_ONE;
	           }
	           else
	           {
	             *pBuff = NEOPIXEL_ZERO;
	           }
	           pBuff++;
	       }
	      }
	      dmaBuffer[DMA_BUFF_SIZE - 1] = 0; // last element must be 0!

	      HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, dmaBuffer, DMA_BUFF_SIZE);
  }
