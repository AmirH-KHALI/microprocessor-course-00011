#include <mega32.h>
#include <delay.h>

int counter = 0;
int pause = 0;

// Timer1 output compare A interrupt service routine
interrupt [TIM1_COMPA] void timer1_compa_isr(void) {
    if (!pause) {
        counter++;
        counter %= 10;
    }
}

interrupt [EXT_INT2] void ext_int2_isr(void) {
	pause ^= 1;
}

void main(void) {
    DDRA = 0x00;
    DDRC = 0xFF;
    
    GICR |= 0xE0;
    MCUCR = 0x0A;
    MCUCSR = 0x00;
    GIFR = 0xE0;
    
    // Timer 1 config
    TCCR1A = 0x00;
    TCCR1B = 0x0C;
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x7A;
    OCR1AL = 0x12;
    OCR1BH = 0x00;
    OCR1BL = 0x00;

    TIMSK = 0x12;

    // Global enable interrupts
    #asm("sei")
    
    while (1) {  
        PORTC = counter;
    }
}
