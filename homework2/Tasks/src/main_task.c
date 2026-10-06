#include "main_task.h"
#include "main.h"
#include "can.h"


extern CAN_HandleTypeDef hcan;


//发送第一个报文

static void CAN_SendTest (void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
    uint32_t TxMailbox;


    TxHeader.StdId = 0x200;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

   HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

void TaskInit (void)
{
    CAN_FilterTypeDef sFilterConfig;

    // 配置过滤器：接收所有报文（回环自测必须！）
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
    CAN_SendTest();
}





void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    static uint32_t lastToggle = 0;
    static uint8_t  first = 1;

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            uint32_t now = HAL_GetTick();

            if (first || (now - lastToggle >= 500))
                {
                    first = 0;
                    lastToggle = now;
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                }

            CAN_SendTest();
        }
}