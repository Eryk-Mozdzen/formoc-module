#include "config.h"

void Clock_Init() {
    RCC->CR |=RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    RCC->APB1ENR1 |=RCC_APB1ENR1_PWREN;
    PWR->CR1 |=(1<<PWR_CR1_VOS_Pos);
    FLASH->ACR |=FLASH_ACR_LATENCY_4WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    RCC->PLLCFGR |=(10<<RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLSRC_HSI | RCC_PLLCFGR_PLLREN;

    RCC->CR |=RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    //RCC->CFGR |=RCC_CFGR_SW_PLL;
    //while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
	(void)pcTaskName;
	(void)pxTask;

	taskDISABLE_INTERRUPTS();
	while(1);
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationMallocFailedHook() {

}

void UsageFault_Handler() {
    while(1);
}

void BusFault_Handler() {
    while(1);
}

void MemMang_Handler() {
    while(1);
}

void HardFault_Handler() {
    while(1);
}