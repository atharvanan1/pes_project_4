/**
  * File Name 		- led_control.h
  * Description 	- header file for led_control.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */


#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "logger.h"

// Macros
typedef enum {
	Red,
	Green,
	Blue,
} led_color_t;

// Prototype Functions
void Turn_On_Only_LED(led_color_t LED);
#endif /* LED_CONTROL_H_ */
