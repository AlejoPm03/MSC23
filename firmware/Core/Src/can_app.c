
#include <stdio.h>
#include "can_app.h"
#include "can_ids.h"
#include "adc.h"
#define CAN_I_MOT_PERIOD_MILIS 1000 / CAN_MSG_MSC19_4_STATE_FREQUENCY
#define CAN_I_BAT_PERIOD_MILIS (uint32_t)(1000U / CAN_MSG_MSC19_5_STATE_FREQUENCY)

static can_hardware_t can_hardware;

void can_config(CAN_HandleTypeDef *hcan1);
void can_send_measurements(uint8_t id, uint8_t signature, float avg, float min, float max);

void can_init(CAN_HandleTypeDef *hcan)
{
	can_hardware.hcan = hcan;
	can_config(hcan);
}

void can_config(CAN_HandleTypeDef *hcan1)
{
	CAN_FilterTypeDef sFilterConfig;

	/* Configure Rx filter */
	// sFilterConfig.IdType = CAN_STANDARD_ID;
	// sFilterConfig.FilterIndex = 0;
	// sFilterConfig.FilterType = CAN_FILTER_MASK;
	// sFilterConfig.FilterConfig = CAN_FILTER_TO_RXFIFO0;
	// sFilterConfig.FilterID1 = 0x00;
	// sFilterConfig.FilterID2 = 0x00;

	can_hardware.fifo = CAN_RX_FIFO0;

	// if (HAL_CAN_ConfigFilter(hcan1, &sFilterConfig) != HAL_OK)
	// {
	// 	LOG_ERROR("Failed to configure CAN");
	// }

	/* Start the CAN module */
	if (HAL_CAN_Start(hcan1) != HAL_OK)
	{
		LOG_ERROR("Failed to configure CAN");
	}
}

/**
 * @brief Send measurements over CAN
 * 
 * @param id CAN ID
 * @param signature Signature of the message
 * @param avg Average value
 * @param min Minimum value
 * @param max Maximum value
 */
void can_send_measurements(uint8_t id, uint8_t signature, float avg, float min, float max)
{
	uint32_t pTxMailbox;

	CAN_TxHeaderTypeDef TxHeader = {
		.StdId = id,
		.IDE = CAN_ID_STD,
		.RTR = CAN_RTR_DATA,
		.DLC = CAN_MSG_MSC19_1_ADC_LENGTH,
	};
	
	uint8_t TxData[CAN_MSG_MSC19_1_ADC_LENGTH];

	/* Signature */
	TxData[CAN_MSG_MSC19_1_ADC_SIGNATURE_BYTE] = signature;
	/* Average */
	TxData[CAN_MSG_MSC19_1_ADC_AVG_L_BYTE] = (uint8_t)(avg * 100);
	TxData[CAN_MSG_MSC19_1_ADC_AVG_H_BYTE] = (uint8_t)((uint16_t)(avg * 100) >> 8);
	/* Minimum */
	TxData[CAN_MSG_MSC19_1_ADC_MIN_L_BYTE] = (uint8_t)(min * 100);
	TxData[CAN_MSG_MSC19_1_ADC_MIN_H_BYTE] = (uint8_t)((uint16_t)(min * 100) >> 8);
	/* Maximum */
	TxData[CAN_MSG_MSC19_1_ADC_MAX_L_BYTE] = (uint8_t)(max * 100);
	TxData[CAN_MSG_MSC19_1_ADC_MAX_H_BYTE] = (uint8_t)((uint16_t)(max * 100) >> 8);

	

	
	if (HAL_CAN_AddTxMessage(can_hardware.hcan, &TxHeader, TxData, &pTxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		//  Error_Handler();
	}
}

void can_send_task(void)
{
	static uint32_t can_i_bat_clk_div = 0;
	static uint32_t can_i_mot_clk_div = 0;
	const volatile measurement_t *i_b = &adc_get_measurements()->i_b;

	if (++can_i_bat_clk_div >= CAN_I_BAT_CLK)
	{
		can_i_bat_clk_div = 0;

		// Input current
		can_send_measurements(
			CAN_MSG_MSC19_4_ADC_ID,
			CAN_SIGNATURE_MSC19_4,
			i_b->avg < 0 ? -i_b->avg : 0,
			i_b->min < 0 ? -i_b->min : 0,
			i_b->max < 0 ? -i_b->max : 0
		);
		
		// Output current
		can_send_measurements(
			CAN_MSG_MSC19_5_ADC_ID,
			CAN_SIGNATURE_MSC19_5,
			i_b->avg > 0 ? i_b->avg : 0,
			i_b->min > 0 ? i_b->min : 0,
			i_b->max > 0 ? i_b->max : 0
		);
	}

}