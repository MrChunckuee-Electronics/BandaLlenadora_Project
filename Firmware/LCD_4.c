//
//Libreria para LCD 2x16 en C18
//Autor: Jose Manuel Alvarado Farias
//

#include <stdio.h>
#include <portb.h>
#include <stdlib.h>
#include <usart.h>
#include <string.h>
#include <delays.h>
#include "LCD_4.h"

//void Escribe_Nibble(void)
//Funcion para escibir un nibble en el LCD
//@return: la función no regresa ningun valor
void Escribe_Nibble(void)
{
}
//void Escribe_Comando(char orden)
//Funcion para enviar un comando al display 
//@orden: es el comando que se le envia al display
//@envia_nibble:tiene la funcion de en mascarar el perametro enviado
//@guarda_valor:guarda el valor del puerto
//@return: la funcion no regresa ningun valor
void Escribe_Comando(char orden)
{
char envia_nibble;
EN=0;
RS=0;
envia_nibble=orden&0xF0;
PORTD=(PORTD&0x0F)|envia_nibble;
Estrobo();
envia_nibble=(orden<<4)&0xF0;
PORTD=(PORTD&0x0F)|envia_nibble;
Estrobo();
if((orden==Home_LCD)||(orden==Borra_LCD))
Delay100TCYx(0x60);
else 
Delay10TCYx(0x19);
}
//void Escribe_letra(char letra)
//Funcion para enviar un caracter al display 
//@letra: es el caracter que se le envia al display
//@envia_nibble:tiene la funcion de en mascarar el perametro enviado
//@return: la funcion no regresa ningun valor
void Escribe_letra(char orden)
{
char envia_nibble;
EN=0;
RS=1;
envia_nibble=orden&0xF0;
PORTD=(PORTD&0x0F)|envia_nibble;
Estrobo();
envia_nibble=(orden<<4)&0xF0;
PORTD=(PORTD&0x0F)|envia_nibble;
Estrobo();
Delay10TCYx(0x19);
}
//void Inicializa_LCD()
//Configura el LCD alfanumerico, con una interface de 4 bits
//@return: la función no regresa ningun valor
void Inicializa_LCD(void)
{
//PORTD=0x30;
//Estrobo();
//Escribe_Comando(0x30);
//PORTD=0x20;
//Estrobo();
//Escribe_Comando(0x30);
Escribe_Comando(0x20);
Escribe_Comando(0x28);
Escribe_Comando(0x0F);
Escribe_Comando(0x01);
Escribe_Comando(0x06);
}
//void Estrobo()
//Genera pulso para habilitar LCD
void Estrobo(void)
{
EN=1;
Delay10TCYx(1);
EN=0;
}
//void Cadena2LCD(char *message)
//Envia una cadena hacia el LCD, donde se encuentra ubicado el cursor
//@message, es una constante de cadena
//@return: la función no regresa ningun valor
void Cadena2LCD(char *message)
{
char index;
for(index=0;*message;index++)
	{
	Escribe_letra(*message);
	message++;
	}
}
//void retardo_segundos(int tiempo)
//Produce un retardo de tiempo*segundo, tomando en cuenta  el ciclo de reloj de .2us
//@tiempo: numero de segundos que tiene el retardo
//@return: la función no regresa ningun valor
void retardo_segundos(int tiempo)
{
char ciclo;
do
	{
	ciclo=5;
	do
		{
		Delay10KTCYx(100);
		ciclo--;
		}
	while(ciclo);
	tiempo--;
	}
while(tiempo);
}
//void limpia_linea(char linea)
//limpia la linea propuesta
//@linea renglon al que salta el lcd
//ejemplo: limpia_linea(Linea1_LCD)
//@return: la función no regresa ningun valor
void limpia_linea(char linea)
{
char conteo=16;
if(linea==Linea1_LCD||linea==Linea2_LCD||linea==Linea3_LCD||linea==Linea4_LCD)
	{
	Escribe_Comando(linea);
	do
		{
		Escribe_letra(Espacio);
		conteo--;
	}
	while(conteo);
	Escribe_Comando(linea);
	}
}
//void limpia_espacio(char linea,char coordenada,char ancho)
//limpia un numero "ancho" de espacios en la coordenada "x" de la linea "y"
//@linea renglon al que salta el lcd
//@coordenada direccrion del primer espacio a limpiar
//@ancho cantidad de espacios a limpiar, menor a 16
//@return: la función no regresa ningun valor
//ejemplo: limpia_linea(Linea1_LCD,3,5)
void limpia_espacio(char linea,char coordenada,char ancho)
{
signed char conteo;
conteo=16-coordenada;
if(conteo>0&&(linea==Linea1_LCD||linea==Linea2_LCD||linea==Linea3_LCD||linea==Linea4_LCD))
	{
	Escribe_Comando(linea+coordenada);
	do
		{
		Escribe_letra(Espacio);
		ancho--;
		conteo--;
	}
	while(ancho&&conteo);
	Escribe_Comando(linea+coordenada);
	}
}
//void cadena_xy(char *message,char linea,char coordenada)
//despliega una cadena de caracteres rn RAM en la linea y coordenada especificas
//@message es una constante de cadena
//@linea es la linea en la que se desea poner la cadema de caracteres
//@coordenada es la coordenada con respecto a la linea o renglon especifica 
//@return: la función no regresa ningun valor
void cadena_xy(char  *message,char linea,char coordenada)
{
char index;
signed char conteo;
conteo=16-coordenada;
if(conteo>0&&(linea==Linea1_LCD||linea==Linea2_LCD||linea==Linea3_LCD||linea==Linea4_LCD))
{
Escribe_Comando(linea+coordenada);
for(index=0;*message&&conteo;index++,conteo--)
	{
	Escribe_letra(*message);
	message++;
	}
}
}
void caracter_xy(char letra,char coordenada)
{
Escribe_Comando(coordenada);
Escribe_letra(letra);
}
//void cadena_rom(char *message,char linea,char cordenada)
//envia una cadena de la memoria de programa al LCD
//@message contiene una cadena guardada en la ROM
//@linea donde se quiere poner el dato
//@coordenada puntero donde se pone al dato con respecto a la linea
void cadena_rom(char *message,char linea,char coordenada)
{
//@cadena_trabajo arreglo que se utliza para copiar a RAM las constantes guardadas en memoria de programa
char cadena_trabajo[17];
strcpypgm2ram( cadena_trabajo, message);
cadena_xy(cadena_trabajo,linea,coordenada);

}