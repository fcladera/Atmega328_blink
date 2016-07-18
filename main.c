/*
 * fclad
 * Awesome sinus led blink!
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <stdint.h>

#include "timer.h"

//PWM Sinus generation
const uint8_t sinus[256]={127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,179,182,184,187,190,193,195,198,
  200,203,205,208,210,213,215,217,219,221,224,226,228,229,231,233,235,236,238,239,241,242,244,245,246,
  247,248,249,250,251,251,252,253,253,254,254,254,254,254,255,254,254,254,254,254,253,253,252,251,251,
  250,249,248,247,246,245,244,242,241,239,238,236,235,233,231,229,228,226,224,221,219,217,215,213,210,
  208,205,203,200,198,195,193,190,187,184,182,179,176,173,170,167,164,161,158,155,152,149,146,143,139,
  136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,75,72,70,67,64,61,59,56,54,
  51,49,46,44,41,39,37,35,33,30,28,26,25,23,21,19,18,16,15,13,12,10,9,8,7,6,5,4,3,3,2,1,1,0,0,0,0,0,
  0,0,0,0,0,0,1,1,2,3,3,4,5,6,7,8,9,10,12,13,15,16,18,19,21,23,25,26,28,30,33,35,37,39,41,44,46,49,51,
  54,56,59,61,64,67,70,72,75,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124};

//-----------------INTERRUPTIONS -----------------//

//Timer 0 Overflow interruption -> PWM leds
/*
Change la valeur du pwm, pour suivre une signal sinusoidale
*/
ISR(TIMER0_OVF_vect){
    static uint8_t ovfs = 0;
    ovfs++;
    /* Period - Prescale calcs
    #frequence du micro
    fosc = 16e6
    tosc = 1/fosc
    #periode de tic de l'horloge
    ttic = tosc * 64
    #periode avant le debordement du timer
    tclk = 0xFF*ttic
    tclk
    0.0010199999999999999
    #on va parcourir 256 valeurs de pwm
    256*tclk
    0.26111999999999996
    #on ajoute un diviseur ovfs pour avoir une periode plus petite
    256*tclk*8
    2.0889599999999997
    #==> Le periode de la sinusoide est de 2 s environ
    */
    static uint8_t counter=0;
    if(ovfs==8){    //software prescale value
        ovfs=0;
        timer0pwmModify(sinus[counter],POWTWO);
        counter++;
        //Counter goes from 255 to 0 automatically (overflow of uint8)
    }
    TIFR0 = 1<<TOV0;    //Deactivate interruption
}

int main(){
    DDRB = 1<<PB4|1<<PB3;   //leds pwm are outputs

    timer0pwmInit();
    timer0clear();
    timer0intEnable();
    timer0start();

    sei();

    while(1);
    return 0;
}
