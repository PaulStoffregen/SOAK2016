#include <Arduino.h>
#include "soak.h"

static unsigned char state[96];  // 0=off, 1=on, 2=blink off, 3=blink on
static unsigned int blink_msec[96];
static unsigned int blink_prev[96];

static const int lightcodes[200] = {
//   1   2   3   4   5   6   7   8   9
-1, -1, -1, -1, 27, 24, 25, 26, -1,  4,  //   0-9
 0,  3,  2,  1,  5, 28, 29, 31, 30,  6,  //  10-19
 7,  8,  9, -1, -1, -1, -1, -1, -1, -1,  //  20-29
-1, -1, -1, 19, -1, -1, -1, -1, 17, -1,  //  30-39
16, -1, -1, 91, -1, -1, 18, -1, -1, -1,  //  40-49
-1, -1, -1, -1, -1, -1, -1, -1, 63, 61,  //  50-59
-1, -1, 60, -1, -1, -1, 92, 94, 13, 10,  //  60-69
93, 95, 12, 11, -1, -1, -1, -1, -1, -1,  //  70-79
-1, -1, 70, 69, 68, 67, 66, 65, 64, 90,  //  80-89
84, -1, 85, 87, 88, 89, -1, 51, 50, 49,  //  90-99
48, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 100-109
-1, -1, -1, -1, -1, 52, 53, 54, 55, 56,  // 110-119
57, 58, 59, -1, -1, -1, -1, -1, 81, 82,  // 120-129
83, -1, -1, -1, 74, 73, 72, 71, 77, 78,  // 130-139
79, 43, 44, 45, 46, 47, -1, -1, 42, 41,  // 140-149
-1, -1, 40, 39, -1, 37, 76, 75, -1, -1,  // 150-159
35, 34, 33, 32, -1, -1, -1, -1, -1, -1,  // 160-169
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 170-179
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 180-189
-1, -1, -1, -1, -1, -1, -1, -1, -1, 14,  // 190-199
// dead: 15 21 22 23 36 38 62 80 86
};

// turn a light on
void light_on(unsigned int num)
{
	if (num > 199) return;
	int lcode = lightcodes[num];
	if (lcode < 0) return;
	state[lcode] = 1;
	Serial1.print(lcode);
	Serial1.print('S');
}

// turn a light off
void light_off(unsigned int num)
{
	if (num > 199) return;
	int lcode = lightcodes[num];
	if (lcode < 0) return;
	state[lcode] = 0;
	Serial1.print(lcode);
	Serial1.print('s');
}

// toggle a light on/off (on if it was off, or off it is was on)
int light_toggle(unsigned int num)
{
	if (num > 199) return 0;
	int lcode = lightcodes[num];
	if (lcode < 0) return 0;
	if (state[lcode] == 0) {
		state[lcode] = 1;
		Serial1.print(lcode);
		Serial1.print('S');
		return 1;
	} else {
		state[lcode] = 0;
		Serial1.print(lcode);
		Serial1.print('s');
		return 0;
	}
}

// cause a light to blink
void light_blink(unsigned int num, unsigned int milliseconds)
{
	if (num > 199) return;
	int lcode = lightcodes[num];
	if (lcode < 0) return;
	if (milliseconds < 50) {
		milliseconds = 50; // fastest = 10 blinks/sec
	} else if (milliseconds > 10000) {
		milliseconds = 10000; // slowest = once every 20 seconds
	}
	Serial1.print(lcode);
	Serial1.print('S');
	blink_msec[lcode] = milliseconds;
	blink_prev[lcode] = millis();
	state[lcode] = 3;
}

// cause a light to blink if it was off, or if on/blinking, turn it off
int light_blink_toggle(unsigned int num, unsigned int milliseconds)
{
	if (num > 199) return 0;
	int lcode = lightcodes[num];
	if (lcode < 0) return 0;
	if (state[lcode] == 0) {
		// led was off, so go into blinking mode
		if (milliseconds < 50) {
			milliseconds = 50; // fastest = 10 blinks/sec
		} else if (milliseconds > 10000) {
			milliseconds = 10000; // slowest = once every 20 seconds
		}
		Serial1.print(lcode);
		Serial1.print('S');
		blink_msec[lcode] = milliseconds;
		blink_prev[lcode] = millis();
		state[lcode] = 3;
		return 1;
	} else {
		// led was on or blinking, so turn it off
		Serial1.print(lcode);
		Serial1.print('s');
		state[lcode] = 0;
		return 0;
	}
}

// returns true if the light is on or blinking (even if momentarily off)
int light_is_on(unsigned int num)
{
	if (num > 199) return 0;
	int lcode = lightcodes[num];
	if (lcode < 0) return 0;
	if (state[lcode] == 0) return 0;
	return 1;
}

// returns true if the light is blinking, but not if it's on solid
int light_is_blinking(unsigned int num)
{
	if (num > 199) return 0;
	int lcode = lightcodes[num];
	if (lcode < 0) return 0;
	if (state[lcode] == 0 || state[lcode] == 1) return 0;
	return 1;
}

// needs to be called every loop(), to update the blinking lights
void lights_update()
{
	unsigned int tnow = millis();

	for (int i=0; i < 96; i++) {
		if (state[i] == 2) {
			if (tnow - blink_prev[i] > blink_msec[i]) {
				Serial1.print(i);
				Serial1.print('S');
				state[i] = 3;
				blink_prev[i] = tnow;
			}
		} else if (state[i] == 3) {
			if (tnow - blink_prev[i] > blink_msec[i]) {
				Serial1.print(i);
				Serial1.print('s');
				state[i] = 2;
				blink_prev[i] = tnow;
			}
		}
	}
}



