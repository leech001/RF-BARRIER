# STM32 HAL library for reading and sending code for radio frequency (RF) barrier (Came, Nice) simple code 12, 24 bit and HCS301 with keeloq 66 bit.

## English note
A simple C library (STM32 HAL) for reading and sending code for radio frequency (RF) barrier (Came, Nice) simple code 12, 24 bit and HCS301 with keeloq 66 bit.
This lib Gate use for control automation garage door and road barriers. Something similar to the rc-switch project https://github.com/sui77/rc-switch.

Config you RXPORT and PIN with GPIO_EXTI:
 - External Interrupt Mode with Rising/Falling edge trigger detection;
 - no pull up no pull down.
 
and add NVIC global interrupt for you RXPORT.

Copy the library header and source file to the appropriate project directories (Inc, Src).

Config you RXPORT and TXPORT on rf-barrier.h file
```
// ======= Config section =============
#define TXPORT	TX433_GPIO_Port
#define TXPIN	TX433_Pin

#define RXPORT	GPIOC
#define RXPIN  	GPIO_PIN_15
// ====================================
```
In the head file of your project (main.c), include the header file
```
/ * USER CODE BEGIN Includes * /
#include "micros.h"			// https://github.com/leech001/micros
#include "rf-barrier.h"
/ * USER CODE END Includes * /
```
add RXPIN receive interrupt
```
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin){
		case RXPIN:
		{
			RX433_Int();
		}
	}
}
/* USER CODE END 0 */
```
add in main function section for initial initialization of the DWT (Data Watchpoint and Trace unit) (https://github.com/leech001/micros) 
```
/ * USER CODE BEGIN 2 * /
DWT_Init();
/ * USER CODE END 2 * /
```
On this project setup is ready.
After starting the program, information (code and etc.) will be available through the hcs301, came12, came24 structure
```
...
hcs301.SerialNum
came12.SerialNum
...
```
To send binary code ("1001010") use function void RF_send_bin(Protocol_t* protocol, const char* sCodeWord);
```
RF_send_bin(&proto_came12, "111111111111");
```

To send code ("1234") use function void RF_send(Protocol_t* protocol, uint64_t code);
```
RF_send(&proto_came12, 1234);
```

If RF_DUBUG is define on rf-barrier.h file, you can watch on USB serial port radio frequency (RF) diagram
```
Came 12		____________________________________-_--_--_--_--_--_--_--_--_--_--_--_--
HCS301		-_-_-_-_-_-_-_-_-_-_-_-__________--_--_--_--_--_--_--_--_--_--_--_--_--_...
...
```
P.S. Came and Nice code algorithm from http://phreakerclub.com/forum/showthread.php?t=126

## Russian note
Простая библиотека на Си (STM32 HAL) для считывания и отправки кодов  от пультов (ворота, шлагбаумы и др.) как с обычной кодировкой Came, Nice 12 и 24 бит. так и кодировкой HCS301 с keeloq (пример https://ironlogic.ru/il_new.nsf/htm/ru_il100).
Чем то похожа на проект rc-switch https://github.com/sui77/rc-switch

Сконфигурируйте ваш порт и пин куда подключен модуль для приема кодов как порт с внешним прерыванием (GPIO_EXTI):
 - External Interrupt Mode with Rising/Falling edge trigger detection;
 - no pull up no pull down.
 
Включите глобальное прерывание для данного порта (NVIC).

Скопируйте заголовочный и исходный файл библиотеки в соответствующие директории проекта (Inc, Src).

Определение ваши PORT и PIN порты куда подключен считыватель в файле rf-barrier.h file
```
// ======= Config section =============
#define TXPORT	TX433_GPIO_Port
#define TXPIN	TX433_Pin

#define RXPORT	GPIOC
#define RXPIN  	GPIO_PIN_15
// ====================================
```
В головном файл вашего проекта (main.c) подключите заголовочные файлы.
```
/* USER CODE BEGIN Includes */
#include "micros.h"		// https://github.com/leech001/micros
#include "rf-barrier.h"
/* USER CODE END Includes */
```
в секцию добавьте вызов прерывания для вашего пина где висит устройство для приема
```
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin){
		case RXPIN:
		{
			RX433_Int();
		}
	}
}
/* USER CODE END 0 */
```
добавьте в секцию функции int main(void) инициализацию DWT (Data Watchpoint and Trace unit) (https://github.com/leech001/micros)
```
/* USER CODE BEGIN 2 */
DWT_Init();
/* USER CODE END 2 */
```
На этом настройка проекта закончена.
После запуска программы информация о кодах и тд. от пультов поступающих при нажатии кнопок будет доступна через структуры hcs301, came12, came24.
```
...
hcs301.SerialNum
came12.SerialNum
...
```
Для отправки бинарного кода ("1001010") используйте функцию void RF_send_bin(Protocol_t* protocol, const char* sCodeWord);
```
RF_send_bin(&proto_came12, "111111111111");
```

Для отправки обычного кода ("1234") используйте функцию void RF_send(Protocol_t* protocol, uint64_t code);
```
RF_send(&proto_came12, 1234);
```

Если в файле rf-barrier.h определен дебагер (RF_DUBUG) вы сможете наблюдать через USB порт что то похожее на частотную радиограмму как это отправляется в эфир.
Ничего умнее для визуализации сигнала придумать не смог :)
```
Came 12		____________________________________-_--_--_--_--_--_--_--_--_--_--_--_--
HCS301		-_-_-_-_-_-_-_-_-_-_-_-__________--_--_--_--_--_--_--_--_--_--_--_--_--_...
...
```
P.S. Алгоритмы кодировок взяты вот от сюда http://phreakerclub.com/forum/showthread.php?t=126
Отдельное спасибо CodePerfect за подробный разбор.
 