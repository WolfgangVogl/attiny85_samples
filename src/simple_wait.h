#ifndef _NOP
#define _NOP()                                                                                                                        \
   do                                                                                                                                 \
   {                                                                                                                                  \
      __asm__ __volatile__("nop");                                                                                                    \
   } while (0)
#endif

// _SimpleWaitMs(ms): just waits approx <ms> ms
#ifndef _SimpleWaitMs
#define _SimpleWaitMs(ms)                                                                                                             \
   {                                                                                                                                  \
      const uint32_t _ticksPerMs = 8000000 / 6980;                                                                                    \
      for (uint8_t _msWait = 0; _msWait < ms; _msWait++)                                                                              \
         for (uint32_t ticks = 0; ticks < _ticksPerMs; ticks++)                                                                       \
            __asm__ __volatile__("nop");                                                                                              \
   };
#endif