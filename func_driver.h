#ifndef ENGINE_FUNC_H_
#define ENGINE_FUNC_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>

#define LEFT_MOST_PLATE			0
#define RIGHT_MOST_PLATE		1
#define LEFT_MOST_COLLIMATOR	2
#define RIGTH_MOST_COLLIMATOR	3

#define ENABLE_FIRST_ENGINE		4
#define ENABLE_SECOND_ENGINE	5

#define FIRST		1
#define SECOND		2

void stop_engine(uint8_t type_engine, uint8_t enable);
uint8_t engine_plate_left_or_right_one_push(uint8_t indication_off, uint8_t enable, uint8_t led);
uint8_t engine_plate_left_or_right(uint8_t counter, uint8_t indication_off, uint8_t enable, uint8_t led);
uint8_t engine_collimator_left_or_right_one_push(uint8_t indication_off, uint8_t enable, uint8_t led);
uint8_t engine_collimator_left_or_right(uint8_t counter, uint8_t indication_off, uint8_t enable, uint8_t led);
void watchdog_reset(void);
void state_indicate(void);

/* not using */
void plate(void);
void collimator(void);

#endif