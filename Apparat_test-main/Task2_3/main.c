#include "main.h"
int main(void)
{
	uint16_t i=7; // чтобы не было вывода незначимых нулей 
	InitUSART1();
	uint16_t message[11]={0x63,0x68,0x74,0x61,0x2d,0x35,0x3e,0,0,0,0}; //сообщение chta-5>
	while(1)
	{
		message[10] = 0;
		message[9] = 0;
		message[8] = 0;
		message[7] = 0;
		while ((USART1->ISR & USART_ISR_RXNE) == 0) { }
		uint8_t d = (uint8_t)USART1->RDR;
		for (int8_t s=0;s<11;s++)
		{				
			while ((USART1->ISR & USART_ISR_TXE) == 0) { }
			USART1->TDR = 0x7f;			
		}
		for(uint16_t k=0;d!=0;k++)
		{
			message[10-k]=(d%5)+48;
			d=d/5;
		}
		i=7;
		for (int32_t s=0;s<11;s++)
		{				
			if(s==i&&s==48)
			{
				i++;
				continue;
			}
			while ((USART1->ISR & USART_ISR_TXE) == 0) { }
			USART1->TDR = message[s];			
		}		
	}
}


void InitUSART1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 0x00280000;
	GPIOA->AFR[1] |= 0x00000110;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;
	USART1->CR1 &= ~USART_CR1_UE;
	USART1->BRR=69; // скорость обмена
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
	USART1->CR1 |= 0x400; // контроль четности
	USART1->CR2 = 0x2000; // стоп-биты
	USART1->CR3 = 0;
	USART1->CR1 |= USART_CR1_UE;
}
