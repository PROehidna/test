//------------------------------
//
//
//------------------------------
#include "main.h"
static uint16_t temp = 5;
static int8_t counter = 0;
static uint16_t half_period_array[] = {0, 5, 0, 0, 0, 0};
static uint16_t temp_array[] = {0, 0, 0, 0, 0, 0};
static uint16_t cur_value[] =  {208, 191, 208, 181, 209, 128, 208, 184, 208, 190, 208, 180, 32, 208, 188, 208, 184, 208, 179, 208, 176, 208, 189, 208, 184, 209, 143, 44, 32, 208, 188, 208, 186, 209, 129, 58, 32, 0};
static uint16_t new_value[] =  {208, 189, 208, 190, 208, 178, 208, 190, 208, 181, 32, 208, 183, 208, 189, 208, 176, 209, 135, 208, 181, 208, 189, 208, 184, 208, 181, 44, 32, 208, 188, 208, 186, 209, 129, 58, 32,};
static uint8_t buf[256]; /*буфер данных, передаваемый на пк посредством USART*/
static uint32_t iReadyTX, iCompleteTX; /*количество битовых пакетов готовых для передачи и переданных на пк соответсственно*/

int main()
{	
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
	
	while(1){		
		uint32_t half_period = 0;
		for (int32_t i = 5, j = 0; i >-1; i --, j++){
			half_period += half_period_array[i] * pov(10, j);
		}
		GPIOB->BSRR=0x1;
		delay(half_period);
		GPIOB->BSRR=0x10000;
		delay(half_period);
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


// 027 091 065 - up								 d == (027 && 091 && 065)
// 027 091 066 - down
// 048 ... 057 numbers
// 127 - bcakspace
// 032 - space
// 013 - enter

void del(void){
	while((USART1->ISR & USART_ISR_TXE) == 0){}
	USART1->TDR = 27;
	while((USART1->ISR & USART_ISR_TXE) == 0){}
	USART1->TDR = 91;
	while((USART1->ISR & USART_ISR_TXE) == 0){}
	USART1->TDR = 65;
	counter = 0;
	temp = 0;
}

void USART1_IRQHandler(void)
{
	uint8_t pack;
	if (USART1->ISR & USART_ISR_RXNE){
		pack=(uint8_t)USART1->RDR;
			if (pack == 13){
				temp = 5;
				for (int i = 0; i < 6; i ++){
					half_period_array[i] = 0;
				}
				while (counter > 0){
					half_period_array[6-counter] = temp_array[temp];
					while((USART1->ISR & USART_ISR_TXE) == 0){}
					USART1->TDR = 127;
					temp--;
					counter--;
				}
				del();
				while((USART1->ISR & USART_ISR_TXE) == 0){}
				USART1->TDR = 13;
				for (int i = 0; i < 38; i ++){
					while((USART1->ISR & USART_ISR_TXE) == 0){}
					USART1->TDR = cur_value[i];
				}
				for (int i = 0; i < 6; i ++){
					while((USART1->ISR & USART_ISR_TXE) == 0){}
					USART1->TDR = half_period_array[i] + 48;
				}	
				while((USART1->ISR & USART_ISR_TXE) == 0){}
				USART1->TDR = 10;
				while((USART1->ISR & USART_ISR_TXE) == 0){}
				USART1->TDR = 13;

				for (int i = 0; i < 38; i ++){
					while((USART1->ISR & USART_ISR_TXE) == 0){}
					USART1->TDR = new_value[i];
				}
				for (int i = 0; i < 6; i ++){
					temp_array[i] = 0;
				}
			}	
			else if (pack == 127 && counter > 0){
				while ((USART1->ISR & USART_ISR_TXE) == 0) {}			
				USART1->TDR = pack;
				counter--;
			}
			else if ((pack >=0x30 && pack <= 0x39) && counter < 6){
				while ((USART1->ISR & USART_ISR_TXE) == 0) {}
				USART1->TDR = pack;
				temp_array[5-counter] = pack - 48;
				counter++;
			}
			
	}
	
	if (USART1->ISR & USART_ISR_TC) {
		
		USART1->ICR=USART_ICR_TCCF;
		
		if (iCompleteTX<iReadyTX) {
			USART1->TDR = buf[(uint8_t)iCompleteTX++];
		}
	}
}

uint32_t pov(int32_t x, int32_t n)																																												/* pov() - функция вычисления степени числа (находится в открытом доступе). 																					                      			 	 																																											*/
{																																																													  /* Входные данные: x - число, возодимое в степень 																																		                      			 	 																																											*/
    if (n==0)																																																								/* Если степень равна 0  																																															                      			 	 																																											*/
        return 1; 																																																					/*  										возвращается 1. 																																							                      			 	 																																											*/
    else if (n==1	)																																																					/* Если степень равна 1  																																															                      			 	 																																											*/
        return x;																																																						/*                      возвращается число. 																																					                      			 	 																																											*/
    else if (n % 2 == 0 )																																																		/* Если степень четная   																																															                      			 	 																																											*/
			return pov( x * x, n/2);																																														  /*                     производится рекурсия, во входных параметрах число умноженное само на себя и половина степени.                       			 	 																																											*/
    else																																																										/* Если степень нечетная 																																															                      			 	 																																											*/
        return pov( x * x, n /2)*x;																																													/*                       производится рекурсия, во входных параметрах число умноженное само на себя и половина степени. Результат умножается на число																																											*/
 }																																																													/*  																																																																																																																			*/
