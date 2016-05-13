
#include <Bounce.h>

void setup()
{
	for (int i=0; i < 46; i++) {
		// all pins outputs, except Serial1
		if (i == 2 || i == 3) continue;
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
	Serial1.begin(115200);
}

void transmit(char cmd, unsigned int num)
{
	//Serial.print(num);
	//Serial.println(cmd);
	Serial1.print(num);
	Serial1.print(cmd);
}

unsigned int incoming=0;
elapsedMillis timeout;
elapsedMillis blinktime;
uint8_t ledstate=0;

const uint8_t outputpins[32] = {
	27,  0,  1,  4,  5,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
	45, 44, 43, 42, 41, 40, 39, 38, 19, 20, 21, 22, 23, 24, 25, 26};

void input(char c)
{
	if (c >= '0' && c <= '9') {
		incoming *= 10;
		incoming += c - '0';
	} else if (c == 'S') {
		if (incoming < 32) {
			digitalWrite(outputpins[incoming], HIGH);
		} else {
			transmit(c, incoming - 32);
		}
	} else if (c == 's') {
		if (incoming < 32) {
			digitalWrite(outputpins[incoming], LOW);
		} else {
			transmit(c, incoming - 32);
		}
	} else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
		transmit(c, incoming);
		incoming = 0;
	}
	timeout = 0;
}

void loop()
{
	if (Serial1.available()) {
		input(Serial1.read());
	}
	if (Serial.available()) {
		input(Serial.read());
	}
	if (timeout > 250) {
		incoming = 0;
		timeout = 0;
	}
	if (blinktime > 500) {
		blinktime -= 500;
		ledstate = (ledstate ? LOW : HIGH);
		digitalWrite(6, ledstate);
	}
}

