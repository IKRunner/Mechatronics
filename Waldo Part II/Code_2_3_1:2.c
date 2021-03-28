/********************************************/
/* Name: main.c
* Author: <Ikenna Achilihu>
* Copyright: <insert your copyright message here>
* License: <insert your license reference here>
*/
#include "teensy_general.h" // includes the resources included in the teensy_general.h file
#include "t_usb.h"
#define DUTY_CYCLE 0x03FF	//100% Duty cycle for enable pin
#define P_GAIN 10			//Proportional gain 	
#define I_GAIN 0			//Integral gain  		
#define D_GAIN 0.0001			//Derivative gain 	
#define SENSOR_ONE 4
#define MOTOR_ONE 1
#define SENSOR_TWO 5

//Subroutine declaration
void adc_setup(int);
int sensor_read (void);
int motorOneError(int, int);
void driveMotorOne(int);
int motorTwoError(int, int);
void driveMotorTwo(int);

//Feedback terms
int iTerm, dTerm, pTerm, lastErrorOne;

int main(void) {	
	teensy_clockdivide(0); //set the clock speed
	teensy_led(ON); // turn on the on board LED

	//Initialize USB communications
	m_usb_init();

	//Declare and initialize variables
	int ADCchannel, sensorOneAngle, errorOne, motorOneAngle, sensorTwoAngle;

	//Initialize PID terms
	iTerm = 0;
	lastErrorOne = 0;

	//Select pin D0, D1, D2, D3, B5 as output
	set(DDRD,0); 		//High for forward
	set(DDRD, 1); 		//High for reverse
	set(DDRD,2); 		//High for forward
	set(DDRD, 3); 		//High for reverse
	set(DDRB, 5); 		//Enabled pin

	//1/1 timer prescalar
	set(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);

	//Set Mode 7 for Timer 1
	clear(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);

	//Pin B5: Clear signal at output compare, set at rollover
	set(TCCR1A, COM1A1);
	clear(TCCR1A, COM1A0);

	//Infinite loop
	while(1) {
		//Select ADC channel to read sensor one
		ADCchannel = SENSOR_ONE;

		//Call ADC setup function
		adc_setup(ADCchannel);

		//Read sensor one data
		sensorOneAngle = sensor_read();

		//Select ADC channel to read sensor two
		ADCchannel = SENSOR_TWO;

		//Call ADC setup function
		adc_setup(ADCchannel);

		//Read sensor two data
		sensorTwoAngle = sensor_read();

		//Read motor position and select ADC channel
		ADCchannel = MOTOR_ONE;
		adc_setup(ADCchannel);
		motorOneAngle = sensor_read();

		//Calculate error
		errorOne = motorOneError(sensorOneAngle, motorOneAngle);

		//Drive motor one
		driveMotorOne(errorOne);

		//Write error to derivative term
		lastErrorOne = errorOne;

		//Drive motor two
		driveMotorTwo(sensorTwoAngle);
	}
	//Return upon successful completion
	return 0; /* never reached */
}

//ADC subroutines
void adc_setup(int ADCchannel) {
	//ADC channel 0 (Pin F0)
	if (ADCchannel == 0) {
		//Disable digital input
		set(DIDR0, ADC0D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 1 (Pin F1)
	if (ADCchannel == 1) {
		//Disable digital input
		set(DIDR0, ADC1D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 4 (Pin F4)
	if (ADCchannel == 4) {
		//Disable digital input
		set(DIDR0, ADC4D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 5 (Pin F5)
	if (ADCchannel == 5) {
		//Disable digital input
		set(DIDR0, ADC5D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 6 (Pin F6)
	if (ADCchannel == 6) {
		//Disable digital input
		set(DIDR0, ADC6D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 7 (Pin F7)
	if (ADCchannel == 7) {
		//Disable digital input
		set(DIDR0, ADC7D);

		//Set ADC input 
		clear(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 8 (Pin D4)
	if (ADCchannel == 8) {
		//Disable digital input
		set(DIDR2, ADC8D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 9 (Pin D6)
	if (ADCchannel == 9) {
		//Disable digital input
		set(DIDR2, ADC9D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 10 (Pin D7)
	if (ADCchannel == 10) {
		//Disable digital input
		set(DIDR2, ADC10D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 11 (Pin B4)
	if (ADCchannel == 11) {
		//Disable digital input
		set(DIDR2, ADC11D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}

	//ADC channel 12 (Pin B5)
	if (ADCchannel == 12) {
		//Disable digital input
		set(DIDR2, ADC12D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}

	//ADC channel 13 (Pin B6)
	if (ADCchannel == 13) {
		//Disable digital input
		set(DIDR2, ADC13D);

		//Set ADC input 
		set(ADCSRB, MUX5);
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
}

int sensor_read(void) {
	//Set voltage reference to VCC
	set(ADMUX, REFS0);
	clear(ADMUX, REFS1);

	//Set ADC Prescaler to 1/128
	set(ADCSRA, ADPS2);
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);

	//Declare and intialize variable
	unsigned int adcFuncAngle;

	//Enable ADC subsystem 
	set(ADCSRA, ADEN);

	//Begin ADC conversion 
	set(ADCSRA, ADSC);

	//Wait for conversion to complete
	while((bit_is_clear(ADCSRA, ADIF)));

	//Calculate pot angle in degrees
	adcFuncAngle = 0.264 * ADC;

	//Return current sensor angle
	return adcFuncAngle;
}

int motorOneError(int sensorAngle, int motorAngle) {
	// if ((sensorAngle - motorAngle) < 0){
	// 	// m_usb_tx_string("NEGATIVE ERROR!!!");
	// 	// m_usb_tx_string("\n");
	// 	// teensy_wait(400);

	// }

	//Return error
	return (sensorAngle - motorAngle);
}

void driveMotorOne(int error) {

	//Compute proportional, integral, derivative terms
	unsigned int PWM = (error * P_GAIN) + ((iTerm += error) * I_GAIN) + 
			((error - lastErrorOne) * D_GAIN);

     // m_usb_tx_string("Duty cycle is: ");
     // m_usb_tx_uint(PWM);
     // m_usb_tx_string("\n");

	//Set upper limit to PWM
	if (PWM > 0x03FF) { OCR1A = 0x03FF; }

	//Set lower limit to PWM
	if (PWM < 0) { OCR1A = 0; }

	//Write PWM to OCR1A if within bounds
	if (PWM > 0 && PWM < 0x03FF) { OCR1A = PWM; }

	//Positive error
	if (error > 0){
		//Spin motor clockwise
		set(PORTD, 0); 
		clear(PORTD, 1);
	}

	//Negative error
	if(error < 0){
		//Spin motor counter clockwise
		set(PORTD, 1); 
		clear(PORTD, 0);
	}

	//Zero error
	if (error == 0) {
		//Turn off motor
		clear(PORTD, 0); 
		clear(PORTD, 1);
	}
}

void driveMotorTwo(int sensorAngle){

     // m_usb_tx_string("OCR1A is: ");
     // m_usb_tx_uint(OCR1A);
     // m_usb_tx_string("\n");

     //Limit motor speed
     OCR1A = 0x64;

     //Spin motor clockwise 
     if (sensorAngle > 230) {
		set(PORTD, 2); 
		clear(PORTD, 3);
     }

     //Spin motor counter clockwise
     else if (sensorAngle < 200) {
		set(PORTD, 3); 
		clear(PORTD, 2);
     }

     //Turn motor off
     else {
     	clear(PORTD, 2); 
		clear(PORTD, 3);
     }
 }


/********************************************/
