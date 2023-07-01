#ifndef CAN_APP_H
#define CAN_APP_H

#include "main.h"

typedef struct
{
    CAN_HandleTypeDef *hcan;
    uint32_t fifo;
} can_hardware_t;

void can_init(CAN_HandleTypeDef *hcan);
void can_send_task(void);

#endif