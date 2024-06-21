#include <Arduino.h>

#define DISTANCE 2.032
#define SAMPLES 3
#define DISPLAYS 5

#define SENSOR_PIN 2
#define BUTTON_PIN 3


unsigned char displayPins[2][7] = {
	{  5,  4,  6,  7,  8,  9, 10 },
	{ 11, 12, A0, A3, A1, A2, A4 },
};

unsigned char digits[] = {
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

unsigned long then = 0;
unsigned long now = 0;
unsigned long duration = 0;
float instSpeed = 0;
float sumSpeed = 0;
unsigned char avgSpeed = 0;
unsigned char counter = 0;
unsigned long lastDisplayTime = 0;


bool displayed = 1;

void clear()
{
	for (unsigned char i = 0; i < 7; ++i)
	{
		digitalWrite(displayPins[0][i], 0);
		digitalWrite(displayPins[1][i], 0);
	}
}

void show(unsigned char number)
{
	unsigned char digit0 = number % 10;
	unsigned char digit1 = number / 10;
	for (unsigned char i = 0; i < 7; ++i)
	{
		digitalWrite(displayPins[0][i], digits[digit0] >> (6 - i) & 1);
		digitalWrite(displayPins[1][i], digits[digit1] >> (6 - i) & 1);
	}
}


void handler()
{
	now = millis();
	instSpeed = DISTANCE * 3600 / (now - then);
	sumSpeed += instSpeed;
	if (++counter > SAMPLES)
	{
		avgSpeed = sumSpeed / SAMPLES;
		counter = sumSpeed = 0;

		if (now - lastDisplayTime > 30000)
		{
			clear();
			displayed = 0;
			lastDisplayTime = now;
		}
		else
			if (displayed)
				show(avgSpeed);

	}
	then = now;
}

void setup()
{
	pinMode(SENSOR_PIN, INPUT_PULLUP);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	for (unsigned char i = 0; i < 7; ++i)
	{
		pinMode(displayPins[0][i], OUTPUT);
		pinMode(displayPins[1][i], OUTPUT);
	}

	attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), handler, FALLING);


}

void loop()
{
	if (!digitalRead(BUTTON_PIN))
	{
		displayed = 1;
		delay(200);
	}
	delay(50);
}