
#define SAMPLE_PWM

#ifdef SAMPLE_PWM
#include "pwm.h"
#include "simple_wait.h"

int main()
{
   auto timerTop = setupPwm(23500);

   uint8_t hzCount = 0;

   while (true)
   {
      _SimpleWaitMs(3);

      hzCount = (hzCount + 1) % 101;

      if (hzCount % 2 == 0)
         setDutyCircle(timerTop, 10);
      else
         setDutyCircle(timerTop, 90);
   }
}
#endif
