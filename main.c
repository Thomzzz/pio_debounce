#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "debounce.pio.h"

int main() {

    static const uint debouncePin = 6;
    static const uint debugPin = 7;

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &debounce_program);

    debounce_program_init(pio, sm, offset, debouncePin, debugPin);
    pio_sm_set_enabled(pio, sm, true);

    // Set debounce (10ms)
    debounce_program_set_debounce(pio, sm, 10.0f);

    // Do nothing
    while (true) 
    {
        sleep_ms(1000);
    }
}
