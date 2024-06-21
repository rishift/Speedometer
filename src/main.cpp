#include <Arduino.h>

#define DISTANCE 2.032
#define SAMPLES 3

#define SENSOR_PIN 2
#define BUTTON_PIN 3

#define DEBOUNCE_DELAY 50

char displayPins[2][7] = {
	{5, 4, 6, 7, 8, 9, 10},
	{11, 12, A0, A3, A1, A2, A4}};

short digits[] = {
	0b1111110,
	0b0110000,
	0b1101101,
	0b1111001,
	0b0110011,
	0b1011011,
	0b1011111,
	0b1110000,
	0b1111111,
	0b1111011,
};

long then = 0;
long now = 0;
long duration = 0;
float instSpeed = 0;
float sumSpeed = 0;
char avgSpeed = 0;
char counter = 0;

bool displayed = 1;

void clear()
{
	for (char i = 0; i < 7; ++i)
	{
		digitalWrite(displayPins[0][i], 0);
		digitalWrite(displayPins[1][i], 0);
	}
}

void showDigit(char digit, bool display)
{
	for (char i = 0; i < 7; ++i)
		digitalWrite(displayPins[display][i], digits[digit] >> (6 - i) & 1);
}

void show(char number)
{
	showDigit(number / 10, 1);
	showDigit(number % 10, 0);
}

void handler()
{
	now = millis();
	instSpeed = DISTANCE * 3600 / (now - then);
	sumSpeed += instSpeed;
	if (++counter > SAMPLES)
	{
		avgSpeed = sumSpeed / SAMPLES;
		counter = 0;
		sumSpeed = 0;
		// if (displayed)
		// 	show(avgSpeed);
	}
	then = now;
}

void handler2() {
	Serial.println("sw");
}

void setup()
{
	pinMode(SENSOR_PIN, INPUT_PULLUP);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	for (char i = 0; i < 7; ++i)
	{
		pinMode(displayPins[0][i], OUTPUT);
		pinMode(displayPins[1][i], OUTPUT);
	}

	attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), handler, FALLING);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handler2, CHANGE);

	Serial.begin(9600);
}

void loop()
{
	// Serial.println(digitalRead(BUTTON_PIN));
	// if (!digitalRead(BUTTON_PIN))
	// {
	// 	Serial.println(displayed);
	// 	if (!displayed)
	// 	{
	// 		clear();
	// 		Serial.println("clear");
	// 	};
	// 	displayed = !displayed;
	// 	delay(100);
	// }
	// Serial.println(digitalRead(BUTTON_PIN));
}