#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int freq = 7;
volatile bool clk = false;

static void
update_freq(int i)
{
  if (i == 1) {
    ++freq;
    if (freq > 7)
      freq = 0;
  } else if (i == -1) {
    --freq;
    if (freq < 0)
      freq = 7;
  }


  if (freq == 7) {
    PORTB = (PORTB & (1 << 7)) | 0b1111111;
    PORTD &= ~(1 << 5);   // clear output
  } else {
    PORTB = (PORTB & (1 << 7)) | ~(1 << freq);
    PORTD |= (1 << 5);    // set output
    clk = true;
  }

  // TIMER 1 for interrupt frequency 2 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  switch (freq) {
    case 0:  // 0.5 Hz
      OCR1A = 15624;
      TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
      break;
    case 1:  // 1 Hz
      OCR1A = 62499;
      TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
      break;
    case 2:  // 2 Hz
      OCR1A = 31249;
      TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
      break;
    case 3:  // 5 Hz
      OCR1A = 12499;
      TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
      break;
    case 4:  // 10 Hz
      OCR1A = 49999;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
    case 5:  // 100 Hz
      OCR1A = 4999;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
    case 6:  // 1 kHz
      OCR1A = 499;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
    case 7:
      OCR1A = 0;
      TCCR1B = 0;
      break;
  }
  // enable timer compare interrupt
  TIMSK |= (1 << OCIE1A);
  sei(); // allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (clk)
    PORTD &= ~(1 << 5);
  else
    PORTD |= (1 << 5);
	clk = !clk;
}

int
main()
{
  DDRB  = 0b01111111;    // all inputs
  PORTB = 0b11111111;    // all indicator leds off
  //        |'''''''  leds
  //        '- input (next)

  DDRD  = 0b00000011;
  PORTD = 0b01110000;
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
      _delay_ms(50);
      while (((PINB >> 7) & 1) == 0);
    } else if (((PIND >> 6) & 1) == 0) {
      update_freq(-1);
      _delay_ms(50);
      while (((PIND >> 6) & 1) == 0);
    }
  }
}
