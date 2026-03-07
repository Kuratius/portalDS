#ifndef __DEBUG9__
#define __DEBUG9__

#define VERSIONMAGIC 45464873

#define     PROF_START()                \
do {                                \
    TIMER2_DATA = 0; TIMER3_DATA = 0;   \
    TIMER3_CR = TIMER_ENABLE | TIMER_CASCADE | TIMER_IRQ_REQ; \
    TIMER2_CR = TIMER_ENABLE;    \
} while(0)

#define PROF_GET(_time) _time = ( TIMER3_DATA << 16 ) | TIMER2_DATA;

#define     PROF_END(_time)             \
do {                                \
    _time = ( TIMER3_DATA << 16 ) | TIMER2_DATA;  \
    TIMER2_CR = 0; TIMER3_CR = 0;    \
} while(0)


#define PROF2_START()
#define PROF2_END(_time) _time=92431
#include <stddef.h>
int snprintf_arm7(char * s, size_t n , const char * fmt, ...);
#define NOGBA(_fmt, _args...) do { char nogba_buffer[128]; snprintf_arm7(nogba_buffer, sizeof(nogba_buffer), _fmt, ##_args); nocashMessage(nogba_buffer); } while(0)

void DS_Debug(char* string, ...);
void DS_DebugPause(void);
size_t DS_UsedMem(void);
size_t DS_FreeMem(void);

#endif
