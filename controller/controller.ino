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
  // start with all the lights off
  for (int i=0; i < 200; i++) {
	light_off(i);
	delay(1);
  }
  for (int i=1; i <= 9; i++) {
	led_color(i, OFF);
  }
}

elapsedMillis ms;

void loop() {
	lights_update();
	leds_update();
	buttons_update();

	if (button_press(4)) {
		light_toggle(4);
		Serial.println("Button 4");
		playEffect.play("BEEP01.WAV");
	}
	if (button_press(5)) {
		light_toggle(5);
		Serial.println("Button 5");
		playEffect.play("BEEP02.WAV");
	}
	if (button_press(6)) {
		// 6 and 7 affect each other's lights...
		light_toggle(6);
		light_toggle(7);
		playEffect.play("BEEP03.WAV");
	}
	if (button_press(7)) {
		// 6 and 7 affect each other's lights...
		light_toggle(6);
		light_toggle(7);
		playEffect.play("BEEP04.WAV");
	}
	if (button_press(8)) {
		playEffect.play("BEEP05.WAV");
	}
	if (button_press(9)) {
		if (light_is_blinking(9)) {
			// when blinking, press turns light off, no sound
			led_color(9, OFF);
			light_off(9);
		} else if (light_is_on(9)) {
			// when on solid, press make it blink, plays beep
			led_color(9, 0x303030);
			playEffect.play("BEEP03.WAV");
			light_blink(9, 200);
		} else {
			// when off, press turns it on solid, plays beep
			led_color(9, WHITE);
			playEffect.play("BEEP07.WAV");
			light_on(9);
		}
	}
	if (button_press(10)) {
		light_toggle(10);
		if (light_is_on(10)) {
			playEffect.play("BEEP08.WAV");
		}
	}
	if (button_press(11)) {
		light_toggle(11);
		if (light_is_on(11)) {
			playEffect.play("BEEP09.WAV");
		}
	}
	if (button_press(12)) {
		led_color(2, YELLOW);
		led_color(3, GREEN);
		light_toggle(12);
		if (light_is_on(12)) {
			playEffect.play("BEEP10.WAV");
		}
	}
	if (button_press(13)) {
		led_color(1, BLUE);
		led_color(2, BLUE);
		light_toggle(13);
		if (light_is_on(13)) {
			playEffect.play("BEEP11.WAV");
		}
	}
	if (button_press(14)) {
		led_color(1, PINK);
		led_color(2, RED);
		if (light_is_blinking(14)) {
			light_off(14);
		} else {
			light_blink(14, 100);
			playEffect.play("BEEP12.WAV");
		}
	}
	if (button_press(15)) {
		light_toggle(15);
		playEffect.play("BEEP13.WAV");
	}
	if (button_press(16)) {
		light_toggle(16);
		playEffect.play("BEEP14.WAV");
	}
	if (button_press(17)) {
		light_toggle(17);
		playEffect.play("BEEP16.WAV");
	}
	if (button_press(18)) {
		light_toggle(18);
		playEffect.play("BEEP17.WAV");
	}
	if (button_press(19)) {
		light_toggle(19);
		playEffect.play("BEEP18.WAV");
	}
	if (button_press(20)) {
		light_toggle(20);
		playEffect.play("BEEP19.WAV");
	}
	if (button_press(21)) {
		light_toggle(21);
		playEffect.play("BEEP21.WAV");
	}
	if (button_press(22)) {
		light_toggle(22);
		playEffect.play("BEEP22.WAV");
	}
	if (button_press(33)) {
		light_blink_toggle(33, 150);
		playEffect.play("BEEP23.WAV");
	}
	if (button_press(36)) {
		playEffect.play("BEEP24.WAV");
	}
	if (button_press(38)) {
		light_blink_toggle(38, 600);
		playEffect.play("BEEP25.WAV");
	}
	if (button_press(39)) {
		playEffect.play("BEEP26.WAV");
	}
	if (button_press(40)) {
		light_toggle(40);
		playEffect.play("BEEP27.WAV");
	}
	if (button_press(41)) {
		playEffect.play("BEEP29.WAV");
	}
	if (button_press(42)) {
		playEffect.play("BEEP30.WAV");
	}
	if (button_press(43)) {
		light_toggle(43);
		playEffect.play("BEEP31.WAV");
	}
	if (button_press(44)) {
		playEffect.play("BEEP32.WAV");
	}
	if (button_press(45)) {
		playEffect.play("BEEP34.WAV");
	}
	if (button_press(46)) {
		light_toggle(46);
		playEffect.play("BEEP35.WAV");
	}
	if (button_press(47)) {
		playEffect.play("BEEP36.WAV");
	}
	if (button_press(48)) {
		playEffect.play("BEEP39.WAV");
	}
	if (button_press(49)) {
		playEffect.play("BEEP41.WAV");
	}
	if (button_press(50)) {
		playEffect.play("BEEP49.WAV");
	}
	if (button_press(51)) {
		playEffect.play("BEEP50.WAV");
	}
	if (button_press(52)) {
		playEffect.play("BEEP51.WAV");
	}
	if (button_press(53)) {
		playEffect.play("BEEP52.WAV");
	}
	if (button_press(54)) {
		playEffect.play("BEEP53.WAV");
	}
	if (button_press(55)) {
		led_color(5, RED);
		led_color(6, RED);
		led_color(7, RED);
		playEffect.play("BEEP54.WAV");
		light_blink(199, 250);
	}
	if (button_press(57)) {
		playEffect.play("BEEP55.WAV");
	}
	if (button_press(58)) {
		light_blink_toggle(58, 250);
		playEffect.play("BEEP58.WAV");
	}
	if (button_press(59)) {
		light_blink_toggle(59, 400);
		playEffect.play("BEEP60.WAV");
	}
	if (button_press(60)) {
		playEffect.play("BEEP61.WAV");
	}
	if (button_press(61)) {
		playEffect.play("BEEP62.WAV");
	}
	if (button_press(62)) {
		light_toggle(62);
		playEffect.play("BEEP66.WAV");
	}
	if (button_press(63)) {
		playEffect.play("BEEP68.WAV");
	}
	if (button_press(64)) {
		playEffect.play("BEEP69.WAV");
	}
	if (button_press(65)) {
		playEffect.play("BEEP70.WAV");
	}
	if (button_press(66)) {
		light_toggle(66);
		playEffect.play("BEEP71.WAV");
	}
	if (button_press(67)) {
		light_toggle(67);
		playEffect.play("BEEP72.WAV");
	}
	if (button_press(68)) {
		light_toggle(68);
		playEffect.play("BEEP73.WAV");
	}
	if (button_press(69)) {
		light_toggle(69);
		playEffect.play("BEEP74.WAV");
	}
	if (button_press(70)) {
		light_blink_toggle(70, 1000);
		playEffect.play("BEEP75.WAV");
	}
	if (button_press(71)) {
		light_blink_toggle(71, 1080);
		playEffect.play("BEEP01.WAV");
	}
	if (button_press(72)) {
		light_blink_toggle(72, 1200);
		playEffect.play("BEEP02.WAV");
	}
	if (button_press(73)) {
		light_blink_toggle(73, 1333);
		playEffect.play("BEEP03.WAV");
	}
	if (button_press(74)) {
		playEffect.play("BEEP04.WAV");
	}
	if (button_press(75)) {
		playEffect.play("BEEP05.WAV");
	}
	if (button_press(76)) {
		playEffect.play("BEEP07.WAV");
	}
	if (button_press(77)) {
		playEffect.play("BEEP08.WAV");
	}
	if (button_press(78)) {
		playEffect.play("BEEP09.WAV");
	}
	if (button_press(79)) {
		playEffect.play("BEEP10.WAV");
	}
	if (button_press(80)) {
		playEffect.play("BEEP11.WAV");
	}
	if (button_press(81)) {
		led_color(5, OFF);
		led_color(6, OFF);
		led_color(7, OFF);
		light_off(199);
		playEffect.play("BEEP12.WAV");
	}
	if (button_press(82)) {
		light_toggle(82);
		playEffect.play("BEEP13.WAV");
	}
	if (button_press(83)) {
		light_toggle(83);
		playEffect.play("BEEP14.WAV");
	}
	if (button_press(84)) {
		light_toggle(84);
		playEffect.play("BEEP16.WAV");
	}
	if (button_press(85)) {
		light_toggle(85);
		playEffect.play("BEEP17.WAV");
	}
	if (button_press(86)) {
		light_toggle(86);
		playEffect.play("BEEP18.WAV");
	}
	if (button_press(87)) {
		light_toggle(87);
		playEffect.play("BEEP19.WAV");
	}
	if (button_press(88)) {
		light_toggle(88);
		playEffect.play("BEEP21.WAV");
	}
	if (button_press(89)) {
		light_toggle(89);
		playEffect.play("BEEP22.WAV");
	}
	if (button_press(90)) {
		light_toggle(90);
		playEffect.play("BEEP23.WAV");
	}
	if (button_press(91)) {
		playEffect.play("BEEP24.WAV");
	}
	if (button_press(92)) {
		light_blink_toggle(92, 200);
		playEffect.play("BEEP25.WAV");
	}
	if (button_press(93)) {
		light_blink_toggle(93, 240);
		playEffect.play("BEEP26.WAV");
	}
	if (button_press(94)) {
		light_blink_toggle(94, 290);
		playEffect.play("BEEP27.WAV");
	}
	if (button_press(95)) {
		light_blink_toggle(95, 200);
		playEffect.play("BEEP29.WAV");
	}
	if (button_press(96)) {
		light_blink_toggle(96, 200);
		playEffect.play("BEEP30.WAV");
	}
	if (button_press(97)) {
		light_blink_toggle(97, 250);
		playEffect.play("BEEP31.WAV");
	}
	if (button_press(98)) {
		light_blink_toggle(98, 220);
		playEffect.play("BEEP32.WAV");
	}
	if (button_press(99)) {
		light_blink_toggle(99, 200);
		playEffect.play("BEEP34.WAV");
	}
	if (button_press(100)) {
		light_blink_toggle(100, 275);
		playEffect.play("BEEP35.WAV");
	}
	if (button_press(101)) {
		playEffect.play("BEEP36.WAV");
	}
	if (button_press(102)) {
		playEffect.play("BEEP39.WAV");
	}
	if (button_press(103)) {
		playEffect.play("BEEP41.WAV");
	}
	if (button_press(104)) {
		playEffect.play("BEEP49.WAV");
	}
	if (button_press(105)) {
		playEffect.play("BEEP50.WAV");
	}
	if (button_press(106)) {
		playEffect.play("BEEP51.WAV");
	}
	if (button_press(107)) {
		playEffect.play("BEEP52.WAV");
	}
	if (button_press(108)) {
		playEffect.play("BEEP53.WAV");
	}
	if (button_press(109)) {
		playEffect.play("BEEP54.WAV");
	}
	if (button_press(110)) {
		playEffect.play("BEEP55.WAV");
	}
	if (button_press(111)) {
		playEffect.play("BEEP58.WAV");
	}
	if (button_press(112)) {
		playEffect.play("BEEP60.WAV");
	}
	if (button_press(113)) {
		playEffect.play("BEEP61.WAV");
	}
	if (button_press(114)) {
		playEffect.play("BEEP62.WAV");
	}
	if (button_press(115)) {
		light_toggle(115);
		playEffect.play("BEEP68.WAV");
	}
	if (button_press(116)) {
		light_toggle(116);
		playEffect.play("BEEP69.WAV");
	}
	if (button_press(117)) {
		light_toggle(117);
		playEffect.play("BEEP70.WAV");
	}
	if (button_press(118)) {
		light_toggle(118);
		playEffect.play("BEEP71.WAV");
	}
	if (button_press(119)) {
		light_toggle(119);
		playEffect.play("BEEP72.WAV");
	}
	if (button_press(120)) {
		light_toggle(120);
		playEffect.play("BEEP73.WAV");
	}
	if (button_press(121)) {
		light_toggle(121);
		playEffect.play("BEEP74.WAV");
	}
	if (button_press(122)) {
		light_toggle(122);
		playEffect.play("BEEP75.WAV");
	}
	if (button_press(123)) {
		playEffect.play("BEEP01.WAV");
	}
	if (button_press(124)) {
		playEffect.play("BEEP02.WAV");
	}
	if (button_press(125)) {
		playEffect.play("BEEP03.WAV");
	}
	if (button_press(126)) {
		playEffect.play("BEEP04.WAV");
	}
	if (button_press(127)) {
		playEffect.play("BEEP05.WAV");
	}
	if (button_press(128)) {
		light_toggle(128);
		playEffect.play("BEEP07.WAV");
	}
	if (button_press(129)) {
		light_toggle(129);
		playEffect.play("BEEP08.WAV");
	}
	if (button_press(130)) {
		light_toggle(130);
		playEffect.play("BEEP09.WAV");
	}
	if (button_press(131)) {
		playEffect.play("BEEP10.WAV");
	}
	if (button_press(132)) {
		playEffect.play("BEEP11.WAV");
	}
	if (button_press(133)) {
		playEffect.play("BEEP12.WAV");
	}
	if (button_press(134)) {
		light_blink_toggle(134, 221);
		playEffect.play("BEEP13.WAV");
	}
	if (button_press(135)) {
		light_blink_toggle(135, 340);
		playEffect.play("BEEP14.WAV");
	}
	if (button_press(136)) {
		light_blink_toggle(136, 333);
		playEffect.play("BEEP16.WAV");
	}
	if (button_press(137)) {
		light_blink_toggle(137, 300);
		playEffect.play("BEEP17.WAV");
	}
	if (button_press(138)) {
		light_blink_toggle(138, 349);
		playEffect.play("BEEP18.WAV");
	}
	if (button_press(139)) {
		light_blink_toggle(139, 156);
		playEffect.play("BEEP19.WAV");
	}
	if (button_press(140)) {
		light_blink_toggle(140, 630);
		playEffect.play("BEEP21.WAV");
	}
	if (button_press(141)) {
		light_blink_toggle(141, 240);
		playEffect.play("BEEP22.WAV");
	}
	if (button_press(142)) {
		light_blink_toggle(142, 320);
		playEffect.play("BEEP23.WAV");
	}
	if (button_press(143)) {
		light_toggle(143);
		playEffect.play("BEEP24.WAV");
	}
	if (button_press(144)) {
		light_toggle(144);
		playEffect.play("BEEP25.WAV");
	}
	if (button_press(145)) {
		light_toggle(145);
		playEffect.play("BEEP26.WAV");
	}
	if (button_press(146)) {
		playEffect.play("BEEP27.WAV");
	}
	if (button_press(147)) {
		playEffect.play("BEEP29.WAV");
	}
	if (button_press(148)) {
		light_toggle(148);
		playEffect.play("BEEP30.WAV");
	}
	if (button_press(149)) {
		light_toggle(149);
		playEffect.play("BEEP31.WAV");
	}
	if (button_press(150)) {
		playEffect.play("BEEP32.WAV");
	}
	if (button_press(151)) {
		playEffect.play("BEEP34.WAV");
	}
	if (button_press(152)) {
		light_toggle(152);
		playEffect.play("BEEP35.WAV");
	}
	if (button_press(153)) {
		light_toggle(153);
		playEffect.play("BEEP36.WAV");
	}
	if (button_press(154)) {
		playEffect.play("BEEP39.WAV");
	}
	if (button_press(155)) {
		light_toggle(155);
		playEffect.play("BEEP41.WAV");
	}
	if (button_press(156)) {
		light_toggle(156);
		playEffect.play("BEEP49.WAV");
	}
	if (button_press(157)) {
		light_toggle(157);
		playEffect.play("BEEP50.WAV");
	}
	if (button_press(158)) {
		playEffect.play("BEEP51.WAV");
	}
	if (button_press(159)) {
		playEffect.play("BEEP52.WAV");
	}
	if (button_press(160)) {
		light_toggle(160);
		playEffect.play("BEEP53.WAV");
	}
	if (button_press(161)) {
		light_toggle(161);
		playEffect.play("BEEP54.WAV");
	}
	if (button_press(162)) {
		light_toggle(162);
		playEffect.play("BEEP55.WAV");
	}
	if (button_press(163)) {
		light_toggle(163);
		playEffect.play("BEEP58.WAV");
	}











	if (ms > 300) {
		ms -= 300;
	}
}

