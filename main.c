/*
 * PWM.c
 *
 * Created: 26.02.2019 12:15:48
 * Author : chibisovnu
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "func_driver.h"

/* Функция инициализации ТС0 для формирования ШИМ */
inline void timer0_init(void)
{
	TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << COM01);
	/*********************************************
	 *  CS02 = CS01 = CS00 => No clock source    *
	 *  CS02 = 0, CS01 = 0, CS00 = 1 => N = 1    *
	 *  CS02 = 0, CS01 = 1, CS00 = 0 => N = 8    *
	 *  CS02 = 0, CS01 = 1, CS00 = 1 => N = 64   *
	 *  CS02 = 1, CS01 = 0, CS00 = 0 => N = 256  *
	 *  CS02 = 1, CS01 = 0, CS00 = 1 => N = 1024 *
	 *********************************************/
	TCCR0 |=  (1 << CS01);			// Prescaler N = 8
}

/* Функция инициализации ТС0 для формирования ШИМ */
inline void timer2_init(void)
{
	TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM21);
	TCCR2 |= (1 << CS21);
}

/* Функция инициализации внешних прерываний */
inline void init_external_interrupt(void)
{
	GICR |= (1 << INT0);
	GICR &= ~(1 << INT1);
	MCUCR |= (1 << ISC00);
}

inline void init_led(void)
{
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3);
	PORTC &= ~(1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3);
}

inline void init_limit_switch(void)
{
	DDRD &= ~(1 << DDD4) | (1 << DDD5) | (1 << DDD6);
	PORTD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6);
	/*DDRB &= ~(1 << DDB5);
	PORTB |= (1 << DDB5);*/
	DDRA &= ~(1 << DDA2);
	PORTA |= (1 << DDA2);
}

inline void init_buttoms(void)
{
	DDRD &= ~(1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3);
	PORTD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3);
	DDRC &= ~(1 << DDD4);
	PORTC |= (1 << DDC4);
}

inline void init_driver(void)
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRC |= (1 << DDC6);
}

/* Обработчик внешнего прерывания (кнопка СТОП) */
ISR(INT0_vect)
{		
	/*if ((OCR0 != 0) || (OCR2 != 0))
	{
		while(OCR0 || OCR2)
		{
			if ((OCR0 <= 255) && (OCR0 != 0)) 
			{
				OCR0--;
				_delay_ms(25);
			}
			if ((OCR2 <= 255) && (OCR2 != 0)) 
			{
				OCR2--;
				_delay_ms(25);
			}
			
			if ((OCR0 != 0) && (OCR2 == 0)) continue;
			if ((OCR0 == 0) && (OCR2 != 0)) continue;
			
			if ((OCR0 == 0) && (OCR2 == 0)) break;
		}
	}*/
	
	OCR0 = 0x00;
	OCR2 = 0x00;
	
	PORTB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4));
	PORTC &= ~(1 << DDC5) | (1 << DDC6);
}

/* Function: init a PINA for reseting watchdog */
inline void init_watchdog(void)
{
	DDRA |= (1 << DDA1);
}

/************************************************************************/
/*                       MAIN FUNCTION                                  */
/************************************************************************/
	
