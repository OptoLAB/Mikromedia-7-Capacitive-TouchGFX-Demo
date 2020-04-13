/*
 * Author: J.Bajic - 2020
 */

#include "TargetDisplay.h"

static uint8_t isTransmittingData = 0;
static uint8_t isPressed = 0;

extern I2C_HandleTypeDef hi2c3;  //i2c port for touch screen
#ifdef DISP_RFRSH_TIM
extern TIM_HandleTypeDef htim5;
#endif
//=============================================================================
// DISPLAY
//=============================================================================
void DISP_Init(void)
{
	// Configure display GPIOs externally (CubeMx)
	  SSD1963_Init();
#ifdef DISP_RFRSH_TE
	  SSD1963_WriteCommand(SSD1963_SET_TEAR_ON); //enable tear efect signal generation
	  SSD1963_WriteData(0x00);
#endif
#ifdef DISP_RFRSH_TIM
	  SSD1963_WriteCommand(SSD1963_SET_TEAR_OFF); //disable tear efect signal generation
	  HAL_TIM_Base_Start_IT(&htim5);
#endif
}

void DISP_TransmitData(uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	uint8_t lo=0, hi=0;
	uint32_t n_bytes=w*h;

    isTransmittingData = 1;					// not necessary with this implementation since DISP_TransmitData is blocking anyway

    SSD1963_SetArea(x, x+w-1, y, y+h-1);
    SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);
    while (n_bytes)
    {
    	lo=*pixels++;;
        hi=*pixels++;;
    	SSD1963_WriteData(hi<<8|lo);
    	n_bytes--;
    }

    isTransmittingData = 0;					// not necessary with this implementation since DISP_TransmitData is blocking anyway

    touchgfx_signalTransmitComplete(); 		// inform TouchGFX to start new transfer since this is complete
}

uint8_t DISP_IsTransmittingData(void) 		// not necessary with this implementation since DISP_TransmitData is blocking anyway
{
    return isTransmittingData;
}

//=============================================================================
// TOUCH
//=============================================================================
void TOUCH_Init(void)
{
	// Configure touch GPIOs externally (CubeMx)
	  FT5XX6_Init(hi2c3);
}

uint8_t TOUCH_IsPressed(void)
{
    return isPressed;
}

void TOUCH_Reset(void)
{
	if(HAL_GPIO_ReadPin(INT_PORT,INT_PIN)==GPIO_PIN_SET) //reset touch state if INT goes high
		isPressed=0;
}

TOUCH_Data TOUCH_GetData(void)
{
	TOUCH_Data td={0,0,0};
	CTP_point_data cpd=FT5XX6_ReadPoint(0);   // only reading first touch point

	td.x=cpd.x;
	td.y=cpd.y;
	td.p=cpd.pressure;

	return td;
}

//=============================================================================
// REFRESH INTERRUPT -> TE signal or Timer interrupt
//=============================================================================

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==INT_PIN) 			//touch interrupt - pressed
	{
		  isPressed=1;				//TouchGFX checks if touch is pressed by calling TOUCH_IsPressed()
	}
#ifdef DISP_RFRSH_TE
	if(GPIO_Pin==TE_PIN) 			//display interrupt - tear signal
	{
		touchgfx_signalVSync(); 	// inform TouchGFX to render new frame since TE is occurred
	}
#endif
}

#ifdef DISP_RFRSH_TIM
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM5)
	{
		touchgfx_signalVSync();
	}
}
#endif
