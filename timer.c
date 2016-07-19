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

void timer0intOVFEnable(){
    TIMSK0 = (1<<TOIE0);
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
    TIMSK0&=~(0x3); //clear TIMSK register, in bits from timer 0
    TIMSK0|=1<<TOIE0|1<<OCIE0A; //program TIMSK register, interruption on top and overflow

}

void timer0pwmInit(){
    timer0stop();
    timer0clear();
    uint8_t wgmvalue=3;   //Fast PWM set
    DDRD |= 1<<PD5|1<<PD6; // Set port D pins as outputs
    TCCR0A = (wgmvalue&0x3)<<WGM00|NONINVERTING<<COM0A0|NONINVERTING<<COM0B0; // Timer reg 1
    TCCR0B = ((wgmvalue>>2)&1)<<WGM02; // Timer reg 2
    timer0intOVFEnable();
    timer0pwmModify(50, PERCENT); // Set pwm to 50%
}

void timer0pwmModify(uint8_t value,pwmMode mode){
    //Warning -> read warnings for timer0pwmInit
    timer0stop();
    if(mode == PERCENT){
        uint16_t regvalue = ((uint16_t)value*255)/100;
        OCR0B = (uint8_t)regvalue;
        OCR0A = (uint8_t)regvalue;
    }
    else{
        OCR0B = value;
        OCR0A = value;
    }
    timer0start();
}
