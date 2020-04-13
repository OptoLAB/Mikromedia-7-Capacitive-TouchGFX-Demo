/*
 * Author: J.Bajic - 2020
 */


#ifndef TARGET_DISPLAY_H
#define TARGET_DISPLAY_H

#include "stm32f7xx_hal.h"
#include "SSD1963.h"
#include "FT5XX6.h"

#define TP_MIN_PRESSURE 50

//#define DISP_RFRSH_TIM
#define DISP_RFRSH_TE

typedef struct {
  uint16_t x;
  uint16_t y;
  uint8_t p; //weight pressure for FT5XX6
} TOUCH_Data;

// DISPLAY
void DISP_Init(void);
void DISP_TransmitData(uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint8_t DISP_IsTransmittingData(void);
void DISP_TransmitComplete();

// TOUCH
void TOUCH_Init(void);
uint8_t TOUCH_IsPressed(void);
void TOUCH_Reset(void);
TOUCH_Data TOUCH_GetData(void);

// TOUCHGFX - implemented in TOUCHGFXHALL.cpp
void touchgfx_signalVSync(void);
void touchgfx_signalTransmitComplete(void);

#endif /* TARGET_DISPLAY_H_ */
