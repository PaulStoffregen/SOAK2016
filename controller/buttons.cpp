#include <Arduino.h>
#include "soak.h"

static int32_t state[8];
static int32_t press[8];
static int32_t release[8];

// Returns true if this button was pressed during the most recent update
int button_press(unsigned int num)
{
	if (num < 256 && (press[num >> 5] & (1 << (num & 31)))) return 1;
	return 0;
}

// Returns true if this button was released during the most recent update
int button_release(unsigned int num)
{
	if (num < 256 && (release[num >> 5] & (1 << (num & 31)))) return 1;
	return 0;
}

// Returns true if the button is being held down
int button_down(unsigned int num)
{
	if (num < 256 && (state[num >> 5] & (1 << (num & 31)))) return 1;
	return 0;
}

// Update info about all buttons.  Causes the above functions to know button info.
void buttons_update()
{
	static int incoming=0;
	static elapsedMillis timeout;

	memset(press, 0, sizeof(state));
	memset(release, 0, sizeof(release));

	while (Serial1.available()) {
		timeout = 0;
		char c = Serial1.read();
		if (c >= '0' && c <= '9') {
			incoming *= 10;
			incoming += c - '0';
			continue;
		} else if (c == 'B') {
			// button press
			if (incoming < 256) {
				press[incoming >> 5] |= (1 << (incoming & 31));
				state[incoming >> 5] |= (1 << (incoming & 31));
			}
			Serial.print("Button ");
			Serial.print(incoming);
			Serial.println();
		} else if (c == 'b') {
			// button release
			if (incoming < 256) {
				release[incoming >> 5] |= (1 << (incoming & 31));
				state[incoming >> 5] &= ~(1 << (incoming & 31));
			}
		} else if (c == 'A' || c == 'a') {
			// analog reading (pot)
			//Serial.print(incoming);
			//Serial.println(c);
		} else {
			// unknown message type
		} 
		incoming = 0;
	}
}

