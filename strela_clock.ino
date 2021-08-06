#include <inttypes.h>

#define POS_SIG	2
#define NEG_SIG	3

#define ledPin 13

void setupTimer1() {
	noInterrupts();
	// Clear registers
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	// 1 Hz (16000000/((15624+1)*1024))
	OCR1A = 15624;
	// CTC
	TCCR1B |= (1 << WGM12);
	// Prescaler 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// Output Compare Match A Interrupt Enable
	TIMSK1 |= (1 << OCIE1A);
	interrupts();
}

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(POS_SIG, OUTPUT);
	pinMode(NEG_SIG, OUTPUT);
	digitalWrite(POS_SIG, HIGH);
	digitalWrite(NEG_SIG, HIGH);
	setupTimer1();
}

void loop() {
}

ISR(TIMER1_COMPA_vect) {
	digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
	static uint8_t seconds = 0;
	static uint8_t pos_neg = 0;
	static uint8_t sending = 0;
	if (sending) {
		sending = 0;
		digitalWrite(POS_SIG, HIGH);
		digitalWrite(NEG_SIG, HIGH);
	}
	seconds++;
	if (seconds == 60) {
		seconds = 0;
		sending++;
		if (pos_neg) {
			digitalWrite(POS_SIG, LOW);
			pos_neg = 0;
		} else {
			digitalWrite(NEG_SIG, LOW);
			pos_neg = 1;
		}
	}
}
