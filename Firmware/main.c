/////////////////////////////////////////////////////////////////////////////////////
////////////////////                   				        	                        ///////////////////
////////////////////  FIRMWARE PARA BANDA TRANSPORTADORA   ///////////////////
////////////////////         INGENIERÍA ELECTRÓNICA     			  ///////////////////
////////////////////                TESCHA               				  	 ///////////////////
////////////////////     TEORIA DE LA AUTOMATIZACIÓN      	        ///////////////////
////////////////////                                      						///////////////////
////////////////////   AUTORES:        				                     ///////////////////
////////////////////      JOSE ANGEL TENORIO ALARCON		       ///////////////////
////////////////////      PEDRO SÁNCHEZ RAMÍREZ         			///////////////////
////////////////////                                      						///////////////////
///////////////////////////////////////////////////////////////////////////////////

//Librerias utilizadas
#include <p18f4620.h>
#include <stdio.h>
#include <portb.h>
#include <stdlib.h>
#include <usart.h>
#include <string.h>
#include <delays.h>
#include "LCD_4.h"

//Configuración del PIC18F4620
#pragma config OSC = INTIO67
#pragma config FCMEN = ON
#pragma config IESO = ON
#pragma config PWRT = ON
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF
#pragma config CP0 = OFF 
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT3 = OFF 
#pragma config WRTC = OFF
#pragma config WRTB = OFF
#pragma config WRTD = OFF
#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF

//Etiquetas de Entradas/Salidas
#define BUTTON_START		PORTBbits.RB0
#define BUTTON_STOP			PORTBbits.RB1
#define BUTTON_INFO			PORTBbits.RB2
#define SENSOR_IR			PORTEbits.RE0
#define RELE_BOMBA			PORTCbits.RC0
#define RELE_BANDA			PORTCbits.RC1
#define LED_SENSOR			PORTCbits.RC2
#define LED_STATUS			PORTCbits.RC3
#define LED_BOMBA			PORTCbits.RC4
#define LED_BANDA			PORTCbits.RC5

//Declaración de funciones
void low_isr(void);
void high_isr(void);
void Config_Timer0(void);
void Config_USART(void);
void Rutina_Principal(void);
void Rutina_USART(void);

//Declaración de variables
char a, x, y;

//Definicion de estructura para campos de bits
struct BIT_FIELD
	{
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
	};

//Registro que guarda el estado de funciomaniento del sistema
struct BIT_FIELD BIC_REG;

#define STATUS_HIGH BIC_REG.bit0
#define STATUS_LOW BIC_REG.bit1

void main (void)
{//Inicio del main

//Configuración del oscilador interno a 4MHz
OSCCONbits.IRCF2=1;
OSCCONbits.IRCF1=1;
OSCCONbits.IRCF0=0;

//Todas las Entradas/Salidas digitales
ADCON1bits.PCFG3=1;
ADCON1bits.PCFG2=1;
ADCON1bits.PCFG1=1;
ADCON1bits.PCFG0=1;

//Se inicializán los puertos del PIC en cero
LATA=0;
LATB=0;
LATC=0;
LATD=0;
LATE=0;

//Configuración de Entradas/Salida
TRISD=0x00;
TRISBbits.RB0=1;
TRISBbits.RB1=1;
TRISBbits.RB2=1;
TRISEbits.RE0=1;
TRISCbits.RC0=0;
TRISCbits.RC1=0;
TRISCbits.RC2=0;
TRISCbits.RC3=0;
TRISCbits.RC4=0;
TRISCbits.RC5=0;
TRISCbits.RC6=0;//TX
TRISCbits.RC7=1;//RX

Config_Timer0();//Se activa el Timer 0
Config_USART();//Se activa la USART

//Se inicializa el LCD
Escribe_Comando(0x20);
Escribe_Comando(0x28);
Escribe_Comando(0x0F);
Escribe_Comando(0x01);
Escribe_Comando(0x06);
Escribe_Comando(Enciende_LCD);
Escribe_Comando(Borra_LCD);
Delay10KTCYx(5);//Espera 5 ms para configuración del LCD

//Mensaje de bienvenida
cadena_rom("ELECTRONIC", Linea1_LCD, 3);
cadena_rom("ENGINEERING", Linea2_LCD, 2);
putrsUSART("ELECTRONIC ENGINEERING\n\r");
Delay10KTCYx(200);

Escribe_Comando(Borra_LCD);
cadena_rom("THEORY", Linea1_LCD, 5);
cadena_rom("AUTOMATION", Linea2_LCD, 3);
putrsUSART("MATTER:\n\r");
putrsUSART("THEORY AUTOMATION\n\r");
Delay10KTCYx(200); 

Escribe_Comando(Borra_LCD);
cadena_rom("JOSE ANGEL T. A.", Linea1_LCD, 0);
cadena_rom("PEDRO SANCHEZ R.", Linea2_LCD, 0);
putrsUSART("BY:\n\r");
putrsUSART("JOSE ANGEL TENORIO ALARCON\n\r");
putrsUSART("PEDRO SANCHEZ RAMIREZ\n\r");
Delay10KTCYx(200);

Escribe_Comando(Borra_LCD);
cadena_rom("PRESS START", Linea1_LCD, 0);
cadena_rom("TO BEGIN", Linea2_LCD, 0);
Delay10KTCYx(5); 

putrsUSART("PRESS a TO BEGIN\n\r");

while(1)
{
LED_SENSOR=0;
LED_BOMBA=0;
Rutina_Principal();

}
}


