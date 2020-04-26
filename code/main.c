#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int freq = 0;

static void
update_freq(int i)
{
  /*
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
    case 4:  // 100 Hz
      OCR1A = 4999;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
    case 5:  // 1 kHz
      OCR1A = 499;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
    case 6:  // 100 kHz
      OCR1A = 4;
      TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
      break;
  }
  // enable timer compare interrupt
  TIMSK |= (1 << OCIE1A);
  sei(); // allow interrupts
  */
  
  cli();
  TCCR1A = (1 << COM1A1) | (1 << WGM10);
  TCCR1B = (1 << CS01);
  OCR1A = 0x10;
  sei();
}

ISR(TIMER1_COMPA_vect) {
  if (PORTD & (1 << 5))
    PORTD &= ~(1 << 5);
  else
    PORTD |= (1 << 5);
}

int
main()
{
  DDRB  = 0b11111111;    // all inputs
  PORTB = 0b01110111;    // all indicator leds off
  //        |'''''''  leds
  //        '- input (next)

  DDRD  = 0b01000011;
  PORTD = 0b01110000;
  //         ||||||'- input push A
  //         |||||'-- input push B
  //         ||'''--- push outputs
  //         |'------ clock output
  //         '------- input (prev)

  update_freq(0);

  for (;;) {
    /*
    // check prev/next buttons
    if (((PINB >> 7) & 1) == 0) {
      update_freq(1);
      _delay_ms(300);
    } else if (((PIND >> 6) & 1) == 0) {
      update_freq(-1);
      _delay_ms(300);
    }
    */
  }
}