int main(void)
{	
	/* Инициализация внешних прерываний */
	init_external_interrupt();
	/* Разрешение глобальных прерываний */
	sei();
	
	/* Инициализация кнопок */
	init_buttoms();
	
	/* Инициализация портов для светодиодов */
	init_led();
	
	/* Инициализация пина для сброса МС WatchDog */
	init_watchdog();
	PORTA |= (1 << DDA1);
	PORTA &= ~(1 << DDA1);
	_delay_ms(1);
	PORTA |= (1 << DDA1);
	
	/* Инициализация пина DDD7 и DDB3 на выход для ШИМ */
	DDRD |= (1 << DDD7);
	DDRB |= (1 << DDB3);
	
	/* Инициализация ножки для разрешения работы двигателся */
	DDRB |= (1 << DDB4);
	PORTB &= ~(1 << DDB4);
	
	DDRC |= (1 << DDC5);
	PORTC &= ~(1 << DDC5);
	
	init_driver();
	
	/* Инициализация концевиков */
	init_limit_switch();
	
	/* Инициализация начальной скважности ШИМ */
	OCR0 = 0x00;
	OCR2 = 0x00;
	
	/* Инициализация ТС0 ТС2 для формирования ШИМ*/
	timer0_init();
	timer2_init();
	
	state_indicate();
	
	uint8_t counter_first_engine, counter_first_engine_1;
	uint8_t counter_second_engine, counter_second_engine_1;
	
    while (1) 
	{
		watchdog_reset();
		if (((PIND & (1 << PIND6)) != 0) && ((PIND & (1 << PIND5)) != 0))
		{
			/* Движение плиты вправо */
			counter_first_engine = engine_plate_left_or_right_one_push(RIGHT_MOST_PLATE, ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);
			watchdog_reset();
			
			if (counter_first_engine >= 90)
			{
				counter_first_engine_1 = engine_plate_left_or_right(counter_first_engine, RIGHT_MOST_PLATE,
				ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);
				watchdog_reset();

				if (counter_first_engine_1 >= 90)
				{
					counter_first_engine_1 = 0;
					stop_engine(FIRST, ENABLE_FIRST_ENGINE);
					watchdog_reset();
				}
				
				watchdog_reset();
			}
			else
			{
				counter_first_engine = 0;
				stop_engine(FIRST, ENABLE_FIRST_ENGINE);
				watchdog_reset();
			}
		}
		else
		{
			if ((PIND & (1 << PIND6)) == 0)
			{
				/* Движение плиты вправо */
				counter_first_engine = engine_plate_left_or_right_one_push(RIGHT_MOST_PLATE, ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);
				watchdog_reset();
				
				if (counter_first_engine >= 90)
				{
					counter_first_engine_1 = engine_plate_left_or_right(counter_first_engine, RIGHT_MOST_PLATE,
					ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);
					watchdog_reset();

					if (counter_first_engine_1 >= 90)
					{
						counter_first_engine_1 = 0;
						stop_engine(FIRST, ENABLE_FIRST_ENGINE);
						watchdog_reset();
					}
					
					watchdog_reset();
				}
				else
				{
					counter_first_engine = 0;
					stop_engine(FIRST, ENABLE_FIRST_ENGINE);
					watchdog_reset();
				}
				
				/* Движение плиты влево */
				counter_first_engine = engine_plate_left_or_right_one_push(LEFT_MOST_PLATE, ENABLE_FIRST_ENGINE, RIGHT_MOST_PLATE);
				watchdog_reset();
				
				if (counter_first_engine >= 90)
				{
					counter_first_engine_1 = engine_plate_left_or_right(counter_first_engine, LEFT_MOST_PLATE,
					ENABLE_FIRST_ENGINE, RIGHT_MOST_PLATE);
					
					watchdog_reset();
					
					if (counter_first_engine_1 >= 90)
					{
						counter_first_engine_1 = 0;
						stop_engine(FIRST, ENABLE_FIRST_ENGINE);
						
						watchdog_reset();
					}
					
					watchdog_reset();
				}
				else
				{
					counter_first_engine = 0;
					stop_engine(FIRST, ENABLE_FIRST_ENGINE);
					
					watchdog_reset();
				}
			}
			
			if ((PIND & (1 << PIND5)) == 0) // Разрешение работы коллиматора
			{
				/* Движение коллиматора вправо */
				counter_second_engine = engine_collimator_left_or_right_one_push(RIGTH_MOST_COLLIMATOR, ENABLE_SECOND_ENGINE, LEFT_MOST_COLLIMATOR);
				watchdog_reset();
				
				if (counter_second_engine >= 90)
				{
					counter_second_engine_1 = engine_collimator_left_or_right(counter_second_engine, RIGTH_MOST_COLLIMATOR,
					ENABLE_SECOND_ENGINE, LEFT_MOST_COLLIMATOR);
					
					watchdog_reset();

					if (counter_second_engine_1 >= 90)
					{
						counter_second_engine_1 = 0;
						stop_engine(SECOND, ENABLE_SECOND_ENGINE);
						
						watchdog_reset();
					}
					watchdog_reset();
				}
				else
				{
					counter_second_engine = 0;
					stop_engine(SECOND, ENABLE_SECOND_ENGINE);
					watchdog_reset();
				}
				/* Движение коллиматора влево */
				counter_second_engine = engine_collimator_left_or_right_one_push(LEFT_MOST_COLLIMATOR, ENABLE_SECOND_ENGINE,
																				 RIGTH_MOST_COLLIMATOR);
				watchdog_reset();
				
				if (counter_second_engine >= 90)
				{
					counter_second_engine_1 = engine_collimator_left_or_right(counter_second_engine, LEFT_MOST_COLLIMATOR,
					ENABLE_SECOND_ENGINE, RIGTH_MOST_COLLIMATOR);
					
					watchdog_reset();
					
					if (counter_second_engine_1 >= 90)
					{
						counter_second_engine_1 = 0;
						stop_engine(SECOND, ENABLE_SECOND_ENGINE);
						
						watchdog_reset();
					}
					watchdog_reset();
				}
				else
				{
					counter_second_engine = 0;
					stop_engine(SECOND, ENABLE_SECOND_ENGINE);
					
					watchdog_reset();
				}
			}
		}
		watchdog_reset();
		state_indicate();
    }
}
