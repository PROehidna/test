#include "main.h"

int main(void)
{
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	uint32_t half_period; 
	uint32_t sw1; uint32_t sw2; uint32_t sw3; uint32_t sw4;
	uint32_t btn = 14;
	sw4=((GPIOB->IDR)&0x1000)>>12; sw3=((GPIOB->IDR)&0x2000)>>13; sw2=((GPIOB->IDR)&0x4000)>>14; sw1=((GPIOB->IDR)&0x8000)>>15;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER9_0;
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15);
	GPIOB->ODR|=0x200;
	half_period = 50000;
	while(1)
	{
		uint32_t bit[] = {sw1, 1, sw2, 0, 1, sw3, 0, sw4};
		uint32_t num[8];
		uint32_t dec = 0; uint32_t div; uint32_t j = 7;
		for (uint32_t i = 0; i < 8; i++)
		{
			dec += (bit[i] * power(2, j));
			j--;
		}
		for (uint32_t i = 0; i < 9; i++)
		{
			div = dec % btn;
			num[i] = div;
			dec -= div;
			dec = dec / btn;
			if (dec < btn)
			{
				i++;
				num[i] = dec;
				if (dec < btn)
				{
					break;
				}
			}
		}
		j = 0;
		uint32_t o;
		for (uint32_t i = 7; i >= 0; i--)
		{
			o = num[i];
			if (j ==0)
			{
				j = num[i];
				if (j != 0)
				{
					output(o);
					delay(200000);
					GPIOB->ODR|=0x200;
					delay(50000);
				}
			}
			else
			{
				output(o);
				delay(200000);
				GPIOB->ODR|=0x200;
				delay(50000);
			}
		}
		GPIOB->BSRR|=0x80;
		//горит тчк
		delay(500000);
		GPIOB->ODR|=0x200;
		delay(500000);
		}
}

void delay(uint32_t count)
{
for (uint32_t i=0;i<count;i++);
}

void output(uint32_t count)
{
	uint32_t zero = 0x3f;
	uint32_t one = 0x6;
	uint32_t two = 0x5b;
	uint32_t three = 0x4f;
	uint32_t four = 0x66;
	uint32_t five = 0x6d;
	uint32_t six = 0x7d;
	uint32_t seven = 0x7;
	uint32_t eight = 0x8;
	uint32_t nine = 0x6f;
	uint32_t a = 0x77;
	uint32_t b = 0x7c;
	uint32_t c = 0x39;
	uint32_t d = 0x5e;
	uint32_t e = 0x79;
	uint32_t f = 0x71;
	uint32_t dot = 0x80;	
}

uint32_t power(uint32_t x, uint32_t n)
 {
    if (n==0)
        return 1;
    else if (n==1)
        return x;
    else if (n % 2 == 0 )
        return power( x * x, n/2);
    else
        return power( x * x, n /2)*x;
 }
 
