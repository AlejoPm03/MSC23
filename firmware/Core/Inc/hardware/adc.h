#ifndef ADC_H
#define ADC_H

#include "main.h"
#include "tim.h"
#include "conf.h"
#include "utils.h"

/**
 * @brief Measurements structure
 * 
 */
typedef struct
{
	float avg;
	float min;
	float max;
}measurement_t;

typedef struct
{
    union 
    {
        measurement_t channels[ADC_CHANNELS];
        struct 
        {
            measurement_t i_m; // Motor current
            measurement_t i_b; // Battery current
        };
    };
}inputs_t;

typedef struct
{
    ADC_HandleTypeDef *hadc_master;
    ADC_HandleTypeDef *hadc_slave;
    TIM_HandleTypeDef *htim_trigger;
}adc_t;

extern uint8_t adc_ready;

void adc_init(ADC_HandleTypeDef *hadc_master, ADC_HandleTypeDef *hadc_slave, TIM_HandleTypeDef *htim_trigger);

HAL_StatusTypeDef adc_start(float freq);

/**
 * @brief Set adc frequency 
 * @note Adc frequency is the frequency of a complete DMA buffer
 * 
 * @param freq frequency in Hz
 */
void adc_set_freq(float freq);

/**
 * @brief get adc frequency 
 * @note Adc frequency is the frequency of a complete DMA buffer
 * 
 * @return freq frequency in Hz
 */
float adc_get_freq(void);
/**
 * @brief returns the pointer of the measurements values 
 * @return pointer of the measurements 
*/
const volatile inputs_t *adc_get_measurements(void);

#endif