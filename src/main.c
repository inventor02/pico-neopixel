#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "neopixel.pio.h"
#include "config.h"

static PIO pio = pio0;
static uint pio_sm;
static uint offset;

static void neopixel_put(uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio, pio_sm, pixel_grb << 8);
}

static uint32_t rgb_to_grb24(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)g << 16) | ((uint32_t)r << 8) | ((uint32_t)b);
}

void main()
{
  pio_sm = pio_claim_unused_sm(pio, true);
  offset = pio_add_program(pio, &neopixel_program);
  neopixel_program_init(pio, pio_sm, offset, NEOPIXEL_PIN, NEOPIXEL_FREQ_HZ);

  neopixel_put(rgb_to_grb24(UINT8_MAX, 0, 0));
  sleep_ms(3000);
  neopixel_put(rgb_to_grb24(0, UINT8_MAX, 0));
  sleep_ms(3000);
  neopixel_put(rgb_to_grb24(0, 0, UINT8_MAX));
  sleep_ms(3000);

  for (;;)
    tight_loop_contents();
}