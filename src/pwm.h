#include <avr/io.h>

/* clang-format off */
/*
-------------------------------------------------------------------------------------------------------------
GTCCR – General Timer/Counter Control Register
BIT         0       |   1       |   2       |   3       |   4       |   5       |   6       |   7
            TSM     |   PWM1B   |   COM1B1  |   COM1B0  |   FOC1B   |   FOC1A   |   PSR1    |   PSR0
Read/Write  R/W     |   R       |   R       |   R       |   R       |   R       |   R       |   R/W
Initial     0       |   0       |   0       |   0       |   0       |   0       |   0       |   0
-------------------------------------------------------------------------------------------------------------
TCCR0A – Timer/Counter Control Register A
BIT         0       |   1       |   2       |   3       |   4       |   5       |   6       |   7
            COM0A1  |   COM0A0  |   COM0B1  |   COM0B0  |   -       |   -       |   WGM01   |   WGM00
Read/Write  R/W     |   R/W     |   R/W     |   R/W     |   R       |   R       |   R/W     |   R/W
Initial     0       |   0       |   0       |   0       |   0       |   0       |   0       |   0
-------------------------------------------------------------------------------------------------------------
TCCR0B – Timer/Counter Control Register B
BIT         0       |   1       |   2       |   3       |   4       |   5       |   6       |   7
            FOC0A   |   FOC0B   |   -       |   -       |   WMG02   |   CS02    |   CS01    |   CS00
Read/Write  R       |   R       |   R       |   R       |   R/W     |   R/W     |   R/W     |   R/W
Initial     0       |   0       |   0       |   0       |   0       |   0       |   0       |   0
-------------------------------------------------------------------------------------------------------------
*/
/* clang-format on */

// sets the pwm duty cicle (in percent) - needs the top value from setupPwm!
inline void setDutyCircle(uint8_t timerTopValue, uint8_t percent)
{
   if (percent == 0)
      OCR0B = 0b0;
   else if (percent >= 100)
      OCR0B = 0b11111111;
   else
      OCR0B = uint8_t((percent / (float)100) * timerTopValue);
}

// setup pwm with phase correct pwm mode (5) on Port B1
// max pwm freq = cpu freq / (prescale * 1 * 2), min pwm freq = cpu freq / (prescale * 255 * 2)
typedef unsigned int Herz;
inline uint8_t setupPwm(Herz freq)
{
   // -> uses Timer/Counter0: OCR0A
   // -> assumes ATTINY85 fused to use 8MHz internal clock (no prescale)
   TCCR0A = 0b0 | ((1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00));
   TCCR0B = 0b0 | ((0 << FOC0A) | (0 << FOC0B) | (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00));

   // MCUCR = MCUCR | (1 << PUD); // MCU Control Register, PUD: Disable all pull-ups
   DDRB = 0b0 | (1 << PORTB1); // Data Direction Register Port B: 1 for output, 0 for input (datasheet 10.2.1, P. 55, Table 10-1)
   PORTB = PORTB & 0b11111101; // Port B Data Register, Bit 1 = PORTB1 = 0

   // freq / prescale / TimerTop / 2 = pwm freq => TimerTop = freq/(2*pwm freq)
   uint8_t timerTopValue = ((8000000 / 2) / freq);
   if ((8000000 / 2) % freq > (freq % 2 == 0 ? freq / 2 : ((freq + 1) / 2)))
      timerTopValue += 1;

   // 160 = 25kHz, 23kHz = ~174
   OCR0A = timerTopValue;

   // duty cycle = OCR0B/OCR0A: 87/174=0.5
   setDutyCircle(timerTopValue, 50);
   return timerTopValue;
}