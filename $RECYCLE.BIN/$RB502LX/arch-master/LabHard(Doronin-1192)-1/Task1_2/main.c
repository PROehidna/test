#include "main.h"

int main(void)
{
	uint32_t half_period;
	uint32_t n;
	uint32_t swreg;
	uint32_t swrev;
	uint32_t l;
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	GPIOB->OSPEEDR|=0X5555; //все порты на скорость в 10 мгц - 0b0101010101010101
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15);
	GPIOB->ODR|=0x100;
	half_period = 50000;
	uint32_t on[] = {0x1, 0x2, 0x4, 0x80, 0x40, 0x20, 0x10, 0x8};
	uint32_t off[] = {0x10000, 0x20000, 0x40000, 0x800000, 0x400000, 0x200000, 0x100000, 0x80000}; 
	while(1)
	{
		n=((GPIOB->IDR)&0x3000)>>12;
		swreg=((GPIOB->IDR)&0x8000)>>15;
		swrev=((GPIOB->IDR)&0x4000)>>14;
		if(swrev!=1)
		{
			if(swreg!=1)
			{
				GPIOB->BSRR=0x100;			
				for (uint32_t i = 0; i<8; i++)
				{
					GPIOB->BSRR=on[i];
					delay(half_period<<n);
				}
				for (uint32_t j =0; j<8; j++)
				{
					GPIOB->BSRR=off[j];
					delay(half_period<<n);
				}			
			}
			else
			{
				GPIOB->BSRR=0x100;
				for (uint32_t i = 0; i<8; i++)
				{
					GPIOB->BSRR=on[i];
					delay(half_period<<n);
					GPIOB->BSRR=off[i];
				}
			}
		}
		else
		{
			{
			if(swreg!=1)
			{
				GPIOB->BSRR=0x100;
				for (int32_t i = 7; i > -1; i--)
				{
					GPIOB->BSRR=on[i];
					delay(half_period<<n);
				}
				for (int32_t j = 7; j > -1; j--)
				{
					GPIOB->BSRR=off[j];
					delay(half_period<<n);
				}			
			}
			else
			{
				GPIOB->BSRR=0x100;
				for (int32_t i = 7; i > -1; i--)
				{
					GPIOB->BSRR=on[i];
					delay(half_period<<n);
					GPIOB->BSRR=off[i];
				}
			}
			}
		}
	}
}

void delay(uint32_t count)
{
for (uint32_t i=0;i<count;i++);
}
