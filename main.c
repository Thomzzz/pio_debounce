#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "debounce.pio.h"

int main() {

    static const uint8_t debouncePin = 25;
    static const uint8_t debugPin = 26;

    // Choose PIO instance (0 or 1)
    PIO pio = pio0;

    // Get first free state machine in PIO 0
    uint sm = pio_claim_unused_sm(pio, true);

    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset = pio_add_program(pio, &debounce_program);

    // Initialize the program using the helper function in our .pio file
    debounce_program_init(pio, sm, offset, 10000.0, debouncePin, debugPin);

    // Start running our PIO program in the state machine
    pio_sm_set_enabled(pio, sm, true);

    // Set debounce (10ms?)
    debounce_program_set_debounce(pio, sm, 50);

    // Do nothing
    while (true) 
    {
        sleep_ms(1000);
    }
}
