#include <stdint.h>

#include "stm32l4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "config.h"

void ledTask(void *param) {
    (void)param;

    while(1) {
        GPIOA->BSRR |=GPIO_BSRR_BS5;
        vTaskDelay(1000);
        
        GPIOA->BSRR |=GPIO_BSRR_BR5;
        vTaskDelay(1000);
    }
}

int main() {

    NVIC_SetPriorityGrouping(0);

    // PA5
    RCC->AHB2ENR |=RCC_AHB2ENR_GPIOAEN;
    GPIOA->MODER &=~(2<<GPIO_MODER_MODE5_Pos);
    GPIOA->MODER |=GPIO_MODER_MODER5_0;

    Clock_Init();

    xTaskCreate(ledTask, "led", 1024, NULL, 4, NULL);

    vTaskStartScheduler();

    return 0;
}