#include "tim.h"

#define TIM_FREQ 56e6f

void tim_set_freq(TIM_HandleTypeDef *htim, float freq)
{
    
    /**
     * Frequency = TIM_FREQ / (2 * period * prescaler)
     * for better resolution period should be great as possible
     */

    // Calculate period for the required frequency assuming prescaler as 1
    uint32_t period = TIM_FREQ / (2 * freq);

    if (period > 65535)
        period = 65535; // 16bit period

    // if period was not enough to achieve the frequency, calculate the prescaler
    uint32_t prescaler = TIM_FREQ / (2 * freq * period);

	// Recompute period
	period = TIM_FREQ / (2 * freq * prescaler);

	// If period overflow round up prescaler and recompute period
	if (period > 65535)
	{
		prescaler++;
		period = TIM_FREQ / (2 * freq * prescaler);
	}


    if (prescaler > 65535)
        prescaler = 65535;

	/* Set the TIM prescaler (prescaler starts in 0) */
    __HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
	/* Set the TIM period */
    __HAL_TIM_SetAutoreload(htim, period);

}