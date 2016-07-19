#include <stdint.h>

#define prescaleTimer0 (1e-3*(F_CPU/128))-1
//#define prescaleTimer0 160

typedef unsigned char pwmMode;
#define PERCENT 0
#define POWTWO  1

#define NOPRESCALE 1
#define STDPRESCALE 5

//FLAGS for TIFR timer 0
#define CLEARTOPINTERRUPT  0x1
#define CLEAROVFINTERRUPT  0x2


//Flags for COMnx f/ fast PWM
#define DISCONNECTED    0
#define NONINVERTING    2
#define INVERTING       3

void timer0init();
void timer0pwmModify(uint8_t value,pwmMode);
void timer0pwmInit();
void timer0stop();
void timer0start();
void timer0intDisable();
void timer0intOVFEnable();
void timer0clear();
