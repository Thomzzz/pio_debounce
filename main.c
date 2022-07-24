#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "debounce.pio.h"

int main() {

    static const uint debouncePin = 6;
    static const uint debugPin = 7;
    static const uint ledPin = 8;

    // Configure PIO
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &debounce_program);
    debounce_program_init(pio, sm, offset, debouncePin, debugPin);
    pio_sm_set_enabled(pio, sm, true);
    debounce_program_set_debounce(pio, sm, 10.0f); // Set debounce (10ms)

    // Set led pin
    gpio_init(ledPin);
    gpio_set_dir(ledPin, GPIO_OUT);

    while (true) 
    {
        bool buttonPressed = debounce_program_get_button_pressed(pio, sm);
        gpio_put(ledPin, buttonPressed);
        //sleep_ms(1000);
    }
}
