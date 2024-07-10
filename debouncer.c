/*
 * debouncer.c
 *
 *  Created on: Jul 9, 2024
 *      Author: pablo-jean
 */

/*
 * Includes
 */

#include "debouncer.h"

/*
 * Privates
 */

/** static **/

static debouncer_button_t *FirstButton = NULL;

/** list functions **/
debouncer_button_t *_get_next(debouncer_button_t *actual){
	return (debouncer_button_t*)(actual->_intern.nextButton);
}

void _add_button(debouncer_button_t *add){
	debouncer_button_t *button;

	if (FirstButton == NULL){
		FirstButton = add;
	}
	else{
		button = FirstButton;
		while (button->_intern.nextButton != NULL){
			button = _get_next(button);
		}
		button->_intern.nextButton = add;
	}
}

/** functions **/


/*
 *Publics
 */

void debouncer_add_button(debouncer_button_t *Button){
	assert(Button != NULL);
	assert(Button->u8DebouncerFactor > 0);
	assert(Button->FxnReadButton != NULL);

	Button->_intern.bState = !Button->bActiveState;
	if (Button->_intern.bState == true){
		Button->_intern.u8BtnVal = 0;
	}
	else{
		Button->_intern.u8BtnVal = 100;
	}
	Button->_intern.u8DownVal = DEBOUNCER_BUTTONS_HIST_DOWN_VAL;
	Button->_intern.u8UpperVal = DEBOUNCER_BUTTONS_HIST_UPPER_VAL;
	_add_button(Button);
}

size_t debouncer_process(){
	debouncer_button_t *button;
	uint_fast16_t a, b, c;
	uint_fast8_t factor;
	size_t btnId = 0;

	button = FirstButton;
	while(button != NULL){
		factor = button->u8DebouncerFactor;
		a = button->_intern.u8BtnVal;
		b = button->FxnReadButton()*100;

		c = a*(factor-1);
		c += b;
		c /= factor;
		button->_intern.u8BtnVal = c;
		// check if button is active in low level
		if (button->bActiveState == false){
			// check if previous state of button was 'released'
			if (button->_intern.bState == true){
				// if value of the button is lower than the Threshold down value
				// then the button is pressed for sufficient time
				if (button->_intern.u8DownVal > button->_intern.u8BtnVal){
					// update button state to false/'pressed'
					button->_intern.bState = false;
					// call the callback
					debouncer_callback(button->sButtonID);
				}
			}
			else{
				btnId = button->sButtonID;
				// otherwise, now we check for button releasing
				if (button->_intern.u8UpperVal < button->_intern.u8BtnVal){
					button->_intern.bState = true;
				}
			}
		}
		// otherwise, the button is active on High level
		else{
			if (button->_intern.bState == false){
				// if value of the button is higher than the Threshold upper value
				// then the button is pressed for sufficient time
				if (button->_intern.u8UpperVal < button->_intern.u8BtnVal){
					// update button state to true/'pressed'
					button->_intern.bState = true;
					// call the callback
					debouncer_callback(button->sButtonID);
				}
			}
			else{
				btnId = button->sButtonID;
				// otherwise, now we check for button releasing
				if (button->_intern.u8DownVal > button->_intern.u8BtnVal){
					button->_intern.bState = false;
				}
			}
		}

		button = _get_next(button);
	}

	return btnId;
}

/** Callback **/

__attribute__((weak)) void debouncer_callback(size_t sButtonId){
	(void)sButtonId;
}