void Rutina_Principal(void)
{

if(BUTTON_START==1||a==1||STATUS_HIGH==1||x==1)
	{
	Escribe_Comando(Borra_LCD);
	cadena_rom("CONVEYOR BELT", Linea1_LCD, 0);
	cadena_rom("WORKING", Linea2_LCD, 0);
       putrsUSART("CONVEYOR BELT WORKING \n\r");
	LED_BANDA=1;
	RELE_BANDA=1;
	a=0;
	//STATUS_HIGH=0;
	}


if(SENSOR_IR==0)
	{
	LED_BANDA=0;
	LED_SENSOR=1;	
	Delay10KTCYx(250);
	Delay10KTCYx(200);//Retardo para que quede en sitio de llenado
	Delay10KTCYx(200);
	RELE_BANDA=0;
	Escribe_Comando(Borra_LCD);
	cadena_rom("FIELDING", Linea1_LCD, 0);
	cadena_rom("BOAT", Linea2_LCD, 0);
       putrsUSART("FIELDING BOAT\n\r");
	LED_SENSOR=0;
	RELE_BOMBA=1;
	LED_BOMBA=1;
	Delay10KTCYx(250);
	Delay10KTCYx(250);//Tiempo de llenado
	RELE_BOMBA=0;
	LED_BOMBA=0;
	LED_BANDA=1;
	RELE_BANDA=1;
	Escribe_Comando(Borra_LCD);
	cadena_rom("CONVEYOR BELT", Linea1_LCD, 0);
	cadena_rom("WORKING", Linea2_LCD, 0);
       putrsUSART("CONVEYOR BELT WORKING \n\r");
	Delay10KTCYx(200);
	Delay10KTCYx(200);//
	a=1;
	}

if(BUTTON_STOP==1||y==1)
	{
	Reset();
	}

}




//Lineas de codigo para las interrupciones
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
  _asm GOTO low_isr _endasm
}
#pragma code /* return to the default code section */
#pragma interruptlow low_isr
void low_isr (void)
{
//Sentesncias de baja prioridad		
}
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
  _asm GOTO high_isr _endasm
}
#pragma code /* return to the default code section */

#pragma interrupt high_isr
void high_isr (void)
{
//Sentencias de alta prioridad			
char COM_RX;

if(INTCONbits.TMR0IF)
	{
	LED_STATUS^=1;
	TMR0H=0;
	TMR0L=3036;
	INTCONbits.TMR0IF=0;
	}

			
if(PIR1bits.RCIF)
	{
	COM_RX=ReadUSART();
	Rutina_USART();
/*
	if(COM_RX=='a')
		STATUS_HIGH=1;
	if(COM_RX=='b')
		STATUS_LOW=1;
*/
	}
PIR1bits.RCIF=0;



}


void Rutina_USART(void)
{
	if(COM_RX=='a')
		x=1;
	if(COM_RX=='b')
		y=1;
}

//Configuracion del TIMER 0
void Config_Timer0(void)
	{
	T0CON=0x83;
	TMR0H=0;
	TMR0L=3036;//Iniciamos en 3036 para que sean pulsos exactos de 1 seg
	INTCONbits.TMR0IF=0;
	INTCONbits.TMR0IE=1;
	INTCON2bits.TMR0IP=1;
	RCONbits.IPEN=1;
	INTCONbits.GIEH=1;
	}


void Config_USART(void)
	{
	OpenUSART (USART_TX_INT_OFF & 
	           USART_RX_INT_ON & 
	           USART_ASYNCH_MODE & 
	           USART_EIGHT_BIT & 
	           USART_CONT_RX & 
	           USART_BRGH_HIGH, 25);
	
	//Conficuracion de interrupciones para la USART
	PIR1bits.RCIF=1;/*The EUSART receive buffer, RCREG, is full (cleared when RCREG is read)*/
	IPR1bits.RCIP=1;/*High priority*/
	PIE1bits.RCIE=1;/*Enables the EUSART receive interrupt*/
	//Conficuracion de interrupciones generales
	RCONbits.IPEN=1;/*Enable priority levels on interrupts*/
	INTCONbits.GIEH=1;/*Enables all high-priority interrupts*/
	INTCONbits.GIEL=1;/*Enables all low-priority peripheral interrupts*/
	}
