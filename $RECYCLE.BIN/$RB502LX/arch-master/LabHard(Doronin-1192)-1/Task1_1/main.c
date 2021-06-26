#include "main.h"

int main(void)
{
	uint32_t half_period;
	uint32_t n;
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER8_0;
	
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13);
	GPIOB->ODR|=0x100;
	half_period = 50000;
	while(1)
	{
		n=((GPIOB->IDR)&0x3000)>>12;
		GPIOB->BSRR|=0x1;
		delay(half_period<<n);
		GPIOB->BSRR|=0x10000;
		delay(half_period<<n);
	}
}

void delay(uint32_t count)
{
for (uint32_t i=0;i<count;i++);
}
