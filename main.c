#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include <avr/io.h>
#include "h_bridge_X.h"
#include "h_bridge_Y.h"
#include "h_bridge_Z.h"
#include <util/delay.h>

#define KNOP_S1 !(PINF & (1 << PF1))        // arduino shield pin s1 PF1
#define KNOP_S2 !(PINF & (1 << PF2))        // arduino shield pin s2 PF2

#define LAMP_D1_AAN PORTB &= ~(1 << PB7)     // Lampje aan
#define LAMP_D1_UIT PORTB |= (1 << PB7)    // Lampje uit

void init(void)
{
    init_h_bridge_X();    // initialiseer geleverde h_brug code
    init_h_bridge_Y();    // initialiseer geleverde h_brug code
    init_h_bridge_Z();    // initialiseer geleverde h_brug code

    DDRB |= (1 << PB7);     // LED pin output
    PORTB |= (1 << PB7);    // LED off
    DDRF &= ~((1 << PF1) | (1 << PF2));     // zet PF1 en PF2 (S1 & S2) input voor knopjes shield
}

int Motor_X_Rechtsom()           // functie motor_X_rechtsom (status 1 = opstarten; 2 = draaien op snelheid; 3 = slow down
{
    LAMP_D1_AAN;            // zet lamp D1 aan op arduino shield

    for(int i = 0; i < 100; i++)
    {
        h_bridge_Y_set_percentage(i);
        _delay_ms(20);
    }
    for(int i = 100; i > 0; i--)
    {
        h_bridge_Y_set_percentage(i);
        _delay_ms(20);
    }
}

void Motor_X_Linksom(void)
{
    LAMP_D1_UIT;
    for(int i = 0; i > -100; i--)
    {
        h_bridge_Y_set_percentage(50);
        _delay_ms(20);
    }
    for(int i = -100; i < 0; i++)
    {
        h_bridge_Y_set_percentage(-50);
        _delay_ms(20);
    }
}

int main(void)
{
    init(); // run init een keer om pinnen te initialiseren en declareren

    while(1) // in de while loop programma code:
    {
        if (KNOP_S1) // als knop S1 is ingedrukt gaat de motor aan
        {
            Motor_X_Rechtsom();
        }

        if (KNOP_S2) // als knop S1 niet is ingedrukt is de motor uit
        {
            Motor_X_Linksom();
        }
    }
    return 0;
}
