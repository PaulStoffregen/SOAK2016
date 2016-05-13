#include <Arduino.h>
#include "soak.h"

void buttons_update()
{
	static int incoming=0;
	static elapsedMillis timeout;

	while (Serial1.available()) {
		timeout = 0;
		char c = Serial1.read();
		if (c >= '0' && c <= '9') {
			incoming *= 10;
			incoming += c - '0';
			continue;
		} else if (c == 'B') {
			// button press
			Serial.print("Button ");
			Serial.print(incoming);
			Serial.println();
		} else if (c == 'b') {
			// button release

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

