/*
 * File:   main.c
 * Author: Zahir Muhammad, Diego Reynosa
 *
 * Lab 5 Part 2 - Two-Digit Seven-Segment Seconds Counter (Common Anode)
 * ECE 3301L - Introduction to Microcontrollers Laboratory
 *
 * Description:
 *   Counts from 00 to 59, then resets to 00.
 *   Uses two common-anode 7-segment displays with digit multiplexing.

 */

#include <xc.h>
#include <stdint.h>

#pragma config FOSC = INTIO67, PLLCFG = OFF, PRICLKEN = ON, FCMEN = OFF, IESO = OFF
#pragma config PWRTEN = ON, BOREN = OFF, WDTEN = OFF
#pragma config PBADEN = OFF, MCLRE = EXTMCLR
#pragma config LVP = ON, XINST = OFF, STVREN = ON, DEBUG = OFF

#define _XTAL_FREQ 16000000UL

#define DIG1  0x10      
#define DIG2  0x20      
#define BLANK 0x7F      


static const uint8_t sevenSeg[10] = {
    0x40,   // 0
    0x79,   // 1
    0x24,   // 2
    0x30,   // 3
    0x19,   // 4
    0x12,   // 5
    0x02,   // 6
    0x78,   // 7
    0x00,   // 8
    0x10    // 9
};


static void init(void) {
    OSCCON = 0x72;                          

    ANSELB = 0; ANSELC = 0; ANSELD = 0;     
    TRISB &= 0x80;  LATB = (LATB & 0x80) | BLANK;   
    TRISC &= 0x80;  LATC = (LATC & 0x80) | BLANK;  
    TRISD &= ~(DIG1 | DIG2);                       
    LATD  &= ~(DIG1 | DIG2);                       
}

/* Multiplex both digits for ~1 second
 * 5 ms per digit -> 100 Hz refresh per digit, no visible flicker */
static void display_one_second(uint8_t tens, uint8_t ones) {
    for (uint8_t i = 0; i < 100; i++) {
        LATD &= ~DIG2;                                
        LATB  = (LATB & 0x80) | sevenSeg[tens];         
        LATD |= DIG1;                                  
        __delay_ms(5);

        LATD &= ~DIG1;                                  
        LATC  = (LATC & 0x80) | sevenSeg[ones];         
        LATD |= DIG2;                                   
        __delay_ms(5);
    }
    LATD &= ~(DIG1 | DIG2);                             
}

/* Seconds counter: 00 to 59, then reset */
void main(void) {
    init();
    uint8_t seconds = 0;

    while (1) {
        display_one_second(seconds / 10, seconds % 10);
        seconds++;
        if (seconds > 59) seconds = 0;
    }
}