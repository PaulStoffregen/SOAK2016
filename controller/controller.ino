// pin connections:

//  0 = Serial1 receive, buttons & pots on control panel
//  1 = Serial1 transmit, lights on control panel
//  2
//  3
//  4 = SD card: CS
//  5
//  6
//  7 = SD card: MOSI
//  8 = Serial 3 transmit: vibe control
//  9 = Audio shield: BCLK
// 10 = Serial 2 transmit: DMX lighting
// 11 = Audio shield: MCLK
// 12 = SD card: MISO
// 13 = Audio shield: RX
// 14 = SE card: SCK
// 15
// 16
// 17
// 18 = Audio shield: SDA
// 19 = Audio shield: SCL
// 20 =
// 21 =
// 22 = Audio shield: TX
// 23 = Audio shield: RX

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "soak.h"

AudioPlaySdWav           playEffect;
AudioPlaySdWav           playVoice;
AudioPlaySdWav           playBackground;
AudioMixer4              mixer1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playEffect, 0, mixer1, 0);
AudioConnection          patchCord2(playVoice, 0, mixer1, 1);
AudioConnection          patchCord3(playBackground, 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, 0, audioOutput, 0);
AudioConnection          patchCord5(mixer1, 0, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

void setup() {
  // turn on serial ports, for controlling everything
  Serial1.begin(117647);
  Serial2.setRX(26);   // don't take pin 9 away from audio shield
  Serial2.begin(250000);
  Serial3.begin(9600); // pin 7 will be claimed by SPI with SD.begin()

  // turn on the audio shield
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);

  // start up the SD card
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(4))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  // TODO: why does this not turn off all the lights?
  for (int i=0; i < 200; i++) {
	light_off(i);
	delay(1);
  }
}

int n=199;
elapsedMillis ms;
int bstate;

void loop() {
	buttons_update();

	if (button_press(4)) {
		Serial.println("Button 4");
		playEffect.play("BEEP01.WAV");
	}
	if (button_press(5)) {
		Serial.println("Button 5");
		playEffect.play("BEEP02.WAV");
	}
	if (button_press(6)) {
		playEffect.play("BEEP03.WAV");
	}
	if (button_press(7)) {
		playEffect.play("BEEP04.WAV");
	}
	if (button_press(8)) {
		playEffect.play("BEEP05.WAV");
	}
	if (button_press(9)) {
		playEffect.play("BEEP07.WAV");
	}
	if (button_press(10)) {
		playEffect.play("BEEP08.WAV");
	}
	if (button_press(11)) {
		playEffect.play("BEEP09.WAV");
	}
	if (button_press(12)) {
		playEffect.play("BEEP10.WAV");
	}
	if (button_press(13)) {
		playEffect.play("BEEP11.WAV");
	}
	if (button_press(14)) {
		playEffect.play("BEEP12.WAV");
	}
	if (button_press(15)) {
		playEffect.play("BEEP13.WAV");
	}
	if (button_press(16)) {
		playEffect.play("BEEP14.WAV");
	}
	if (button_press(17)) {
		playEffect.play("BEEP16.WAV");
	}
	if (button_press(18)) {
		playEffect.play("BEEP17.WAV");
	}
	if (button_press(19)) {
		playEffect.play("BEEP18.WAV");
	}
	if (button_press(20)) {
		playEffect.play("BEEP19.WAV");
	}
	if (button_press(21)) {
		playEffect.play("BEEP21.WAV");
	}



	if (ms > 300) {
		if (bstate) {
			light_on(n);
			bstate = 0;
		} else {
			light_off(n);
			bstate = 1;
		}
		ms -= 300;
	}
}

