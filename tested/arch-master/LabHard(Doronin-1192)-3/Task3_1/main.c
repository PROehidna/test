//------------------------------
//
//
//------------------------------

#include "main.h"

static uint8_t buf[256]; /*буфер данных, передаваемый на пк посредством USART*/
static uint32_t iReadyTX, iCompleteTX; /*количество битовых пакетов готовых для передачи и переданных на пк соответсственно*/

int main()
{
	uint32_t half_period, n;
	
	__disable_irq();
	
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 |GPIO_MODER_MODER8_0;
	GPIOB->MODER&=~(GPIO_MODER_MODER12 | GPIO_MODER_MODER12);
	GPIOB->ODR|=0x100;
	
	iReadyTX = 0;
	iCompleteTX = 0;
	InitUSART1();
	NVIC->ISER[0] |= 0x08000000;
	__enable_irq();
	
	half_period=50000;
	while(1){
		n=((GPIOB->IDR)&0x3000)>>12;
		GPIOB->BSRR=0x1;
		delay(half_period<<n);
		GPIOB->BSRR=0x10000;
		delay(half_period<<n);
	}
}

void InitUSART1(void)
{
	
	RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	GPIOA->MODER |= 0x00280000;
	GPIOA->AFR[1] |= 0x00000110;
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;
	
	USART1->CR1 &= ~USART_CR1_UE;
	USART1->BRR = 69;
	
	
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
	
	
	
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_TCIE;
	
	
	USART1->CR2 = 0;
	USART1->CR3 = 0;
	USART1->CR1 |= USART_CR1_UE;
	
}

void delay(uint32_t count)
{
	for (uint32_t i = 0; i < count; i++);
}

void USART1_IRQHandler(void)
{
	uint8_t pack, d100;
	
	if (USART1->ISR & USART_ISR_RXNE){
		pack=(uint8_t)USART1->RDR;
		d100=(uint8_t)pack/100;
		pack-=d100*100;
		buf[(uint8_t)iReadyTX++] = d100+48;
		
		buf[(uint8_t)iReadyTX++] = (uint8_t)pack/10+48;
		
		buf[(uint8_t)iReadyTX++] = (uint8_t)pack%10+48;
		
		buf[(uint8_t)iReadyTX++] = 32;
		
		while((USART1->ISR & USART_ISR_TXE) == 0){}
		USART1->TDR = buf[(uint8_t)iCompleteTX++];
	}
	
	if (USART1->ISR & USART_ISR_TC) {
		
		USART1->ICR=USART_ICR_TCCF;
		
		if (iCompleteTX<iReadyTX) {
			USART1->TDR = buf[(uint8_t)iCompleteTX++];
		}
	}
}


