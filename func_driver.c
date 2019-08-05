#include "func_driver.h"

uint8_t counter_first_engine, counter_first_engine_1;
uint8_t counter_second_engine, counter_second_engine_1;

void stop_engine(uint8_t type_engine, uint8_t enable)
{
	if (type_engine == FIRST)
	{		
		OCR0 = 0;

		PORTB &= ~((1 << DDB0) | (1 << DDB1));
		PORTB &= ~(1 << enable);
	}
	else if (type_engine == SECOND)
	{		
		OCR2 = 0;
		
		PORTB &= ~(1 << DDB2);
		PORTC &= ~(1 << DDC6);
		PORTC &= ~(1 << enable);
	}
}

/* Reset a Watch Dog */
void watchdog_reset(void)
{
	PORTA &= ~(1 << DDA1);
	_delay_ms(1);
	PORTA |= (1 << DDA1);
}

uint8_t engine_plate_left_or_right_one_push(uint8_t indication_off, uint8_t enable, uint8_t led)
{
	uint8_t counter = 0; //!!!

	if ((indication_off == RIGHT_MOST_PLATE) && (enable == ENABLE_FIRST_ENGINE) && (led == LEFT_MOST_PLATE))
	{
		/* Move to right plate's side */
		if (((PIND & (1 << PIND0)) == 0) && ((PIND & (1 << PIND5))))
		{
			/* Switch off the most left condition of the plate */
			//if ((PIND & (1 << PIND4)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the first engine */
			PORTB |= (1 << enable);

			/* Moving while the bottom is pushed */
			while ((PIND & (1 << PIND0)) == 0)
			{
				watchdog_reset();
				state_indicate();
				if (OCR0 == 255) continue;
				
				if ((PIND & (1 << PIND5)) == 0)
				{
					OCR0 = 0;
					break;
				}
				
				counter++;

				if (counter <= 100)
				{
					/* Switch on first engine (clockwise)*/
					PORTB |= (1 << DDB0);
					PORTB &= ~(1 << DDB1);

					OCR0++;
					_delay_ms(25);
					watchdog_reset();
				}
				else break;

				if (OCR0 == 0) break;
			}
			
			/* Switch on indication of the most left side plate */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}
	else if ((indication_off == LEFT_MOST_PLATE) && (enable == ENABLE_FIRST_ENGINE) && (led == RIGHT_MOST_PLATE))
	{
		/* Move to left plate's side */
		if (((PIND & (1 << PIND1)) == 0) && ((PIND & (1 << PIND4))))
		{
			/* Switch off the most right condition of the plate */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the first engine */
			PORTB |= (1 << enable);

			/* Moving while the bottom is pushed */
			while (((PIND & (1 << PIND1)) == 0))
			{
				watchdog_reset();
				state_indicate();
				if (OCR0 == 255) continue;
				
				if ((PIND & (1 << PIND4)) == 0)
				{
					OCR0 = 0;
					break;
				}
				
				counter++;

				if (counter <= 100)
				{
					/* Switch on first engine (counter clock-wise)*/
					PORTB &= ~(1 << DDB0);
					PORTB |= (1 << DDB1);

					OCR0++;
					_delay_ms(25);
					watchdog_reset();
				}
				else break;

				if (OCR0 == 0) break;
			}
			
			/* Switch on indication of the most right side plate */
			//if ((PIND & (1 << PIND4)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}

	return counter;
}

uint8_t engine_plate_left_or_right(uint8_t counter, uint8_t indication_off, uint8_t enable, uint8_t led)
{
	if ((counter >= 90) && (indication_off == RIGHT_MOST_PLATE) && (enable == ENABLE_FIRST_ENGINE) && (led == LEFT_MOST_PLATE))
	{
		/* Move to right plate's side */
		if (((PIND & (1 << PIND0)) == 0) && ((PIND & (1 << PIND5))))
		{
			/* Switch off the most left condition of the plate */
			//if ((PIND & (1 << PIND4)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the first engine */
			PORTB |= (1 << enable);

			/* Move to the left side */
			while ((PIND & (1 << PIND5)))
			{
				watchdog_reset();
				state_indicate();
				if (OCR0 == 0) break;
				if (OCR0 == 255) continue;

				counter++;
				
				if ((PIND & (1 << PIND5)) == 0)
				{
					OCR0 = 0;
					break;
				}

				/* Switch on first engine (clockwise)*/
				PORTB |= (1 << DDB0);
				PORTB &= ~(1 << DDB1);

				OCR0++;
				_delay_ms(25);
				watchdog_reset();
			}
			
			/* Switch on indication of the most left side plate */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}
	else if ((counter >= 90) && (indication_off == LEFT_MOST_PLATE) && (enable == ENABLE_FIRST_ENGINE) && (led == RIGHT_MOST_PLATE))
	{
		/* Move to left plate's side */
		if (((PIND & (1 << PIND1)) == 0) && ((PIND & (1 << PIND4))))
		{
			/* Switch off the most right condition of the plate */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the first engine */
			PORTB |= (1 << enable);

			/* Move to the right side */
			while ((PIND & (1 << PIND4)))
			{
				watchdog_reset();
				state_indicate();
				if (OCR0 == 0) break;
				if (OCR0 == 255) continue;

				counter++;
				
				if ((PIND & (1 << PIND4)) == 0)
				{
					OCR0 = 0;
					break;
				}

				/* Switch on first engine (counter clock-wise)*/
				PORTB &= ~(1 << DDB0);
				PORTB |= (1 << DDB1);

				OCR0++;
				_delay_ms(25);
				watchdog_reset();
			}
			
			/* Switch on indication of the most right side plate */
			//if ((PIND & (1 << PIND4)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}

	return counter;
}

uint8_t engine_collimator_left_or_right_one_push(uint8_t indication_off, uint8_t enable, uint8_t led)
{
	uint8_t counter = 0; //!!!

	if ((indication_off == RIGTH_MOST_COLLIMATOR) && (enable == ENABLE_SECOND_ENGINE) && (led == LEFT_MOST_COLLIMATOR))
	{
		/* Move to left collimator's side */
		if (((PIND & (1 << PIND3)) == 0) && ((PINA & (1 << PINA2))))
		{
			/* Switch off the most right condition of the collimator */
			//if ((PIND & (1 << PIND6)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the second engine */
			PORTC |= (1 << enable);

			/* Move to the left side */
			while (((PIND & (1 << PIND3)) == 0))
			{
				watchdog_reset();
				state_indicate();
				if (OCR2 == 255) continue;

				counter++;
				
				if ((PINA & (1 << PINA2)) == 0)
				{
					OCR2= 0;
					break;
				}

				if (counter <= 100)
				{
					/* Switch on first engine (clockwise)*/
					PORTC |= (1 << DDC6);
					PORTB &= ~(1 << DDB2);

					OCR2++;
					_delay_ms(25);
					watchdog_reset();
				}
				else break;

				if (OCR2 == 0) break;
			}
			
			/* Switch on indication of the most left side collimator */
			//if ((PINA & (1 << PINA2)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}
	else if ((indication_off == LEFT_MOST_COLLIMATOR) && (enable == ENABLE_SECOND_ENGINE) && (led == RIGTH_MOST_COLLIMATOR))
	{
		/* Move to right collimator's side */
		if (((PINC & (1 << PINC4)) == 0) && ((PIND & (1 << PIND6))))
		{
			/* Switch off the most left condition of the collimator */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the second engine */
			PORTC |= (1 << enable);

			/* Move to the right side */
			while ((PINC & (1 << PINC4)) == 0)
			{
				watchdog_reset();
				state_indicate();
				if (OCR2 == 255) continue;

				counter++;
				
				if ((PIND & (1 << PIND6)) == 0)
				{
					OCR2 = 0;
					break;
				}

				if (counter <= 100)
				{
					/* Switch on first engine (counter clock-wise) */
					PORTC &= ~(1 << DDC6);
					PORTB |= (1 << DDB2);

					OCR2++;
					_delay_ms(25);
					watchdog_reset();
				}
				else break;

				if (OCR2 == 0) break;
			}
			
			/* Switch on indication of the most right side collimator */
			//if ((PIND & (1 << PIND6)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}

	return counter;
}

uint8_t engine_collimator_left_or_right(uint8_t counter, uint8_t indication_off, uint8_t enable, uint8_t led)
{
	if ((counter >= 90) && (indication_off == RIGTH_MOST_COLLIMATOR) && (enable == ENABLE_SECOND_ENGINE) && (led == LEFT_MOST_COLLIMATOR))
	{
		/* Move to left collimator's side */
		if (((PIND & (1 << PIND3)) == 0) && ((PINA & (1 << PINA2))))
		{
			/* Switch off the most right condition of the collimator */
			//if ((PIND & (1 << PIND6)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the second engine */
			PORTC |= (1 << enable);

			/* Move to the left side */
			while ((PINA & (1 << PINA2)))
			{
				watchdog_reset();
				state_indicate();
				if (OCR2 == 0) break;
				if (OCR2 == 255) continue;

				counter++;

				/* Switch on second engine (clockwise) */
				PORTC |= (1 << DDC6);
				PORTB &= ~(1 << DDB2);

				OCR2++;
				_delay_ms(25);
				watchdog_reset();
				
				if ((PINA & (1 << PINA2)) == 0)
				{
					OCR2 = 0;
					break;
				}
			}
			
			/* Switch on indication of the most left side collimator */
			//if ((PINA & (1 << PINA2)) == 0) PORTC |= (1 << led);
			//else PORTC &= ~(1 << led);
		}
	}
	else if ((counter >= 90) && (indication_off == LEFT_MOST_COLLIMATOR) && (enable == ENABLE_SECOND_ENGINE) && (led == RIGTH_MOST_COLLIMATOR))
	{
		/* Move to left collimator's side */
		if (((PINC & (1 << PINC4)) == 0) && ((PIND & (1 << PIND6))))
		{
			/* Switch off the most left condition of the collimator */
			//if ((PIND & (1 << PIND5)) == 0) PORTC |= (1 << indication_off);
			//else PORTC &= ~(1 << indication_off);

			/* Enable the second engine */
			PORTC |= (1 << enable);

			/* Move to the right side */
			while ((PIND & (1 << PIND6)))
			{
				watchdog_reset();
				state_indicate();
				if (OCR2 == 0) break;
				if (OCR2 == 255) continue;

				counter++;

				/* Switch on first engine (counter clock-wise) */
				PORTC &= ~(1 << DDC6);
				PORTB |= (1 << DDB2);

				OCR2++;
				_delay_ms(25);
				watchdog_reset();
				
				if ((PIND & (1 << PIND6)) == 0)
				{
					OCR2 = 0;
					break;
				}
				
				/* Switch on indication of the most right side collimator */
				//if ((PIND & (1 << PIND6)) == 0) PORTC |= (1 << led);
				//else PORTC &= ~(1 << led);
			}
		}
	}

	return counter;
}

void plate(void)
{
	/* Движение плиты вправо */
	counter_first_engine = engine_plate_left_or_right_one_push(RIGHT_MOST_PLATE, ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);

	if (counter_first_engine >= 90)
	{
		counter_first_engine_1 = engine_plate_left_or_right(counter_first_engine, RIGHT_MOST_PLATE,
		ENABLE_FIRST_ENGINE, LEFT_MOST_PLATE);

		if (counter_first_engine_1 >= 90)
		{
			counter_first_engine_1 = 0;
			stop_engine(FIRST, ENABLE_FIRST_ENGINE);
		}
	}
	else
	{
		counter_first_engine = 0;
		stop_engine(FIRST, ENABLE_FIRST_ENGINE);
	}
	/* Движение плиты влево */
	counter_first_engine = engine_plate_left_or_right_one_push(LEFT_MOST_PLATE, ENABLE_FIRST_ENGINE, RIGHT_MOST_PLATE);
	
	if (counter_first_engine >= 90)
	{
		counter_first_engine_1 = engine_plate_left_or_right(counter_first_engine, LEFT_MOST_PLATE,
		ENABLE_FIRST_ENGINE, RIGHT_MOST_PLATE);
		
		if (counter_first_engine_1 >= 90)
		{
			counter_first_engine_1 = 0;
			stop_engine(FIRST, ENABLE_FIRST_ENGINE);
		}
	}
	else
	{
		counter_first_engine = 0;
		stop_engine(FIRST, ENABLE_FIRST_ENGINE);
	}
}

void collimator(void)
{
	/* Движение коллиматора вправо */
	counter_second_engine = engine_collimator_left_or_right_one_push(RIGTH_MOST_COLLIMATOR, ENABLE_SECOND_ENGINE, LEFT_MOST_COLLIMATOR);

	if (counter_second_engine >= 90)
	{
		counter_second_engine_1 = engine_collimator_left_or_right(counter_second_engine, RIGTH_MOST_COLLIMATOR,
		ENABLE_SECOND_ENGINE, LEFT_MOST_COLLIMATOR);

		if (counter_second_engine_1 >= 90)
		{
			counter_second_engine_1 = 0;
			stop_engine(SECOND, ENABLE_SECOND_ENGINE);
		}
	}
	else
	{
		counter_second_engine = 0;
		stop_engine(SECOND, ENABLE_SECOND_ENGINE);
	}
	/* Движение коллиматора влево */
	counter_second_engine = engine_collimator_left_or_right_one_push(LEFT_MOST_COLLIMATOR, ENABLE_SECOND_ENGINE,
	RIGTH_MOST_COLLIMATOR);
	
	if (counter_second_engine >= 90)
	{
		counter_second_engine_1 = engine_collimator_left_or_right(counter_second_engine, LEFT_MOST_COLLIMATOR,
		ENABLE_SECOND_ENGINE, RIGTH_MOST_COLLIMATOR);
		
		if (counter_second_engine_1 >= 90)
		{
			counter_second_engine_1 = 0;
			stop_engine(SECOND, ENABLE_SECOND_ENGINE);
		}
	}
	else
	{
		counter_second_engine = 0;
		stop_engine(SECOND, ENABLE_SECOND_ENGINE);
	}
}

void state_indicate(void)
{
	/* Indication of plate */
	if ((PIND & (1 << PIND4)) == 0)
	{
		PORTC |= (1 << DDC1);
	}
	else
	{
		PORTC &= ~(1 << DDC1);
	}
	
	if ((PIND & (1 << PIND5)) == 0)
	{
		PORTC |= (1 << DDC0);
	}
	else
	{
		PORTC &= ~(1 << DDC0);
	}
	
	/* Indication of collimator */
	if ((PINA & (1 << PINA2)) == 0)
	{
		PORTC |= (1 << DDC2);
	}
	else
	{
		PORTC &= ~(1 << DDC2);
	}
	
	if ((PIND & (1 << PIND6)) == 0)
	{
		PORTC |= (1 << DDC3);
	}
	else
	{
		PORTC &= ~(1 << DDC3);
	}
}