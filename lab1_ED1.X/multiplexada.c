/* 
 * File:   multiplexada.h
 * Author: Andy Bonilla
 *
 * Created on 17 de julio de 2021, 11:32 PM
 */

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>                 //se incluye libreria del compilador
#include <stdint.h>             //se incluye libreria
#include <pic16f887.h>          //se incluye libreria del pic
#include "multiplexada.h"
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
-------------------------------- variables -----------------------------------
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-------------------------------- FUNCIONES -----------------------------------
-----------------------------------------------------------------------------*/
void multiplexada(unsigned char numero)
{
    switch(numero)
    {
        case(0):
            PORTD=0b10111111;   //equivalente a 0
            break;
            
        case(1):
            PORTD=0b01011011;   //equivalente a 1
            break;
            
        case(2):
            PORTD=0b01011011;   //equivalente a 2
            break;
            
        case(3):
            PORTD=0b01001111;   //equivalente a 3
            break;
            
        case(4):
            PORTD=0b01100110;   //equivalente a 4
            break;
            
        case(5):
            PORTD=0b01101101;   //equivalente a 5
            break;
            
        case(6):
            PORTD=0b01111101;   //equivalente a 6
            break;
            
        case(7):    
            PORTD=0b00000111;   //equivalente a 7
            break;
            
        case(8):
            PORTD=0b01111111;   //equivalente a 8
            break;
            
        case(9):
            PORTD=0b01101111;   //equivalente a 9
            break;
        
        case(10):
            PORTD=0b01110111;   //equivalente a A
            break;
        
        case(11):
            PORTD=0b01111100;   //equivalente a B
            break;
        
        case(12):
            PORTD=0b00111001;   //equivalente a C
            break;
        
        case(13):
            PORTD=0b01011110;   //equivalente a D
            break;
        
        case(14):
            PORTD=0b01111001;   //equivalente a E
            break;
        
        case(15):
            PORTD=0b01110001;   //equivalente a F
            break;
             
    }
    return;
}


    

    