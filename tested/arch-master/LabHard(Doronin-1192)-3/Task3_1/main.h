#include "stm32f0xx.h"				/*Подключение библиотеки с моделью stm32f0xx																*/
void InitUSART1(void);				/*Декларация функции инициализации USART1																		*/
void USART1_IRQHandler(void); /*Декларация функции обработки прерывания от USART1													*/
void delay(uint32_t);  				/*Функция задержки: count– кол-во периодов задержки длительностью 2.5 мкс*	*/
