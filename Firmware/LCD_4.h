#ifndef __LCD_4_H
#define __LCD_4_H

// Libreria para LCD 16x2 controlada a 4 Lineas
// Autor: Jose Manuel Alvarado Farias


//Declaración de terminales utilizadas por el display alfanumerico

#define 	RS		    	PORTDbits.RD2
#define 	EN		     	PORTDbits.RD3
#define 	Dato_4			PORTDbits.RD4
#define 	Dato_5			PORTDbits.RD5
#define 	Dato_6			PORTDbits.RD6
#define 	Dato_7			PORTDbits.RD7

//Declaración de constantes utlizadas por comados del LCD

#define		Apaga_LCD	   		8
#define 	Enciende_LCD		12
#define		Enciende_cursor		14
#define		Apaga_cursor		10
#define		Borra_LCD			1
#define 	Home_LCD			2
#define		Linea1_LCD			0x80
#define		Linea2_LCD			0xC0
#define		Linea3_LCD			0X90
#define		Linea4_LCD			0xD0
#define		Espacio				0X20

//Declaracion de funciones utilizadas por el LCD;

void Escribe_Comando(char orden);
void Escribe_letra(char letra);
void Inicializa_LCD(void);
void Escribe_Nibble(void);
void Estrobo(void);
void Cadena2LCD(char *message);
void retardo_segundos(int tiempo);
void limpia_linea(char linea);
void limpia_espacio(char linea,char cordenada,char ancho);
void cadena_xy(char *message,char linea,char cordenada);
void caracter_xy(char letra,char coordenada);
void cadena_rom(char *message,char linea,char cordenada);

#endif