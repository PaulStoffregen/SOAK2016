#include <Bounce.h>

#define MINSPEED 150
#define RATELIMIT 4500

Bounce button(15, 20);

int speed=0;
int target_speed=0;
int command_speed=0;
elapsedMillis manualtimer=0;
elapsedMillis commandtimer=0;
elapsedMicros changetimer=0;
bool manual_mode=false;


unsigned int incoming=0;
elapsedMillis incomingtimeout=0;

void setup()
{
	pinMode(13, OUTPUT);       // orange LED
	pinMode(15, INPUT_PULLUP); // pushbutton
	pinMode(16, OUTPUT);       // red LED
	analogWriteResolution(10);
	analogWriteFrequency(23, 23437.5);  // motor control
	analogWrite(23, 0);
	digitalWrite(13, LOW);
	digitalWrite(16, LOW);
	Serial1.begin(9600);
}

void loop()
{
	if (Serial1.available()) {
		incomingtimeout = 0;
		char c = Serial1.read();
		if (c >= '0' && c <= '9') {
			incoming *= 10;
			incoming += c - '0';
		} else if (c == 'V') {
			if (incoming == 0 || incoming >= MINSPEED || incoming <= 1023) {
				command_speed = incoming;
				if (!manual_mode) target_speed = incoming;
				commandtimer = 0;
			}
			incoming = 0;
		} else {
			incoming = 0;
		}
	}
	if (incomingtimeout > 250) {
		incoming = 0;
		incomingtimeout = 0;
	}
	if (commandtimer > 20000) {
		command_speed = 0;
		if (!manual_mode) target_speed = 0;
	}
	button.update();
	if (button.fallingEdge()) {
		manual_mode = true;
		manualtimer = 0;
	}
	if (manual_mode) {
		if (manualtimer > 20000) {
			manual_mode = false;
			target_speed = command_speed;
			//Serial.printf("target_speed = %d\n", target_speed);
		} else {
			// target_speed range = 338 to 1023
			float f = (float)analogRead(14) / 1023.0f;
			f *= (float)(1023 - MINSPEED);
			target_speed = (int)f + MINSPEED;
			//Serial.printf("target_speed = %d\n", target_speed);
		}
	}
	if (target_speed < MINSPEED) {
		target_speed = 0;
	} else if (target_speed > 1023) {
		target_speed = 1023;
	}

	if (speed != target_speed && changetimer > RATELIMIT) {
		changetimer -= RATELIMIT;
		if (target_speed == 0) {
			if (speed > MINSPEED) {
				digitalWrite(16, HIGH);
				speed--;
			} else {
				digitalWrite(16, LOW);
				speed = 0;
			}
		} else if (speed == 0) {
			digitalWrite(16, HIGH);
			speed = MINSPEED;
		} else if (speed < target_speed) {
			digitalWrite(16, HIGH);
			speed++;
		} else if (speed > target_speed) {
			digitalWrite(16, HIGH);
			speed--;
		}
		analogWrite(23, speed);
		//Serial.printf("speed = %d\n", speed);
	}
	asm volatile (" wfi");
}


