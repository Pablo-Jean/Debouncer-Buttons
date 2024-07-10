/*
 * debouncer.h
 *
 *  Created on: Jul 9, 2024
 *      Author: pablo-jean
 */

#ifndef DEBOUNCER_BUTTONS_DEBOUNCER_H_
#define DEBOUNCER_BUTTONS_DEBOUNCER_H_

/**
 * Includes
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/**
 * Macros
 */

#ifndef DEBOUNCER_BUTTONS_HIGH_VAL
#define DEBOUNCER_BUTTONS_HIGH_VAL				100
#endif

#ifndef DEBOUNCER_BUTTONS_HIST_UPPER_VAL
#define DEBOUNCER_BUTTONS_HIST_UPPER_VAL		60
#endif

#ifndef DEBOUNCER_BUTTONS_HIST_DOWN_VAL
#define DEBOUNCER_BUTTONS_HIST_DOWN_VAL			40
#endif

/**
 * Enumerates
 */


/**
 * Structs and Typedefs
 */

typedef uint8_t(*fxn_button_read_t)(void);

typedef struct{
	fxn_button_read_t 	FxnReadButton;
	bool 				bActiveState;
	uint_fast8_t 		u8DebouncerFactor;
	size_t 				sButtonID;
	struct{
		uint_fast8_t 	u8BtnVal;
		bool 			bState;
		uint_fast8_t 	u8UpperVal;
		uint_fast8_t 	u8DownVal;
		void*			nextButton;
	}_intern;
}debouncer_button_t;


/**
 * Publics
 */

void debouncer_add_button(debouncer_button_t *Button);

size_t debouncer_process();

/* Callback */

void debouncer_callback(size_t sButtonId);

#endif /* DEBOUNCER_BUTTONS_DEBOUNCER_H_ */
