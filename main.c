#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "debounce.pio.h"

int main() 
{
    static const uint8_t debugPin = 25;

    // Load the debounce program in both pio's
    uint offset0 = pio_add_program(pio0, &debounce_program);
    uint offset1 = pio_add_program(pio1, &debounce_program);

    // Initialize all 8 statemachines across the 2 pio's
    // (debugPin only works for pio1 for some reason, but that does not matter)
    for (uint8_t i = 0; i < 8; i++)
    {
        PIO pio = (i < 4) ? pio0 : pio1;
        uint8_t sm = i % 4;

        if (i < 4)
            debounce_program_init(pio, sm, offset0, i, debugPin);
        else
            debounce_program_init(pio, sm, offset1, i, debugPin);
          
        pio_sm_set_enabled(pio, sm, true);
        debounce_program_set_debounce(pio, sm, 10.0f); // Set debounce (10ms)
    }

    // Set output pins from 8 to 15
    for (uint8_t i = 0; i < 8; i++)
    {  
        gpio_init(i + 8);
        gpio_set_dir(i + 8, GPIO_OUT);
    }

    while (true) 
    {
        // Output the state of all buttons
        for (uint8_t i = 0; i < 8; i++)
        {
            PIO pio = (i < 4) ? pio0 : pio1;
            uint8_t sm = i % 4;
            gpio_put(i + 8, debounce_program_get_button_pressed(pio, sm));
        } 
    }
}
