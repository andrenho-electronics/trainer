#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int freq = 0;

static void
update_freq(int i)
{
  if (i == 1) {
    ++freq;
    if (freq > 6)
      freq = 0;
  } else if (i == -1) {
    --freq;
    if (freq < 0)
      freq = 6;
  }
  PORTB = (PORTB & (1 << 7)) | ~(1 << freq);
}

int
main()
{
  DDRB  = 0b11111111;    // all inputs
  PORTB = 0b01111111;    // all indicator leds off
  //        |'''''''  leds
  //        '- input (next)

  DDRD  = 0b01000011;
  PORTD = 0b01010000;
  //         ||||||'- input push A
  //         |||||'-- input push B
  //         ||'''--- push outputs
  //         |'------ clock output
  //         '------- input (prev)

  update_freq(0);

  for (;;) {
    // check prev/next buttons
    if (((PINB >> 7) & 1) == 0) {
      update_freq(1);
      _delay_ms(300);
    } else if (((PIND >> 6) & 1) == 0) {
      update_freq(-1);
      _delay_ms(300);
    }
  }
}
