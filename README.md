# A simple solution for button debouncing

With the release of the RP2040 comes a new peripheral called PIO. This Programmable I/O can handle in and output operations in the background, while the processor is busy doing other stuff. PIO is usually used for communication handling, but I thought it could also serve as a nice button debounce peripheral.

My usecase for this debounce program was a keypad used for rhythm games. It should be no surprise that the main priority was response time. Every debounce implementation, be it software or hardware, has a delay from button press and/or release to the actual detection of a press or release. In this implementation the delay is applied when the button is released.

## Features
* Designed for circuits where a button is connected from an input pin of the RP2040 to ground.
* Built-in pull-up resistor is enabled by the debounce program.
* Can be used on all 8 PIO state machines, so up to 8 buttons.
* Uses 15 out of the 32 instructions of the PIO instruction memory.
* User can set a debounce time in milliseconds at runtime.
* Debounce time is updated after every button release.
* User can get the state of a button using a polling method.
* The state machines do not set interrupts (yet?)

## Using PIO debounce
```c
// Include these headers
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "debounce.pio.h"

int main() 
{
  static const uint debouncePin = 6;

  // Decide which pio (0 or 1) and statemachine (0 to 3) to use.
  PIO pio = pio0;
  uint sm = 0;

  // Load the debounce program in the instrucntion memory of pio 0
  // If both pio's are used then this method must be called for each pio
  // and the offsets of each program must be kept for the next step.
  uint offset = pio_add_program(pio, &debounce_program);

  // Initialize one state machine with the pin to be used as input.
  // GPIO and pull-up initialization are done by the pio program.
  debounce_program_init(pio, sm, offset, debouncePin);

  // Start a state machine.
  pio_sm_set_enabled(pio, sm, true);

  // Set debounce time of a state machine to 10ms.
  debounce_program_set_debounce(pio, sm, 10.0f);

  while (true) 
  {
    // Get the state of a button.
    bool buttonPressed = debounce_program_get_button_pressed(pio, sm);
    
    // Do whatever you want with the button input ...
  }
}
```
## State diagram
The state diagram describes the behaviour of the PIO state machine.

![State diagram](https://user-images.githubusercontent.com/33899330/181623192-9c733956-471b-493d-9ed6-40da6a4b53b3.svg)