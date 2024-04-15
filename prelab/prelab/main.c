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

uint8_t contador=0;
uint8_t pulsador=0;
uint8_t valor_PC=0;
uint8_t valor_ADC=0;
uint8_t valor_disp1=0;
uint8_t valor_disp2=0;
uint8_t valor_PORTC=0;
const uint8_t display7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

//interrupcion de botones
ISR(PCINT1_vect){
	//revisa si es la primera o segunda interrupción
	valor_PC=PINC;
	if (pulsador==0){
		if (valor_PC&(1<<1)){
			contador++;
			if (contador==256){
				contador=0;
			}
			pulsador++;
		}else if (valor_PC&(1<<0)){
			contador--;
			if(contador<0){
				contador=255;
			}
			pulsador++;
		}
	}else{
		pulsador=0;
	}
}



ISR(ADC_vect){
	valor_ADC=ADCH;
	valor_disp1=(valor_ADC&0x0F);
	valor_disp2=(valor_ADC>>4);
	ADCSRA&=~(1<<ADIF);
}

void setup(void){
	cli();
	DDRD=0b11111111;
	DDRB=0b01111111;
	DDRC=0b00011100;
	PCICR=0;
	PCICR |=(1<<PCIE1);
	PCMSK1|=(1<<PCINT8);
	PCMSK1|=(1<<PCINT9);
	UCSR0B=0;
	sei();
}

void init_adc(void){
	ADMUX = 0;
	ADCSRA=0;
	
	// VCC=5V
	ADMUX |=(1<<REFS0);
	
	// Entrada 6 de MUX
	ADMUX |=(1<<MUX1)|(1<<MUX2)|(1<<ADLAR);
	
	//Encender ADC y  habilitar isr adc
	ADCSRA=0;
	ADCSRA|=(1<<ADEN)|(1<<ADIE);
	
	// preescaler = 16M/128=125k
	ADCSRA|=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	
	//no se utilizara el estado B
	ADCSRB=0;
	//DIDRO=0;
}

int main(void){
	init_adc();
	setup();
	//muestra el valor del contador
	while(1){
		//iniciar primera secuencia del adc
		ADCSRA|=(1<<ADSC);
		_delay_ms(4);
		
		PORTC&=~(1<<PINC3);
		PORTC&=~(1<<PINC4);
		
		//primer display
		PORTC|=(1<<PINC3);
		PORTB=display7[valor_disp1];
		//PORTB=0x06;
		if (PORTB&(1<<6)){
			PORTC|=(1<<PINC2);
		}else{
			PORTC&=~(1<<PINC2);
		}
		_delay_ms(4);
		PORTC&=~(1<<PINC3);
		PORTC|=(1<<PINC4);
		//PORTB=0x7C;
		PORTB=display7[valor_disp2];
		if (PORTB&(1<<6)){
			PORTC|=(1<<PINC2);
			}else{
			PORTC&=~(1<<PINC2);
		}
		_delay_ms(4);
		
		//contador de bits
		PORTD=contador;
		if (valor_ADC>=contador){
			PORTC|=(1<<PINC5);
		}else{
			PORTC&=~(1<<PINC5);
		}
	}
}




