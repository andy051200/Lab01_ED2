/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 1
Hardware: PIC16F887
    
Creado: 16 de julio de 2021    
Descripcion: 
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // master clear off 
#pragma config CP = OFF         // Code Protection bit off
#pragma config CPD = OFF        // Data Code Protection bit off
#pragma config BOREN = OFF      // Brown Out Reset Selection bits off
#pragma config IESO = OFF       // Internal External Switchover bit off
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit off 
#pragma config LVP = OFF        // Low Voltage Programming Enable bit off 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>                 //se incluye libreria del compilador
#include <stdint.h>             //se incluye libreria
#include <pic16f887.h>          //se incluye libreria del pic
#include "adc_config.h"         //se incluye libreria del adc
#include "multiplexada.h"       //se incluye libreria de multiplexada

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);       //prototipo de funcion de configuracion
void botones(void);     //prototipo de funcion de los botones
void transistores(void);    //protoripo de funcion de transistores

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char antirrebote1=0; //variable de antirrebote para boton SUMA
unsigned char antirrebote2=0; //variable de antirrebote para boton RESTA
unsigned char multiplex=0;
unsigned char display1;
unsigned char display2;

//unsigned char a,b,c ; //a para centenas, b para decenas, c para unidades
unsigned int conversion;

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //-----interrupcion de Timer0
    if (INTCONbits.T0IF)
    {
        multiplex++;
        INTCONbits.T0IF=0;
    }
    
    //-----interrupcion por IOCB
    if (INTCONbits.RBIF)
    {
        switch(PORTB)
        {
            case(0b11111110):     //si se apacha el boton de SUMA
                antirrebote1=1;
                break;
            
            case(0b11111101):     //si se apacha el boton de RESTA
                antirrebote2=1;
                break;
            
            default:                    //si alguno no es apachado
                antirrebote1=0;
                antirrebote2=0;
                break;
        }
        INTCONbits.RBIF=0;              //se apaga bandera de interrupcion
    }
    
    //-----interrupcion por converscion del ADC
    if (PIR1bits.ADIF==1) 
    {
        conversion=ADRESH;  //se almacena el valor del ADRESH en variable
        ADCON0bits.GO = 1; //se activa medición inicial
        PIR1bits.ADIF=0;    //se aaga bandera de interrupcion de ADC
    }
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    __delay_ms(20);
    ADCON0bits.GO = 1; //se activa medición inicial
    
    while(1)
    {
        //-----INVOCACION DE FUNCIONES
        transistores(); //llamo funcion de multiplexada de displays
        botones();      //llamo funcion para los botones de suma y resta
        
        //-----DIVISION DE DIGITOS PARA DISPLAYS 7 SEGMENTOS
        display1 = ADRESH & 0x0f;
        display2 = ADRESH & 0xf0;
        display2 = display2 >> 4;
        
        
        //-----LED DE ADVERTENCIA DE IGUALDAD
        if (PORTC==ADRESH)
            PORTDbits.RD7=1;
        else
            PORTDbits.RD7=0;
        
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //CONFIGURACION DE ENTRDAS ANALOGICAS
    ANSEL=0;                //sin entradas analógicas
    ANSELH=0;               //sin entradas analogicas
    ANSELbits.ANS0=1;       //An0 como entrada analogica, potenciometro
    
    //CONFIGURACION DE IN-OUT DE PUERTOS
    TRISAbits.TRISA0=1;     //AN0 como entrada de potanciometro
    TRISAbits.TRISA0=0;     //AN1 como salida de led de alerta
    
    TRISBbits.TRISB0=1;     //RB0 como entrada, boton SUMA
    TRISBbits.TRISB1=1;     //RB1 como entrada, boton RESTA

    TRISC=0;                //PortC como salida, contador LEDS
    TRISD=0;                //PortD como salida, 7 segmentos
    TRISE=0;                //PortE como salida, multiplexada 7 segmentos
    
    PORTA=0;             //se limpia PortA
    PORTB=0;             //se limpia PortA
    PORTC=0;             //se limpia PortE
    PORTD=0;             //se limpia PortE
    PORTE=0;             //se limpia PortE
    
    //CONFIGURACION DE RELOJ
    OSCCONbits.IRCF = 0b110; //Fosc 4MHz
    OSCCONbits.SCS = 1;      //configuracion de reloj interno
    
    //CONFIGURACION DEL TIMER0
    OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
    OPTION_REGbits.PS2 = 1;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 255;             // preset for timer register

    //WEAK PULL UPs PORTB
    OPTION_REGbits.nRBPU = 0;   // enable Individual pull-ups
    WPUBbits.WPUB0 = 1;         // enable Pull-Up de RB0 
    WPUBbits.WPUB1 = 1;         // enable Pull-Up de RB1 
    WPUBbits.WPUB2 = 1;         // enable Pull-Up de RB2 

    //IMPORTAR FUNCION DEL ADC DESDE LIBRERIA
    adc_config();               //se llama funcion
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilitan las interrupciones globales
    INTCONbits.T0IE=1;          // enable bit de int timer0
    INTCONbits.T0IF=0;        //se apaga la bandera de int timer0
    INTCONbits.RBIE=1;          // se habilita IntOnChange
    INTCONbits.RBIF=0;          // se apaga la bandera de IntOnChangeB  
    //CONFIGURACION INTERRUPCION DEL ADC
    PIE1bits.ADIE = 1 ; //se prende interrupcion por ADC
    PIR1bits.ADIF = 0; // se baja bandera de conversion
    //INTERRUPT ON CHANGE PORT B
    IOCBbits.IOCB0=1;           //se habilita IOCB RB0
    IOCBbits.IOCB1=1;           //se habilita IOCB RB1
    return;
}

/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

//------funcion para los botones de suma y resta
void botones(void)
{
    //-----antirrebote boton SUMA
    if (antirrebote1==1 && PORTBbits.RB0==0)
    {
        PORTC++;
        antirrebote1=0;
    }
    //-----antirrebote boton RESTA
    if (antirrebote2==1 && PORTBbits.RB1==0)
    {
        PORTC--;
        antirrebote2=0;
    }
    
    return;
}

//------funcion para los transistores en PortE
void transistores(void)
{
    switch(multiplex)
    {
        case(1):
            multiplexada(display1);
            PORTEbits.RE0=1;    //se prende transistor 1
            PORTEbits.RE1=0;    //se apaga transistor 2
            PORTEbits.RE2=0;    //se apaga transistor 3
            break;
        case(2):
            multiplexada(display2);
            PORTEbits.RE0=0;    //se prende transistor 1
            PORTEbits.RE1=1;    //se apaga transistor 2
            PORTEbits.RE2=0;    //se apaga transistor 3
            break;
        case(3):
            multiplex=0;
            break;       
    }
    return;
}
