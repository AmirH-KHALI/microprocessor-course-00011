#include <mega32.h>
#include <alcd.h>
#include <delay.h>      
#include <stdlib.h>
#include <stdio.h>

#define KEYPAD_R1 PORTD.0
#define KEYPAD_R2 PORTD.1
#define KEYPAD_R3 PORTD.2
#define KEYPAD_R4 PORTD.3
#define KEYPAD_C1 PIND.4
#define KEYPAD_C2 PIND.5
#define KEYPAD_C3 PIND.6
#define KEYPAD_C4 PIND.7

#define KEYPAD_NUM0 '0'
#define KEYPAD_NUM1 '1'
#define KEYPAD_NUM2 '2'
#define KEYPAD_NUM3 '3'
#define KEYPAD_NUM4 '4'
#define KEYPAD_NUM5 '5'
#define KEYPAD_NUM6 '6'
#define KEYPAD_NUM7 '7'
#define KEYPAD_NUM8 '8'
#define KEYPAD_NUM9 '9'

#define KEYPAD_DIVISION '/'
#define KEYPAD_ADDITION '+'
#define KEYPAD_MINUS '-'
#define KEYPAD_MULTIPLICATION '*'
#define KEYPAD_EQUAL '='
#define KEYPAD_AC 'C'

unsigned char keypad_scan();

float calculate(float num1, float num2, unsigned char operation) {
    float result;
    switch (operation)
        {
            case KEYPAD_ADDITION: 
                result = num1 + num2;   
                break;
            case KEYPAD_MULTIPLICATION: 
                result = num1 * num2; 
                break;
            case KEYPAD_DIVISION: 
                result = num1 / num2;
                break;
            case KEYPAD_MINUS: 
                result = num1 - num2;
                break;
            default: break;
        }
    return result;
}


float number[3] = {0, 1, 0};
char operation[2] = {KEYPAD_ADDITION, KEYPAD_MULTIPLICATION};
int operation_enter = 0; 
int equal_enter = 1;
char buffer[32];

void main(void) {
    unsigned char key_res;
    DDRC = 0xFF;
    DDRD = 0x0F;
    PORTC = 0x00;

    lcd_init(16);
    lcd_clear();

    while (1) {
        key_res = keypad_scan();
        
        if (key_res != 255) {
            while (keypad_scan() != 255);
            delay_ms(20);
            
            if (key_res == KEYPAD_AC) {
                lcd_clear();

                number[0] = 0;
                number[1] = 1;
                number[2] = 0;
                operation[0] = KEYPAD_ADDITION;
                operation[1] = KEYPAD_MULTIPLICATION;
                operation_enter = 0;
                equal_enter = 1;
                   
            } else if (key_res == KEYPAD_EQUAL) {
                if (operation_enter != 1 && equal_enter != 1) {
                    number[1] = calculate(number[1], number[2], operation[1]);
                    number[2] = 0;
                    number[0] = calculate(number[0], number[1], operation[0]);
                    number[1] = 0;
                    
                    lcd_gotoxy(0, 1);
                    sprintf(buffer, "%.2f", number[0]);
                    lcd_puts(buffer);

                    number[0] = 0;
                    number[1] = 1;
                    number[2] = 0;
                    operation[0] = KEYPAD_ADDITION;
                    operation[1] = KEYPAD_MULTIPLICATION;
                    operation_enter = 0;
                    equal_enter = 1;
                }
                
            } else if (key_res == KEYPAD_DIVISION || key_res == KEYPAD_MULTIPLICATION || key_res == KEYPAD_MINUS || key_res == KEYPAD_ADDITION) {
                if (operation_enter != 1 && equal_enter != 1) {
                    if (key_res == KEYPAD_DIVISION || key_res == KEYPAD_MULTIPLICATION) {
                        number[1] = calculate(number[1], number[2], operation[1]);
                        number[2] = 0;
                        operation[1] = key_res;    
                    } else {
                        number[1] = calculate(number[1], number[2], operation[1]);
                        number[2] = 0;
                        number[0] = calculate(number[0], number[1], operation[0]);
                        number[1] = 1;
                        operation[0] = key_res;
                        operation[1] = KEYPAD_MULTIPLICATION;
                    }
                    lcd_putchar(key_res);
                    
                    //lcd_clear();
                    //sprintf(buffer, "%.2f %c %.2f %c %.2f = %.2f", number[0], operation[0], number[1], operation[1], number[2], calculate(number[0], calculate(number[1], number[2], operation[1]), operation[0]));
                    //lcd_puts(buffer);
                    
                    operation_enter = 1;
                }
            } else {
                if (equal_enter == 1) {
                    lcd_clear();
                }
                equal_enter = 0;    
                operation_enter = 0;
                lcd_putchar(key_res);
            
                number[2] *= 10;
                number[2] += key_res - 48;
                
                //lcd_clear();
                //sprintf(buffer, "%.2f %c %.2f %c %.2f = %.2f", number[0], operation[0], number[1], operation[1], number[2], calculate(number[0], calculate(number[1], number[2], operation[1]), operation[0]));
                //lcd_puts(buffer);
            }
                   
        }
    }
}

unsigned char keypad_scan() {
    unsigned char result = 255;

    ////////////////////////  ROW1 ////////////////////////
    KEYPAD_R1 = 1;
    KEYPAD_R2 = 0; 
    KEYPAD_R3 = 0; 
    KEYPAD_R4 = 0;
    
    delay_ms(5);
    if (KEYPAD_C1)
        result = KEYPAD_NUM7;
    else if (KEYPAD_C2)
        result = KEYPAD_NUM8;
    else if (KEYPAD_C3)
        result = KEYPAD_NUM9;
    else if (KEYPAD_C4)
        result = KEYPAD_DIVISION;
        

    ////////////////////////  ROW2 ////////////////////////
    KEYPAD_R1 = 0;
    KEYPAD_R2 = 1; 
    KEYPAD_R3 = 0;
    KEYPAD_R4 = 0;
    
    delay_ms(5);
    if (KEYPAD_C1)
        result = KEYPAD_NUM4;
    else if (KEYPAD_C2)
        result = KEYPAD_NUM5;
    else if (KEYPAD_C3)
        result = KEYPAD_NUM6;
    else if (KEYPAD_C4)
        result = KEYPAD_MULTIPLICATION;
    ////////////////////////  ROW3 ////////////////////////
    KEYPAD_R1 = 0;
    KEYPAD_R2 = 0;
    KEYPAD_R3 = 1;
    KEYPAD_R4 = 0;
    
    delay_ms(5);
    if (KEYPAD_C1)
        result = KEYPAD_NUM1;
    else if (KEYPAD_C2)
        result = KEYPAD_NUM2;
    else if (KEYPAD_C3)
        result = KEYPAD_NUM3;
    else if (KEYPAD_C4)
        result = KEYPAD_MINUS;
    ////////////////////////  ROW4 ////////////////////////
    KEYPAD_R1 = 0;
    KEYPAD_R2 = 0;
    KEYPAD_R3 = 0;
    KEYPAD_R4 = 1;
    
    delay_ms(5);
    if (KEYPAD_C1)
        result = KEYPAD_AC;
    else if (KEYPAD_C2)
        result = KEYPAD_NUM0;
    else if (KEYPAD_C3)
        result = KEYPAD_EQUAL;
    else if (KEYPAD_C4)
        result = KEYPAD_ADDITION;

    return result;
}
