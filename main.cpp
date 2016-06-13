/*
 * Copyright (C) 2016 Antonio Horrillo Horrillo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Incluimos las librerías necesarias.
 */
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Definimos el array de enteros (int) que contendrá la información, siendo:
 * La posición cadena[0] = parte entera de la humedad.
 * La posición cadena[1] = parte decimal de la humedad.
 * La posición cadena[2] = parte entera de la temperatura.
 * La posición cadena[3] = parte decimal de la temperatura.
 * La posición cadena[4] = parte de comprobación.
 */
int cadena[5] = {0,0,0,0,0};

/**
 * Método principal main.
 */
int main(void) {

	/**
	 * Si no se reconoce el dispositivo, salimos. La función wiringPiSetup()
	 * es de la libreria wiringPi.h
	 * Si la salida de la funcion es -1 no se ha reconocido el hardware y se
	 * realiza un exit.
	 */
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	
	/**
	 * En caso contrario, se reconoce el dispositivo, e iniciamos el proceso.
	 */
	else {
		
		/**
	 	 * Declaramos las variables y las inicializamos.
	 	 */
		uint8_t estado		= HIGH;
		uint8_t j			= 0;
		uint8_t i			= 0;
		uint8_t contador	= 0;

		/**
	 	 * El sensor Dht11 está conectado al Gpio 21.
		 * Establecemos el modo "OUT" en el Gpio 21.
		 * Enviamos el estado "LOW" al Gpio 21.
		 * Establecemos una pausa, delay.  
	 	 */	 
		pinMode(21,OUTPUT);
		digitalWrite(21,LOW);
		delay(18);

		/**
		 * Enviamos el estado "HIGH" al Gpio 21.
		 * Establecemos una pausa, delayMicroseconds.
		 * Establecemos el modo "IN" en el Gpio 21.  
	 	 */			
		digitalWrite(21,HIGH);
		delayMicroseconds(40);
		pinMode(21,INPUT);

		/**
	 	 * Una vez enviada la señal, realizamos la captura de los datos.
	 	 */	 
		for (i = 0; i < 85; i++) {
			contador = 0;	
			while (digitalRead(21) == estado) {
				contador++;
				delayMicroseconds(1);
				if (contador == 255) {
					break;
				}
			}			
			if ((i >= 4) && (i % 2 == 0)) {
				cadena[j / 8] <<= 1;
				if ( contador > 16 )
					cadena[j / 8] |= 1;
				j++;
			}
			estado = digitalRead(21);
		}
	 
		/**
	 	 * Si la información se ha recibido correctamente, se muestran los datos para su captura
	 	 * separados por un guíón.
	 	 */
		if ((j >= 40) && (cadena[4] == ((cadena[0]+cadena[1]+cadena[2]+cadena[3])&0xFF))) {
			printf("%d.%d-%d.%d\n",cadena[0],cadena[1],cadena[2],cadena[3]);
		}
		
		/**
	 	 * Si la información NO se ha recibido correctamente, se muestra un código de error para
	 	 * identificarlo posteriormente.
	 	 */
		else  {
			printf("1701\n");
		}	
	}
	return(0);
}
