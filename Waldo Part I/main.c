/********************************************/
/* Name: main.c
* Author: <Ikenna Achilihu>
* Copyright: <insert your copyright message here>
* License: <insert your license reference here>
*/
#include "teensy_general.h" // includes the resources included in the teensy_general.h file
#include "t_usb.h"

//Subroutine declaration
void adc_setup(int);
void adc_read1(void);
void adc_read2(void);

int main(void){
	
	teensy_clockdivide(0); //set the clock speed
	teensy_led(ON); // turn on the on board LED

	//Initialize USB communications
	m_usb_init();

	//Declare and initialize variables
	int ADCchannel;
	

	//1/1 timer prescalar
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);

	//Set PWM Mode 5
	set(TCCR1B, WGM12);
	clear(TCCR1A, WGM11);
	set(TCCR1A, WGM10);
	clear(TCCR1B, WGM13);

	/*ADC presetup*/

	
	/**************/
	

	//Infinite loop
	while(1){

		//Select ADC channel pin F4 
		ADCchannel = 4;

		//Call ADC setup function
		adc_setup(ADCchannel);

		adc_read1();

		//Select ADC channel pin F0 
		ADCchannel = 0;

		//Call ADC setup function
		adc_setup(ADCchannel);
		
		adc_read2();
	}
	//Return upon successful completion
	return 0; /* never reached */
}

//ADC subroutines
void adc_setup(int ADCchannel){
	//ADC channel 0 (Pin F0)
	if (ADCchannel == 0)
	{
		//Disable digital input
		set(DIDR0, ADC0D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 1 (Pin F1)
	if (ADCchannel == 1)
	{
		//Disable digital input
		set(DIDR0, ADC1D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 4 (Pin F4)
	if (ADCchannel == 4)
	{
		//Disable digital input
		set(DIDR0, ADC4D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 5 (Pin F5)
	if (ADCchannel == 5)
	{
		//Disable digital input
		set(DIDR0, ADC5D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 6 (Pin F6)
	if (ADCchannel == 6)
	{
		//Disable digital input
		set(DIDR0, ADC6D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 7 (Pin F7)
	if (ADCchannel == 7)
	{
		//Disable digital input
		set(DIDR0, ADC7D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 8 (Pin D4)
	if (ADCchannel == 8)
	{
		//Disable digital input
		set(DIDR2, ADC8D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 9 (Pin D6)
	if (ADCchannel == 9)
	{
		//Disable digital input
		set(DIDR2, ADC9D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 10 (Pin D7)
	if (ADCchannel == 10)
	{
		//Disable digital input
		set(DIDR2, ADC10D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 11 (Pin B4)
	if (ADCchannel == 11)
	{
		//Disable digital input
		set(DIDR2, ADC11D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 12 (Pin B5)
	if (ADCchannel == 12)
	{
		//Disable digital input
		set(DIDR2, ADC12D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 13 (Pin B6)
	if (ADCchannel == 13)
	{
		//Disable digital input
		set(DIDR2, ADC13D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
}

void adc_read1(void){

	//Set voltage reference to VCC
	set(ADMUX, REFS0);
	clear(ADMUX, REFS1);

	//Set ADC Prescaler to 1/128
	set(ADCSRA, ADPS2);
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);

	//Declare and intialize variable
	unsigned int adcAngle;

	//Continuous conversion
	set(ADCSRA, ADATE);

	//Enable ADC subsystem 
	set(ADCSRA, ADEN);

	//Begin ADC conversion 
	set(ADCSRA, ADSC);

	//Calculate pot angle in degrees
	adcAngle = 0.264 * ADC;

	//Print to terminal
	m_usb_tx_string("Sensor 2 reads: ");
	m_usb_tx_uint(adcAngle);
	m_usb_tx_string(" degrees");
	m_usb_tx_string("\n");
	teensy_wait(1000);

	//*10/1024*10 for ADC on 0-100% scale
}

void adc_read2(void){

	//Set voltage reference to VCC
	set(ADMUX, REFS0);
	clear(ADMUX, REFS1);

	//Set ADC Prescaler to 1/128
	set(ADCSRA, ADPS2);
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);

	//Declare and intialize variable
	unsigned int adcAngle;

	//Continuous conversion
	set(ADCSRA, ADATE);

	//Enable ADC subsystem 
	set(ADCSRA, ADEN);

	//Begin ADC conversion 
	set(ADCSRA, ADSC);

	//Calculate pot angle in degrees
	adcAngle = 0.264 * ADC;

	//Calculate angle in degrees

	//Print to terminal
	m_usb_tx_string("Sensor 1 reads: ");
	m_usb_tx_uint(adcAngle);
	m_usb_tx_string(" degrees");
	m_usb_tx_string("\n");
	teensy_wait(1000);

	//*10/1024*10 for ADC on 0-100% scale
}
/********************************************/