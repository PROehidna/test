//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Разработчит проекта: Доронин Всеволод Михайлович, студент группы 1192б.
//
//Цель: разработать программу для принятия пакетов данных ПК по USART и возвращения их обратно.
//
//Решамые проектом задачи:
//	1. Реализация сервера (стенд STM_01) для принятиz пакетов данных и отправки из обратно в теримнал PuTTY.
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "main.h" 																						/* Заголовчоный файл с описанием подключаемых библиотечных модулей 																																												     	*/
																															/*																																																																											   	    */																																																																																																																	 /* */																																																																																																																																															 /* */
																															/* main() - обязательная функция для исполнения кода пользователя. 																																											   	    */
																															/* После подачи питания или щелчка кнопкой "reset" стартует Загрузчик, который выполняет начальную настройку основых регистров микроконтроллер						 	    */
																															/* В завершение работы Загрузчик передаёт управление микроконтроллером функции main. 																																		   	    */
																															/*																																																																											   	    */																																																																																																																	 /* */
int main(void)																								/*  																																																																												    */
{																															/*  																																																																												    */
	InitUSART1();																								/* Инициализация USART1 лабораторного комплекса STM_01. 																																									                      */
																															/*  																																																																												    */
	while(1)																										/*  																																																																												    */
	{																														/* Получение данных из USART. 																																																						                      */
		while ((USART1->ISR & USART_ISR_RXNE) == 0) {}						/* Чтение регистров ISR и анализ флага RXNE: RXNE выставляется USART в 1, когда новый пакет данных получен приемником Rx и скопирован в RDR.                    */
		uint8_t d = (uint8_t)USART1->RDR;													/* Кодирование данных из регистра USART (регистр RDR) в программную переменную. 																													                      */
																															/* Чтение регистра RDR приводит к сбросу флага RXNE=0. После чего USART снова получает возможность просигналить программе о получении нового пакета от ПК.      */
																															/* Отправка данных в USART.   																																																						                      */
		while ((USART1->ISR & USART_ISR_TXE) == 0) {}							/* Чтение регистра состояния ISR и анализ флага TXE: TXE=0, пока данные из регистра TDR не скопированы в сдвиговый регистр передатчика Tx.                      */
																															/* TXE автоматчиески выставлеят USART в 1, когда копирование из TDR в сдвиговый регистра завершено и регистр TDR готов к получению следующего пакета.           */
		USART1->TDR = d;																					/* Передача данных из программной переменной в USART (регистр TDR).                                                                                             */
			}																												/* Запись в регистр TDR приводит к сбросу флага TXE=0. 					                                                                                                */
}																															/*  																																																																												    */
																															/*  																																																																												    */
void InitUSART1(void)																					/* Функция инициализации USART лабораторного комплекса STM0_1.                                                                                                  */
{																															/*  																																																																												    */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;												/* Включение тактирования USART1. RCC_APB2ENR_USARRT1EN=0x00004000.                                                                                             */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;													/* Включение тактирования порта A. RCC_AHBENR_GPIOAAEN=0x00020000.                                                                                              */
																															/* Настройка линий порта A: PA9(TX_1) - выход передатчика; PA10 (RX_1) - вход приемника.                                                                        */
	GPIOA->MODER |= 0x00280000;																	/* Перевести линии PA.9 и PA.10 в режим альтернативной функции.                                                                                                 */
	GPIOA->AFR[1] |= 0x00000110;																/* Включить на линиях PA.9 и PA.10 альтернативнуж функцию AF1.                                                                                                  */
																															/* Настройка линии передатчика Tx (PA.9).                                                                                                                       */
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;													/* Сбросить 9 бит GPIOA->OTYPER - переключение в режим push-pull для линии PA.9 (активный выход).                                                               */
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;													/* Отключение подтягивающих резисторов для линии PA.9.                                                                                                          */
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;										/* Установка высокой скорости синхронизации для линии PA.10.                                                                                                    */
																															/* Настрйока линии приемника Rx (PA.10).                                                                                                                        */
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;												/* Сброс режима подтягивающих резисторов для линии PA.9.                                                                                                        */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;												/* Включение подтягивающего резистора pull-up на входной линии PA.10 (вход приемника Rx)                                                                        */
																															/* Конфигурирование USART.                                                                                                                                      */
	USART1->CR1 &= ~USART_CR1_UE;																/* Запрещение работы модуля USART1 для изменения параметров его конфигруации.                                                                                   */
	USART1->BRR = 69;																						/* Настройка делителя частоты тактирующего USART и задающего скорость приема и передачи данных на уровне 115200 бит/с: Частота тактирующего генератора = 8 МГц; */
																															/* Скорость обмена по USART = 115200 бит/с; коэффициент деления = 8000000 / 115200 = 69,4444(4); Округленное значение = 69.                                     */
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;									/* Разрешить работу приемника и передатчика USART. Остальные биты этого регистра сброшены, что обесипечивает: количество бит данных в пакете = 8;               */
																															/* контроль четности - отключен; прерывания по любым флагам USART - запрещены; состояние USART - отключен.                                                      */
	USART1->CR2 = 0;																						/* Количетсво стоп-бит - 1.                                                                                                                                     */
	USART1->CR3 = 0;																						/* DMA1 - отключен                                                                                                                                              */
	USART1->CR1 |= USART_CR1_UE;																/* По завершении конфигурирования USART разрешить его работу (биту UE регистра CR1 присвоить 1).                                                                */
}																															/* Примечание: конфигурирование USART можно выполнить только в не активном состояниии: UE=0.                                                                    */
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Руководство пользователя:
//		1. Для запуска загруженной программы удалите перемычку "boot" на стенде и нажмите кнопку "reset".
//		2. Для запуска терминала сконфигурирутйе PuTTY на входную скорость в 115200 бодов и подлкючитесь к нужному COM-порту.
//		3. При нажатии на клавишу в терминал будет передаваться символ, соответстующий клавише.
//		4. Управление местом написания осуществлять с помощью стрелок клавиатуры.
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
