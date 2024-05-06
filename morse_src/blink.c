#include <stdio.h>

#include "blink.h"

// WiringX API header
#include <wiringx.h>

//LED pin
#define DUO_LED 25

// Fill in the three functions to
// 1.) enable the output mode for onboard LED
// 2.) set the led to high
// 3.) set the led to low

// Write code to enable led
void initialize_led()
{
    // Code to initialize gpio port for LED

    // Verify the pin is available
    if(wiringXValidGPIO(DUO_LED) != 0) {
        printf("Invalid GPIO %d\n", DUO_LED);
        wiringXGC();
        //return -1;
    }

    // Initialize the GPIO pin as PINMODE_OUTPUT
    pinMode(DUO_LED, PINMODE_OUTPUT);
    printf("Duo LED Intialized %d\n", DUO_LED);
 
}

// Write code to turn led on
void turn_on_led()
{
    // Code to turn on the LED
    printf("Duo LED GPIO (wiringX) %d: High\n", DUO_LED);
    digitalWrite(DUO_LED, HIGH);
}

// Write code to turn led off
void turn_off_led()
{
    // Code to turn off the LED
    //printf("Duo LED GPIO (wiringX) %d: Low\n", DUO_LED);
    digitalWrite(DUO_LED, LOW);
}

