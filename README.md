# Software Debouncer Library for Embedded Systems

## 📝 1. Description

This library provides a software-based debouncing solution for embedded systems, using a moving average filtering approach. It can be used with any microcontroller to effectively eliminate unwanted signal noise and bouncing effects from input switches, buttons, or other digital inputs.

The library can handle how much buttons you need, all you need to do is just register then with the provided API. Our library uses a logic of Linked List.

## 🔧 2. Features

*   🌊 Moving average filtering for robust debouncing   
*   🔌 Compatible with any microcontroller  
*   📚 Easy-to-use API for integration into your project   
*   🕰️ Configurable debounce time and filter window size  
*   🧹 Removes unwanted signal noise and bouncing effects
    
## 🚀 3. Getting Started

1.  📥 Clone the repository or download the library files.
2.  🔌 Include the library in your embedded project.
3.  🛠️ Configure the debounce time and filter window size according to your needs.
4.  🔧 Integrate the debouncer into your input handling code.

## 📚 4. API 

### 4.1 Instancing Struct

**debouncer_button_t**: This instance your button and add to the list to run the debouncer routine. The fields into this struct are:

```C
debouncer_button_t{
    // Routine to read the button based on your microcontroller. Prototype are uint8_t fxn(void);
    fxn_button_read_t 	FxnReadButton;

    // The state that your button will give if is pressed. Ex: Button with Pull-Up is active in
    // low-level, so, you provide 'false' value
	bool 				bActiveState;

    // This is the Moving Average Factor, the bigger, the more intense is the filtering, giving
    // a slower response, but more strong against noise.
    // This value must be greater than 0, value 1 will turn-off debouncer.
	uint_fast8_t 		u8DebouncerFactor;
    
    // A value ID to identify your button, useful to know what button is pressed when you
    // register more than 1.
	size_t 				sButtonID;
}
```

### 4.2 Function APIs

### 4.2.1 `void debouncer_add_button(debouncer_button_t *Button);`

This Function is used to add a button to the List and initialize with first values, by providing the pointer of instance of your button.

### 4.2.2 `size_t debouncer_process();`

This routine will run the Moving Average algorithm, and do all checks if button is pressed or not. If is pressed, the callback will generate and the value `sButtonID` is returned.

**Note 1**: This routine is safe to call before any `debouncer_add_button`.

### 4.2.3 `void debouncer_callback(size_t sButtonId);`

When a 'software edge' coming from 'released' to 'pressed' is detected, this callback routine is called, and the parameter `sButtonId` contains the identifier of the button.

**Note 1**: Call this routine periodically, can be called from a hardware timer, a software timer, or even a Task.

**Note 2**: Configure a good peridiocally value, like 1ms, 10ms.

**Note 3**: Threat this routine as an ISR, in other words, do not perform heavy algorithms and routines.

## 📚 5. Usage

See below an example on STM32, where `BTN_0` is a button connected with a Pull-Up resistor, and `BTN_1` with a Pull-Down. This buttons is instanced on Btn0 and Btn1 variables.

When `BTN_0` or `BTN_1` is pressed, a LED is toggled.

```C
#include "debouncer.h"

/** Read Gpio Functions **/
uint8_t read_btn0(){
	if (HAL_GPIO_ReadPin(BTN_0_GPIO_Port, BTN_0_Pin) == GPIO_PIN_SET){
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t read_btn1(){
	if (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_SET){
		return 1;
	}
	else{
		return 0;
	}
}

/** Timer Callback for debouncer process **/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	debouncer_process();
}

/** Debouncer Callback **/
void debouncer_callback(size_t sButtonId){
	if (sButtonId == BTN_0_Pin){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
	else if (sButtonId == BTN_1_Pin){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
}

void main(){

    /* Hal and Peripheral Initialization */

    /* Debouncer Button Initialization and Register */
    Btn0.FxnReadButton = read_btn0;
    Btn0.bActiveState = false;
    Btn0.sButtonID = BTN_0_Pin;
    Btn0.u8DebouncerFactor = 10;

    Btn1.FxnReadButton = read_btn1;
    Btn1.bActiveState = true;
    Btn1.sButtonID = BTN_1_Pin;
    Btn1.u8DebouncerFactor = 5;

    debouncer_add_button(&Btn0);
    debouncer_add_button(&Btn1);

    HAL_TIM_Base_Start_IT(&htim6);

    while (1){

    }
}

```


## 6. 📄 License

This project is licensed under the [MIT License](https://github.com/Pablo-Jean/Debouncer-Buttons/blob/main/LICENSE).