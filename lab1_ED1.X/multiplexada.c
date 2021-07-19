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
void multiplexada(uint8_t numero)
{
    switch(numero)
    {
        case(0):
            PORTD=0b00111111;
            break;
            
        case(1):
            PORTD=0b01011011;
            break;
            
        case(2):
            PORTD=0b01011011;
            break;
            
        case(3):
            PORTD=0b01001111;
            break;
            
        case(4):
            PORTD=0b01100110;
            break;
            
        case(5):
            PORTD=0b01101101;
            break;
            
        case(6):
            PORTD=0b01111101;
            break;
            
        case(7):
            PORTD=0b00000111;
            break;
            
        case(8):
            PORTD=0b01111111;
            break;
            
        case(9):
            PORTD=0b01101111;
            break;
            
    }
}


    

    