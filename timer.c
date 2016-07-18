#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

void timer0start(){
    uint8_t prescalevalue = 0x3;
    TCCR0B|=prescalevalue<<CS00;
}

void timer0clear(){
    TCNT0 = 0;
}

void timer0intEnable(){
    TIMSK0 |= (1<<TOIE0);
}

void timer0intDisable(){
    TIMSK0 &= ~(1<<OCIE0B|1<<OCIE0A|1<<TOIE0);  //clear all interrupts
}

void timer0stop(){
    TCCR0B &= ~(0x7);
}

void timer0counterInit(){
    //Clear timer on compare match mode, OCRA
    TCCR0A=(1<<WGM01)|(0<<WGM00)|(0<<COM0A0)|(0<<COM0B0); //Timer stopped
    OCR0A=prescaleTimer0;
    TIMSK0&=~(0x3);                  //clear TIMSK register, in bits from timer 0
    TIMSK0|=1<<TOIE0|1<<OCIE0A;       //program TIMSK register, interruption on top and overflow

}

void timer0pwmInit(){
    //Warning, atm644 has 2 OC0, but atm128 has only one...
    //you also should have your set pins...
    uint8_t wgmvalue=3;   //Fast PWM set
    DDRB |= 1<<PB4|1<<PB3; // Set port D pins as outputs
    TCCR0A = (wgmvalue&0x3)<<WGM00|NONINVERTING<<COM0A0|NONINVERTING<<COM0B0; // Timer reg 1
    TCCR0B = ((wgmvalue>>2)&1)<<WGM02; // Timer reg 2
    uint8_t pwmvalue = 10; // Set pwm to 10%
    uint16_t regvalue = ((uint16_t)pwmvalue*255)/100;
    OCR0A = (uint8_t)regvalue;
    OCR0B = (uint8_t)regvalue;
    TIMSK0 &= ~(1<<OCIE0B|1<<OCIE0A|1<<TOIE0);  //clear all interrupts as defeult
    TCCR0B|=0x0<<CS00;   //timer stopped

}

void timer0pwmModify(uint8_t value,pwmMode mode){
    //Warning -> read warnings for timer0pwmInit
    TCCR0B &= ~0x7;  //timer stop
    if(mode == PERCENT){
        uint16_t regvalue = ((uint16_t)value*255)/100;
        OCR0B = (uint8_t)regvalue;
        OCR0A = (uint8_t)regvalue;
    }
    else{
        OCR0B = value;
        OCR0A = value;
    }
    TCCR0B |= 0x3 <<CS00; //timer start
}
