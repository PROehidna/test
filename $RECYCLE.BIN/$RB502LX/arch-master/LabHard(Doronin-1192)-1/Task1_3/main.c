#include "main.h"

int main(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER9_0;
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13 |  GPIO_MODER_MODER14 |  GPIO_MODER_MODER15);
	uint16_t bitness = 14;
	uint16_t in[8] = {0, 1, 0, 0, 1, 0, 0, 0} ;
	uint16_t n = 0;
	uint16_t out[3] = {0,0,0};
	

	unsigned reg[14] = {0x23F, 0x206, 0x25B, 0x24F, 0x266, 0x26D, 0x27D, 0x207, 0x27F, 0x26F, 0x277, 0x27C, 0x239, 0x25E};
	
	while(1){
		n=0;
		in[0]=((GPIOB->IDR)&0x8000)>>15;
		in[2]=((GPIOB->IDR)&0x4000)>>14;
		in[5]=((GPIOB->IDR)&0x2000)>>13;
		in[6]=((GPIOB->IDR)&0x1000)>>12;
		
		uint32_t j=7;
	
		for (uint16_t i = 0 ; i<8; i++){         //to 10
			n += in[i]* powi (2, j);
			j--;
		}
		
		for (uint16_t i = 0 ; i<bitness; i++){         //to 14		
			if(n<bitness) {
				out[i] = n;	
				break;
			}
			out[i] = n%bitness;
			n = n/bitness;
		} 
		
		for (int16_t i = 2; i>-1; i--){
			if(i == 2 & out[i]== 0){
				continue;}
			GPIOB->BSRR|=reg[out[i]];
			delay(200000);
			GPIOB->BSRR|=0xffff0000;			
			delay(50000);
		}
		GPIOB->BSRR|=0x00000280;
		delay(500000);
		GPIOB->BSRR|=0xffff0000;	
	}
}


uint32_t powi(uint32_t x, uint32_t n)
 {
    if (n==0)
        return 1;
    else if (n==1)
        return x;
    else if (n % 2 == 0 )
        return powi( x * x, n/2);
    else
        return powi( x * x, n /2)*x;
 }
 
void delay(uint32_t count)
{
	volatile uint32_t i;
	for (i =0;i<count;i++);
}
