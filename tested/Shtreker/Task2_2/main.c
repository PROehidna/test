//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Разработчит проекта: Штрекер Владислав Александрович, студент группы 1193б.
//
//Цель: разработать программу для принятия пакетов данных ПК по USART и возвращения их обратно.
//
//Решамые проектом задачи:
//	1. Реализация сервера (стенд STM_01) для принятия пакетов данных и отправки из обратно в теримнал PuTTY.
//	2. Реализация сервера удаленного контроля светодиодов.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "main.h" 																																	/* Заголовчоный файл с описанием подключаемых библиотечных модулей 																																												     	*/
																																										/*																																																																											   	    */																																																																																																																	 /* */																																																																																																																																															 /* */
																																										/* main() - обязательная функция для исполнения кода пользователя. 																																											   	    */
																																										/* После подачи питания или щелчка кнопкой "reset" стартует Загрузчик, который выполняет начальную настройку основых регистров микроконтроллер						 	    */
																																										/* В завершение работы Загрузчик передаёт управление микроконтроллером функции main. 																																		   	    */
																																										/*																																																																											   			*/																																																																																																																	 /* */
int main(void)																																			/*  																																																																														*/
{																																										/*  																																																																														*/
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;																									/* Включение тактирования порта B RCC_AHBENR_GPIOBEN=0x00040000 -> 18 бит данного регистра отвечает за вкл./выкл. тактирования порта B. 												*/
	GPIOB->MODER|=0x15555;																														/* Переключение линий 0-8 порта B в режим "Output". 																																																						*/
	InitUSART1();																																			/*  																																																																														*/
	GPIOB->ODR|=0x100;																																/* Разрешение работы светодиодов на стенде STM_01 с помощью установки логической "1" на выводе PB.8. 																														*/
																																										/*  																																																																														*/
	uint8_t num[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};									/* Массив, хранящий HEX кодировку ASCII символов от "1" до "8". 																																																*/
	uint8_t status[] = {0, 0, 0, 0, 0, 0, 0, 0};																			/* Массив, хранящий состояние диодов.																																																														*/
	uint8_t diode[] = {0x1, 0x2, 0x4, 0x80, 0x40, 0x20, 0x10, 0x8};										/* Массив, хранящий данные о битах для включения диодов.  																																																			*/
	uint16_t i;																																				/* Переменная счетчика индекса для предыдущих массивов. 																																																				*/
																																										/*  																																																																														*/
	while(1)																																					/*  																																																																														*/
	{																																									/* Получение данных из USART. 																																																																	*/
		while ((USART1->ISR & USART_ISR_RXNE) == 0) {}																	/* Чтение регистров ISR и анализ флага RXNE: RXNE выставляется USART в 1, когда новый пакет данных получен приемником Rx и скопирован в RDR. 										*/
		uint8_t d = (uint8_t)USART1->RDR;																								/* Кодирование данных из регистра USART (регистр RDR) в программную переменную. 																																								*/
		for (i = 0; i<8; i++)																														/* Цикл для провекри всех элементов массива.  																																																									*/
		{																																								/*  																																																																														*/
			uint8_t msg[] = {0x64, 0x69, 0x6f, 0x64, 0x65, 0x2d, num[i], 									/* Массив, хранящий сообщение, выводящееся в терминал PuTTY: 																																																		*/
											 0x20, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 										/* "diode-X status is Y" 																																																																				*/
											 0x20, 0x69, 0x73, 0x20, status[i] + 48, 0x0a, 0x0d, 0x00};		/*  																																																																														*/
			if (d == num[i])																															/* Проверка на соответствие вводимого символа элемента массива кодировокю 																																											*/
			{																																							/*  																																																																														*/
				if (status[i] == 0)																													/* Проверка на статус диода. 																																																																		*/
				{																																						/*  																																																																														*/
					status[i] = 1;																														/* Переключение статуса. 																																																																				*/
					msg[18] = status[i] + 48;																									/* Присвоение массиву сообщения данных о статусе. 																																																							*/
					GPIOB->ODR|=diode[i];																											/* Зажигание диода. 																																																																						*/
					for (int32_t j = 0; j<22; j++)																						/* Вывод сообщения. 																																																																						*/
					{																																					/*  																																																																														*/
						while ((USART1->ISR & USART_ISR_TXE) == 0) {}														/*  																																																																														*/
						USART1->TDR = msg[j];																										/*  																																																																														*/
					}																																					/*  																																																																														*/
				}																																						/*  																																																																														*/
				else if (status[i] == 1)																										/* Проверка на статус диода. 																																																																		*/
				{																																						/*  																																																																														*/
					status[i] = 0;																														/* Переключение статуса. 																																																																				*/
					msg[18] = status[i] + 48;																									/* Присвоение массиву сообщения данных о статусе. 																																																							*/
					GPIOB->BRR|=diode[i];																											/* Выключение диода. 																																																																						*/
					for (int32_t j = 0; j<22; j++)																						/* Вывод сообщения. 																																																																						*/
					{																																					/*  																																																																														*/
						while ((USART1->ISR & USART_ISR_TXE) == 0) {}														/*  																																																																														*/
						USART1->TDR = msg[j];																										/*  																																																																														*/
					}																																					/*  																																																																														*/
				}																																						/*  																																																																														*/
			}																																							/*  																																																																														*/
			if (d == 0x30 || d == 0x39)																										/* Проверка на клавиши "9" и "0" 																																																																*/
			{																																							/*  																																																																														*/
				if (d == 0x30)																															/* Дополнительная проверка на клвишу "0" выключения диодов. 																																																		*/
				{																																						/*  																																																																														*/
					for (int32_t j = 0; j<8; j++)																							/* Цикл, выключающий диоды и обновляющий статус.  																																																							*/
					{																																					/*  																																																																														*/
						GPIOB->BRR|=diode[j];																										/* Выключение диодов. 																																																																					*/
						status[j] = 0;																													/* Обновление статуса.  																																																																				*/
					}																																					/*  																																																																														*/
				}																																						/*  																																																																														*/
				for (int32_t k = 0; k<22; k++)																							/* Вывод сообщения. 																																																																						*/
				{																																						/*  																																																																														*/
					while ((USART1->ISR & USART_ISR_TXE) == 0) {}															/*  																																																																														*/
					USART1->TDR = msg[k];																											/*  																																																																														*/
				}																																						/*  																																																																														*/
			}																																							/*  																																																																														*/
		}																																								/*  																																																																														*/
		i = 0;																																					/* Обнуление счетчика 																																																																					*/
	}																																									/*  																																																																														*/
}																																										/*  																																																																														*/
																																										/*  																																																																														*/
