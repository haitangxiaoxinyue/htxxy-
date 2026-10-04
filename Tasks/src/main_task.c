#include "main_task.h"
#include "main.h"

volatile uint32_t tick = 0;

extern TIM_HandleTypeDef htim2;
extern IWDG_HandleTypeDef hiwdg;

void TaskInit (void)
{

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
    HAL_TIM_Base_Start_IT(&htim2);


}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if(htim->Instance == TIM2)
        {
            tick++;
            HAL_IWDG_Refresh(&hiwdg);

        }


}

