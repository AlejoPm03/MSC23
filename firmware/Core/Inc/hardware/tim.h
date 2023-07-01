#ifndef TIM_H
#define TIM_H

#include "main.h"
#include "utils.h"

/**
 * @brief Change timer frequency
 * 
 * @param htim timer handler
 * @param freq frequency
 */
void tim_set_freq(TIM_HandleTypeDef *htim, float freq);

#endif