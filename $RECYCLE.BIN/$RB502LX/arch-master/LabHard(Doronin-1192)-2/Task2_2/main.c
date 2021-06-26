#include "main.h"


int main(void)
{
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER8_0;
	InitUSART1();
	GPIOB->ODR|=0x100;
	bool flag = true;
	while(1)
	{
		while ((USART1->ISR & USART_ISR_RXNE) == 0) {}
		uint8_t d = (uint8_t)USART1->RDR;
		if (d == 0x31)
		{
			if (flag == true)
			{
				GPIOB->ODR|=0x1;
				flag = !flag;

				USART1->TDR = 0x62;
					while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x69;
										while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x74;
															while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x31;
																				while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x2B;
																									while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x0A;
																														while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x0D;
																																			while ((USART1->ISR & USART_ISR_TXE) == 0) {}
			}
			else
			{
				GPIOB->BRR|=0x1;
				flag = !flag;
				USART1->TDR = 0x62;
					while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x69;
										while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x74;
															while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x31;
																				while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x2D;
																									while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x0A;
																														while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = 0x0D;
																																			while ((USART1->ISR & USART_ISR_TXE) == 0) {}
			}
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
	USART1->BRR = 69;
	
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
	
	USART1->CR2 = 0;
	USART1->CR3 = 0;
	USART1->CR1 |= USART_CR1_UE;
}
