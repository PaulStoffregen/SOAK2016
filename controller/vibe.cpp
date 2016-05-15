#include <Arduino.h>
#include "soak.h"

static unsigned int level=0;
static unsigned int update_needed=0;
static elapsedMillis offtimer;
static unsigned int offtime;

void vibe_update()
{
	if (level > 0 && offtimer > offtime) {
		level = 0;
		update_needed = 1;
	}
	if (update_needed) {
		if (Serial3.availableForWrite() >= 5) {
			Serial3.print(level);
			Serial3.write('V');
			update_needed = 0;
		}
	}
}

void vibe_on(unsigned int amount, unsigned int milliseconds)
{
	if (amount < 150) {
		amount = 150;
	} else if (amount > 1023) {
		amount = 1023;
	}
	if (milliseconds > 20000) {
		milliseconds = 20000; // 20 second maximum
	}
	if (level != amount) {
		level = amount;
		update_needed = 1;
	}
	offtimer = 0;
	offtime = milliseconds;
}

void vibe_off()
{
	if (level > 0) {
		level = 0;
		if (Serial3.availableForWrite() > 2) {
			Serial3.write('0');
			Serial3.write('V');
		} else {
			update_needed = 1;
		}
	}
}
