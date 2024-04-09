/*
 prelab4
 UNIVERSIDAD DEL VALLE DE GUATEMALA
 PROGRAMACIÓN DE MICROCONTROLADORES
 PRE-LABORATORIO 4
 AUTOR: nATHALIE fAJARDO/LEFAJ
 CREADO: 8/4/2024 22:23:24 PM
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int contador=0;
int pulsador=0;
uint8_t valor_PC=0;

//interrupcion de botones
ISR(PCINT1_vect){
	//revisa si es la primera o segunda interrupción
	valor_PC=PINC;
	if (pulsador==0){
		if (valor_PC==1){
			contador++;
			pulsador++;
		}else if (valor_PC==2){
			contador--;
			pulsador++;
		}
	}else{
		pulsador=0:
	}
}

void setup(void){
	cli();
	DDRD=0xF0;
	PORTD |= 0cFC;
	PCICR=0;
	PCICR |=(1<<PCIE1)
	PCMSK1|=(1<<PCINT8)
	PCMSK1|=(1<<PCINT9)
	sei();
}

int main(void)
{	//muestra el valor del contador
	PORTD=contador;
	delay(100);
}




