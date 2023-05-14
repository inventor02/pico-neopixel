#include "neopixel.pio.h"

#include <stdint.h>

#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "neopixel.pio.h"

void neopixel_program_init(PIO pio, uint sm, uint offset, uint pin, float freq)
{
  pio_gpio_init(pio, pin);
  pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

  pio_sm_config config = neopixel_program_get_default_config(offset);
  sm_config_set_sideset_pins(&config, pin);
  sm_config_set_out_shift(&config, false, true, 24);

  uint8_t cycles_per_bit = 4 * neopixel_T1;
  float clkdiv = clock_get_hz(clk_sys) / (freq * cycles_per_bit);
  sm_config_set_clkdiv(&config, clkdiv);

  pio_sm_init(pio, sm, offset, &config);
  pio_sm_set_enabled(pio, sm, true);
}