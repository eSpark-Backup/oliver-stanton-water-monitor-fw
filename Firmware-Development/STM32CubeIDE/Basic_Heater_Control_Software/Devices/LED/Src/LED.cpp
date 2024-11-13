#include "LED.h"


void RgbDriver::setRgbLedGpio(TIM_HandleTypeDef *htim)
{
    // Set GPIOs at the start of the period (turn ON the LEDs)
    HAL_GPIO_WritePin(R_CNTRL_GPIO_Port, R_CNTRL_Pin, GPIO_PIN_SET); // Red ON
    HAL_GPIO_WritePin(G_CNTRL_GPIO_Port, G_CNTRL_Pin, GPIO_PIN_SET); // Green ON
    HAL_GPIO_WritePin(B_CNTRL_GPIO_Port, B_CNTRL_Pin, GPIO_PIN_SET); // Blue ON
}


void RgbDriver::resetRgbLedGpio(TIM_HandleTypeDef *htim)
{
    uint32_t current_timer_value = __HAL_TIM_GET_COUNTER(htim);

    if (current_timer_value >= __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_2))
        HAL_GPIO_WritePin(R_CNTRL_GPIO_Port, R_CNTRL_Pin, GPIO_PIN_RESET); // Red OFF

    if (current_timer_value >= __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_3))
        HAL_GPIO_WritePin(G_CNTRL_GPIO_Port, G_CNTRL_Pin, GPIO_PIN_RESET); // Green OFF

    if (current_timer_value >= __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_4))
        HAL_GPIO_WritePin(B_CNTRL_GPIO_Port, B_CNTRL_Pin, GPIO_PIN_RESET); // Blue OFF
}


void RgbDriver::setRgbColor(HexColor color)
{
    // Extract Red, Green, and Blue values from the color enum (hex value)
    int16_t r = (color >> 16) & 0xFF;  // Extract Red (8 bits)
    int16_t g = (color >> 8) & 0xFF;   // Extract Green (8 bits)
    int16_t b = color & 0xFF;          // Extract Blue (8 bits)

    // Ensure brightness values are within the correct range (0 to 255)
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    // Scale the values to match the range of 0 to 1000%
    uint16_t scaled_r = (r * 1000) / 255;
    uint16_t scaled_g = (g * 1000) / 255;
    uint16_t scaled_b = (b * 1000) / 255;

    if (scaled_r > 1000) scaled_r = 1000;
    if (scaled_g > 1000) scaled_g = 1000;
    if (scaled_b > 1000) scaled_b = 1000;

    rgbColor[0] = scaled_r; 
    rgbColor[1] = scaled_g; 
    rgbColor[2] = scaled_b;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rgbColor[0]);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, rgbColor[1]);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, rgbColor[2]);
}


void RgbDriver::setupInit(void)
{
    if(HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PWM_PULSE_FINISHED_CB_ID, RgbDriver::setRgbLedGpio) != HAL_OK)
        DEBUG_RTT(LOG_ERROR, "RgbDriver class setRgbLedGpio not registered");
    
    if(HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PERIOD_ELAPSED_CB_ID, RgbDriver::resetRgbLedGpio) != HAL_OK)
        DEBUG_RTT(LOG_ERROR, "Period Elapsed Callback not registered");
}


void RgbDriver::setOutput(void)
{
    HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_4);
}


void RgbDriver::stopOutput(void)
{
	    // Manually reset only the relevant GPIOs for the stopped channels
    HAL_GPIO_WritePin(R_CNTRL_GPIO_Port, R_CNTRL_Pin, GPIO_PIN_RESET); // Red OFF
    HAL_GPIO_WritePin(G_CNTRL_GPIO_Port, G_CNTRL_Pin, GPIO_PIN_RESET); // Green OFF
    HAL_GPIO_WritePin(B_CNTRL_GPIO_Port, B_CNTRL_Pin, GPIO_PIN_RESET); // Blue OFF

    HAL_TIM_Base_Stop_IT(&htim1);
    HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_4);
}
