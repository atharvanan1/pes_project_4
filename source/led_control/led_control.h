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
#define RED 	0
#define BLUE  	1
#define GREEN  	2
void Turn_On_Only_LED(uint8_t LED_Macro);
#endif /* LED_CONTROL_H_ */
