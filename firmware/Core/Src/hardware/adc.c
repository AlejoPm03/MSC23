#include "adc.h"

static adc_t adc;
static const float ADC_ANG_COEFFS[ADC_CHANNELS] =
	{
		// 7.58257e-2f,
		0.0758256987f / ADC_SAMPLES,
		0.0758256987f / ADC_SAMPLES,
		// 7.58257e-2f,
};
static const float ADC_LIN_COEFFS[ADC_CHANNELS] =
	{
		-1.57422e+2f,
		-1.57422e+2f,
};

uint8_t adc_ready = 0;

volatile uint16_t adc_buffer[ADC_BUFFER_SIZE];
volatile uint32_t adc_sum[ADC_CHANNELS];
volatile uint16_t adc_min[ADC_CHANNELS];
volatile uint16_t adc_max[ADC_CHANNELS];
volatile inputs_t adc_measurements;

void adc_init(ADC_HandleTypeDef *hadc_master, ADC_HandleTypeDef *hadc_slave, TIM_HandleTypeDef *htim_trigger)
{
	adc.hadc_master = hadc_master;
	adc.hadc_slave = hadc_slave;
	adc.htim_trigger = htim_trigger;
}

HAL_StatusTypeDef adc_start(float freq)
{

	// if (HAL_ADCEx_Calibration_Start(adc.hadc) != HAL_OK)
	// {
	//     LOG_ERROR("error calibrating ADC");
	//     return HAL_ERROR;
	// }

	if (HAL_ADC_Start(adc.hadc_slave) != HAL_OK)
	{
		LOG_ERROR("error starting slave ADC");
		return HAL_ERROR;
	}

#ifdef COMBINED_MODE
	if (HAL_ADCEx_MultiModeStart_DMA(adc.hadc_master, (uint32_t *)adc_buffer, ADC_BUFFER_SIZE / 2) != HAL_OK)
#else
	if (HAL_ADC_Start_DMA(adc.hadc_master, (uint32_t *)adc_buffer, ADC_BUFFER_SIZE) != HAL_OK)
#endif
	{
		LOG_ERROR("error starting master DMA ADC");
		return HAL_ERROR;
	}

	if (HAL_TIM_Base_Start_IT(adc.htim_trigger) != HAL_OK)
	{
		LOG_ERROR("error starting adc trigger timer");
		return HAL_ERROR;
	}

	adc_set_freq(freq);

	return HAL_OK;
}

void adc_set_freq(float freq)
{

	float trigger_freq = freq * ADC_SAMPLES / (ADC_CHANNELS * ADC_OVERSAMPLE);

	tim_set_freq(adc.htim_trigger, trigger_freq);
}

float adc_get_freq(void)
{
	uint32_t period = __HAL_TIM_GET_AUTORELOAD(adc.htim_trigger);
	// HAL doesn't have macro to get prescaler
	uint32_t prescaler = adc.htim_trigger->Instance->PSC;
	return ADC_TIM_FREQ / (2 * period * prescaler);
}

const volatile inputs_t *adc_get_measurements(void)
{
	return &adc_measurements;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	// Disable led 1
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

	for (int i = (ADC_BUFFER_SIZE / 2); i < ADC_BUFFER_SIZE; i++)
	{
		/* Calculate sum */
		adc_sum[i % ADC_CHANNELS] += adc_buffer[i];

		/* Calculate min */
		if (adc_buffer[i] < adc_min[i % ADC_CHANNELS])
		{
			adc_min[i % ADC_CHANNELS] = adc_buffer[i];
		}

		/* Calculate max */
		if (adc_buffer[i] > adc_max[i % ADC_CHANNELS])
		{
			adc_max[i % ADC_CHANNELS] = adc_buffer[i];
		}
	}

	for (int i = 0; i < ADC_CHANNELS; i++)
	{

		adc_measurements.channels[i].avg = (ADC_ANG_COEFFS[i] * (float)adc_sum[i]) + ADC_LIN_COEFFS[i];
		adc_measurements.channels[i].min = (ADC_ANG_COEFFS[i] * (float)adc_min[i] * ADC_SAMPLES) + ADC_LIN_COEFFS[i];
		adc_measurements.channels[i].max = (ADC_ANG_COEFFS[i] * (float)adc_max[i] * ADC_SAMPLES) + ADC_LIN_COEFFS[i];
		
		/* Clear variables */
		adc_sum[i] = 0;
		adc_min[i] = 0xFFFFFFFF;
		adc_max[i] = 0;
		
	}
	adc_ready = 1;
	// Enable led 1
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	for (int i = 0; i < ADC_BUFFER_SIZE / 2; i++)
	{
		/* Calculate sum */
		adc_sum[i % ADC_CHANNELS] += adc_buffer[i];
		
		/* Calculate min */
		if (adc_buffer[i] < adc_min[i % ADC_CHANNELS])
		{
			adc_min[i % ADC_CHANNELS] = adc_buffer[i];
		}

		/* Calculate max */
		if (adc_buffer[i] > adc_max[i % ADC_CHANNELS])
		{
			adc_max[i % ADC_CHANNELS] = adc_buffer[i];
		}
	}
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}