void InitUSART1(void)																																/* Функция инициализации USART лабораторного комплекса STM0_1.                                                                                                  */
{																																										/*  																																																																												    */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;																							/* Включение тактирования USART1. RCC_APB2ENR_USARRT1EN=0x00004000.                                                                                             */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;																								/* Включение тактирования порта A. RCC_AHBENR_GPIOAAEN=0x00020000.                                                                                              */
																																										/* Настройка линий порта A: PA9(TX_1) - выход передатчика; PA10 (RX_1) - вход приемника.                                                                        */
	GPIOA->MODER |= 0x00280000;																												/* Перевести линии PA.9 и PA.10 в режим альтернативной функции.                                                                                                 */
	GPIOA->AFR[1] |= 0x00000110;																											/* Включить на линиях PA.9 и PA.10 альтернативнуж функцию AF1.                                                                                                  */
																																										/* Настройка линии передатчика Tx (PA.9).                                                                                                                       */
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;																								/* Сбросить 9 бит GPIOA->OTYPER - переключение в режим push-pull для линии PA.9 (активный выход).                                                               */
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;																								/* Отключение подтягивающих резисторов для линии PA.9.                                                                                                          */
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;																					/* Установка высокой скорости синхронизации для линии PA.10.                                                                                                    */
																																										/* Настрйока линии приемника Rx (PA.10).                                                                                                                        */
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;																							/* Сброс режима подтягивающих резисторов для линии PA.9.                                                                                                        */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;																							/* Включение подтягивающего резистора pull-up на входной линии PA.10 (вход приемника Rx)                                                                        */
																																										/* Конфигурирование USART.                                                                                                                                      */
	USART1->CR1 &= ~USART_CR1_UE;																											/* Запрещение работы модуля USART1 для изменения параметров его конфигруации.                                                                                   */
	USART1->BRR = 69;																																	/* Настройка делителя частоты тактирующего USART и задающего скорость приема и передачи данных на уровне 115200 бит/с: Частота тактирующего генератора = 8 МГц; */
																																										/* Скорость обмена по USART = 115200 бит/с; коэффициент деления = 8000000 / 115200 = 69,4444(4); Округленное значение = 69.                                     */
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;																				/* Разрешить работу приемника и передатчика USART. Остальные биты этого регистра сброшены, что обесипечивает: количество бит данных в пакете = 8;               */
																																										/* контроль четности - отключен; прерывания по любым флагам USART - запрещены; состояние USART - отключен.                                                      */
	USART1->CR2 = 0;																																	/* Количетсво стоп-бит - 1.                                                                                                                                     */
	USART1->CR3 = 0;																																	/* DMA1 - отключен                                                                                                                                              */
	USART1->CR1 |= USART_CR1_UE;																											/* По завершении конфигурирования USART разрешить его работу (биту UE регистра CR1 присвоить 1).                                                                */
}																																										/* Примечание: конфигурирование USART можно выполнить только в не активном состояниии: UE=0.                                                                    */																																										/*  */
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Руководство пользователя:
//		1. Для запуска загруженной программы удалите перемычку "boot" на стенде и нажмите кнопку "reset".
//		2. Для запуска терминала сконфигурирутйе PuTTY на входную скорость в 115200 бодов и подлкючитесь к нужному COM-порту.
//		3. При нажатии на клавиши "1"-"8" будет включаться/выключаться соответсвующий порядковому номеру светодиод, при счете слева направо, и выводиться сообщение о статусе диода.
//		4. При нажатии на клаивши "0"-"9" будет выводить сообщение о статусе всех светодиодов, дополнительно при нажатии "0" будет происходить выключние всех светодиодов.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
