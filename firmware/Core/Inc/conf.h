#ifndef CONF_H
#define CONF_H

// Analog Configuration
#define ANALOG_VREF 3.0f

// ADC Configuration
#define ADC_OVERSAMPLE 4
#define ADC_CHANNELS 2
#define ADC_SAMPLES 10
#define ADC_BUFFER_SIZE (ADC_CHANNELS * ADC_SAMPLES)
#define COMBINED_MODE

// ADC default frequency
#define ADC_TIM_FREQ 56e6 //ADC timer clock frequency
#define ADC_FREQ 100 //ADC frequency in Hz
#define DEFAULT_ADC_FREQ CONTROL_DEFAULT_FREQUENCY

#define CAN_FREQUENCY 100.0f //CAN loop frequency in hz
#define CAN_I_BAT_CLK (CAN_FREQUENCY / CAN_MSG_MSC19_5_ADC_FREQUENCY)
#define CAN_I_MOT_CLK (CAN_FREQUENCY / CAN_MSG_MSC19_6_ADC_FREQUENCY)

#endif