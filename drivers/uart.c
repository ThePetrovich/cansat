/*
 * uart.c
 *
 * Created: 07.03.2019 20:22:02
 *  Author: ThePetrovich
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"

volatile char rx0_buffer[32] = "";
volatile char tx0_buffer[128] = "";
volatile int rx0_pointer = 0;
volatile char *tx0_data;
volatile int tx0_pointer = 0;
volatile int tx0_size = 0;

volatile unsigned char creg0 = 0;
volatile unsigned char creg1 = 0;
	
volatile char rx1_buffer[128] = "";
volatile char tx1_buffer[128] = "";
volatile int rx1_pointer = 0;
volatile char *tx1_data;
volatile int tx1_pointer = 0;
volatile int tx1_size = 0;

int uart0_init(unsigned int ubrr){
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0B = (RX0EN<<RXEN)|(TX0EN<<TXEN)|(RX0IE<<RXCIE)|(TX0IE<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

void uart0_transmit(){
	UDR0 = tx0_buffer[0];
	UCSR0B |= (1<<UDRIE);
	creg0 |= (1<<TX0BUSY);
}

void rx0_buffer_flush(){
	rx0_buffer[0] = 0;
	rx0_pointer = 0;
}

void tx0_buffer_flush(){
	tx0_pointer = 0;
	tx0_buffer[0] = '\x0';
}

ISR(USART0_RX_vect){
	char data = UDR0;
	if(strlen((char*)&rx0_buffer) < 32){
		rx0_buffer[rx0_pointer] = data;
		rx0_pointer++;
	}
}

ISR(USART0_UDRE_vect){
	tx0_pointer+=1;
	if(tx0_buffer[tx0_pointer] != '\x0'){
		UDR0 = tx0_buffer[tx0_pointer];
	}
	else {
		UCSR0B &= ~(1<<UDRIE);
		creg0 &= ~(1<<TX0BUSY);
		tx0_buffer_flush();
	}
}


int uart1_init(unsigned int ubrr){
	UBRR1H = 0;
	UBRR1L = 51;
	UCSR1B = (RX1EN<<RXEN)|(TX1EN<<TXEN)|(RX1IE<<RXCIE)|(TX1IE<<TXCIE)|(0<<UDRIE);
	UCSR1C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

void uart1_transmit(){
	UDR1 = tx1_buffer[0];
	UCSR1B |= (1<<UDRIE);
	creg1 |= (1<<TX1BUSY);
}

void rx1_buffer_flush(){
	rx1_buffer[0] = 0;
	rx1_pointer = 0;
}

void tx1_buffer_flush(){
	tx1_pointer = 0;
	tx1_buffer[0] = '\x0';
}

ISR(USART1_RX_vect){
	char data = UDR1;
	if(strlen((char*)&rx1_buffer) < 32){
		rx1_buffer[rx1_pointer] = data;
		rx1_pointer++;
	}
}

ISR(USART1_UDRE_vect){
	tx1_pointer+=1;
	if(tx1_buffer[tx1_pointer] != '\x0'){
		UDR1 = tx1_buffer[tx1_pointer];
	}
	else {
		UCSR1B &= ~(1<<UDRIE);
		creg1 &= ~(1<<TX1BUSY);
		tx1_buffer_flush();
	}
}