# trainer

Modules:

- Logic indicator
  - 8x2 leds, tristate
  - hexa display
  - rgb led?
- Buttons
  - switches spdt (4), permit connect to gnd or vcc (with jumpers)
    - maybe use existing? or dip switches
  - debounced pushbuttons (2 or 4)
- Pots
  - 1k and 10k (?)
- Square wave generator
  - 1 555
  - 1 pot for frequency
  - 1 pot for duty cicle
  - led
  - frequency selector for high freqencies (5 hz, 10 hz, 100 hz, 1 khz, 10 khz, 100 khz?) - led indicator
- Sound
  - speaker / piezo (+ pot for volume)
  - buzzer
- Relays
  - 2 relays
  - 2 screwdriver connectors
  - (or driver?)
- USB
  - male/female?
- Other sensors?
  - check Arduino box

- Connector
  - X pins
    - 2 power
    - 8 + 8 + 1 = 17 for leds
    - 4 + 2 = 6 for buttons
    - 2 pots
    - 1 square wave
    - 2 sound
    - 2 usb

