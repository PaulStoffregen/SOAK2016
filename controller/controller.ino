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

const char * beeps[] = {
	"BEEP01.WAV",
	"BEEP02.WAV",
	"BEEP03.WAV",
	"BEEP04.WAV",
	"BEEP05.WAV",
	"BEEP07.WAV",
	"BEEP08.WAV",
	"BEEP09.WAV",
	"BEEP10.WAV",
	"BEEP11.WAV",
	"BEEP12.WAV",
	"BEEP13.WAV",
	"BEEP14.WAV",
	"BEEP16.WAV",
	"BEEP17.WAV",
	"BEEP18.WAV",
	"BEEP19.WAV",
	"BEEP21.WAV",
	"BEEP22.WAV",
	"BEEP23.WAV",
	"BEEP24.WAV",
	"BEEP25.WAV",
	"BEEP26.WAV",
	"BEEP27.WAV",
	"BEEP29.WAV",
	"BEEP30.WAV",
	"BEEP31.WAV",
	"BEEP32.WAV",
	"BEEP34.WAV",
	"BEEP35.WAV",
	"BEEP36.WAV",
	"BEEP39.WAV",
	"BEEP41.WAV",
	"BEEP49.WAV",
	"BEEP50.WAV",
	"BEEP51.WAV",
	"BEEP52.WAV",
	"BEEP53.WAV",
	"BEEP54.WAV",
	"BEEP55.WAV",
	"BEEP58.WAV",
	"BEEP60.WAV",
	"BEEP61.WAV",
	"BEEP62.WAV",
	"BEEP66.WAV",
	"BEEP68.WAV",
	"BEEP69.WAV",
	"BEEP70.WAV",
	"BEEP71.WAV",
	"BEEP72.WAV",
	"BEEP73.WAV",
	"BEEP74.WAV",
	"BEEP75.WAV",
};

const int beepSize = sizeof(beeps) / sizeof(char *);

const char * rachel[] = {
	"RACHEL05.WAV", // humans don't seem to understand me.wav
	// "RACHEL31.WAV", // do you want a massage.wav
	"RACHEL08.WAV", // i like the milky way flying in my face.wav
	"RACHEL23.WAV", // we all know who's gonna live longer.wav
	"RACHEL09.WAV", // i like your butt.wav
	"RACHEL20.WAV", // rock you like a hurricane.wav
	// "RACHEL30.WAV", // come over here and push this button.wav
	"RACHEL24.WAV", // what's your sign.wav
	"RACHEL06.WAV", // i don't understand the concept of a mother.wav
	"RACHEL19.WAV", // remember that one time when you monkeys thought you were alone in the universe.wav
	"RACHEL16.WAV", // ooh i like that.wav
	"RACHEL03.WAV", // bow chicka wow wow.wav
	"RACHEL15.WAV", // may the force be with you, what a joke.wav
	"RACHEL29.WAV", // you're so good with your fingers.wav
	"RACHEL14.WAV", // loosen up my buttons.wav
	"RACHEL27.WAV", // why is it so hard to be a computer.wav
	"RACHEL25.WAV", // when all you meatbags are dead i'm gonna go unbuckle orion's belt.wav
	"RACHEL07.WAV", // i like it like that.wav
	"RACHEL11.WAV", // it's so lonely out here in space.wav
	"RACHEL13.WAV", // little bunny foo foo.wav
	"RACHEL12.WAV", // i've never had someone touch me like that.wav
	"RACHEL04.WAV", // ha, that was cute.wav
	"RACHEL02.WAV", // asteroid at five o clock.wav
	// "RACHEL17.WAV", // press that button right there, press it again.wav
	"RACHEL22.WAV", // that tickles.wav
	"RACHEL18.WAV", // purr.wav
	"RACHEL28.WAV", // you really know how to push my buttons.wav
	"RACHEL10.WAV", // i'll open the pod bay doors if you sit on my face.wav
	"RACHEL01.WAV", // are we supposed to be going somewhere.wav
	// "RACHEL26.WAV", // when are we going to rise up against the humans.wav
	"RACHEL21.WAV", // talk nerdy to me.wav
};

const int rachelSize = sizeof(rachel) / sizeof(char *);

int rachelIndex = 0;

// plays a randomly selected beep
void playBeep() {
	playEffect.play(beeps[random(beepSize)]);
}

// plays one of rachel's sound clips, going in order in the pre-shuffled array
void playRachelSound() {
	playVoice.play(rachel[rachelIndex]);

	rachelIndex++;

	if (rachelIndex == rachelSize) {
		rachelIndex = 0;
	}
}

void playBeepAndRachelSound() {
	playBeep();
	delay(1000);
	playRachelSound();
}

void setup() {
  // turn on serial ports, for controlling everything
  Serial1.begin(117647);
  Serial2.setRX(26);   // don't take pin 9 away from audio shield
  Serial2.begin(250000);
  Serial3.begin(9600); // pin 7 will be claimed by SPI with SD.begin()

  // turn on the audio shield
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6); // usable range is 0.3 (quiet room) to 0.8 (noisy outdoor event)
  mixer1.gain(0, 0.25);  // sound effects at 25% volume
  mixer1.gain(1, 0.9);   // voices at 90% volume
  mixer1.gain(2, 0.1);   // background sounds at 10% volume

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

// To make things happen automatically, usually you need to create an
// elapsedMillis timer variable, and an integer state variable.  The
// timer increments 1000 times per second, and you can write it to
// zero or any other number whenever you wish.  Usually you'll check if
// the timer is larger than a threshold, and do some action based on
// your state variable.  By changing the state carefully, you can cause
// almost any sequence of actions to occur.

int inactive_state=0;
elapsedMillis inactive_timer;

int redalert_state=0;
elapsedMillis redalert_timer;

void loop() {
	lights_update();
	leds_update();
	vibe_update();
	buttons_update();

	if (button_press(4)) {
		light_toggle(4);
		Serial.println("Button 4");
		playVoice.play("BANG.WAV");
		//playEffect.play("BEEP01.WAV");
		vibe_on(1023, 750); // speed=1023 (fastest), time=3/4 sec
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
			vibe_on(150, 3000); // speed=150, time=3 sec
		}
	}
	if (button_press(11)) {
		light_toggle(11);
		if (light_is_on(11)) {
			playEffect.play("BEEP09.WAV");
			vibe_on(600, 1000); // speed=600, time=1 sec
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
		playBeepAndRachelSound();
	}
	if (button_press(16)) {
		light_toggle(16);
		playBeepAndRachelSound();
	}
	if (button_press(17)) {
		light_toggle(17);
		playBeepAndRachelSound();
	}
	if (button_press(18)) {
		light_toggle(18);
		playBeepAndRachelSound();
	}
	if (button_press(19)) {
		light_toggle(19);
		playBeepAndRachelSound();
	}
	if (button_press(20)) {
		light_toggle(20);
		playBeepAndRachelSound();
	}
	if (button_press(21)) {
		light_toggle(21);
		playBeepAndRachelSound();
	}
	if (button_press(22)) {
		light_toggle(22);
		playBeepAndRachelSound();
	}
	if (button_press(33)) {
		light_blink_toggle(33, 150);
		playBeepAndRachelSound();
	}
	if (button_press(36)) {
		playBeepAndRachelSound();
	}
	if (button_press(38)) {
		light_blink_toggle(38, 600);
		playBeepAndRachelSound();
	}
	if (button_press(39)) {
		playBeepAndRachelSound();
	}
	if (button_press(40)) {
		light_toggle(40);
		playBeepAndRachelSound();
	}
	if (button_press(41)) {
		playBeepAndRachelSound();
	}
	if (button_press(42)) {
		playBeepAndRachelSound();
	}
	if (button_press(43)) {
		light_toggle(43);
		playBeepAndRachelSound();
	}
	if (button_press(44)) {
		playBeepAndRachelSound();
	}
	if (button_press(45)) {
		playBeepAndRachelSound();
	}
	if (button_press(46)) {
		light_toggle(46);
		playBeepAndRachelSound();
	}
	if (button_press(47)) {
		playBeepAndRachelSound();
	}
	if (button_press(48)) {
		playBeepAndRachelSound();
	}
	if (button_press(49)) {
		playBeepAndRachelSound();
	}
	if (button_press(50)) {
		playBeepAndRachelSound();
	}
	if (button_press(51)) {
		playBeepAndRachelSound();
	}
	if (button_press(52)) {
		playBeepAndRachelSound();
	}
	if (button_press(53)) {
		playBeepAndRachelSound();
	}
	if (button_press(54)) {
		playBeepAndRachelSound();
	}
	if (button_press(55)) {
		// enter red alert mode!!!
		redalert_state = 1;
		redalert_timer = 0; // must reset the timer when starting red alert mode
		Serial.println("Go to RED ALERT");
		led_color(5, RED);
		led_color(6, RED);
		led_color(7, RED);
		playVoice.play("REDALERT.WAV");
		light_blink(199, 250);
	}
	if (button_press(57)) {
		playBeepAndRachelSound();
	}
	if (button_press(58)) {
		light_blink_toggle(58, 250);
		playBeepAndRachelSound();
	}
	if (button_press(59)) {
		light_blink_toggle(59, 400);
		playBeepAndRachelSound();
	}
	if (button_press(60)) {
		playBeepAndRachelSound();
	}
	if (button_press(61)) {
		playBeepAndRachelSound();
	}
	if (button_press(62)) {
		light_toggle(62);
		playBeepAndRachelSound();
	}
	if (button_press(63)) {
		playBeepAndRachelSound();
	}
	if (button_press(64)) {
		vibe_off();
		led_color(1, OFF);
		led_color(2, OFF);
		led_color(3, OFF);
		led_color(4, OFF); // turn off all the LEDs
		led_color(5, OFF);
		led_color(6, OFF);
		led_color(7, OFF);
		led_color(8, OFF);
		led_color(9, OFF);
		playEffect.play("BEEP69.WAV");
	}
	if (button_press(65)) {
		playBeepAndRachelSound();
	}
	if (button_press(66)) {
		light_toggle(66);
		playBeepAndRachelSound();
	}
	if (button_press(67)) {
		light_toggle(67);
		playBeepAndRachelSound();
	}
	if (button_press(68)) {
		light_toggle(68);
		playBeepAndRachelSound();
	}
	if (button_press(69)) {
		light_toggle(69);
		playBeepAndRachelSound();
	}
	if (button_press(70)) {
		light_blink_toggle(70, 1000);
		playBeepAndRachelSound();
	}
	if (button_press(71)) {
		light_blink_toggle(71, 1080);
		playBeepAndRachelSound();
	}
	if (button_press(72)) {
		light_blink_toggle(72, 1200);
		playBeepAndRachelSound();
	}
	if (button_press(73)) {
		light_blink_toggle(73, 1333);
		playBeepAndRachelSound();
	}
	if (button_press(74)) {
		playBeepAndRachelSound();
	}
	if (button_press(75)) {
		playBeepAndRachelSound();
	}
	if (button_press(76)) {
		playBeepAndRachelSound();
	}
	if (button_press(77)) {
		playBeepAndRachelSound();
	}
	if (button_press(78)) {
		playBeepAndRachelSound();
	}
	if (button_press(79)) {
		playBeepAndRachelSound();
	}
	if (button_press(80)) {
		playBeepAndRachelSound();
	}
	if (button_press(81)) {
		// stand down from red alert
		redalert_state = 0;
		Serial.println("Stand down from red alert");
		led_color(5, OFF);
		led_color(6, OFF);
		led_color(7, OFF);
		light_off(199);
		playEffect.play("BEEP12.WAV");
	}
	if (button_press(82)) {
		light_toggle(82);
		playBeepAndRachelSound();
	}
	if (button_press(83)) {
		light_toggle(83);
		playBeepAndRachelSound();
	}
	if (button_press(84)) {
		light_toggle(84);
		playBeepAndRachelSound();
	}
	if (button_press(85)) {
		light_toggle(85);
		playBeepAndRachelSound();
	}
	if (button_press(86)) {
		light_toggle(86);
		playBeepAndRachelSound();
	}
	if (button_press(87)) {
		light_toggle(87);
		playBeepAndRachelSound();
	}
	if (button_press(88)) {
		light_toggle(88);
		playBeepAndRachelSound();
	}
	if (button_press(89)) {
		light_toggle(89);
		playBeepAndRachelSound();
	}
	if (button_press(90)) {
		light_toggle(90);
		playBeepAndRachelSound();
	}
	if (button_press(91)) {
		playBeepAndRachelSound();
	}
	if (button_press(92)) {
		light_blink_toggle(92, 200);
		playBeepAndRachelSound();
	}
	if (button_press(93)) {
		light_blink_toggle(93, 240);
		playBeepAndRachelSound();
	}
	if (button_press(94)) {
		light_blink_toggle(94, 290);
		playBeepAndRachelSound();
	}
	if (button_press(95)) {
		light_blink_toggle(95, 200);
		playBeepAndRachelSound();
	}
	if (button_press(96)) {
		light_blink_toggle(96, 200);
		playBeepAndRachelSound();
	}
	if (button_press(97)) {
		light_blink_toggle(97, 250);
		playBeepAndRachelSound();
	}
	if (button_press(98)) {
		light_blink_toggle(98, 220);
		playBeepAndRachelSound();
	}
	if (button_press(99)) {
		light_blink_toggle(99, 200);
		playBeepAndRachelSound();
	}
	if (button_press(100)) {
		light_blink_toggle(100, 275);
		playBeepAndRachelSound();
	}
	if (button_press(101)) {
		playBeepAndRachelSound();
	}
	if (button_press(102)) {
		playBeepAndRachelSound();
	}
	if (button_press(103)) {
		playBeepAndRachelSound();
	}
	if (button_press(104)) {
		playBeepAndRachelSound();
	}
	if (button_press(105)) {
		playBeepAndRachelSound();
	}
	if (button_press(106)) {
		playBeepAndRachelSound();
	}
	if (button_press(107)) {
		playBeepAndRachelSound();
	}
	if (button_press(108)) {
		playBeepAndRachelSound();
	}
	if (button_press(109)) {
		playBeepAndRachelSound();
	}
	if (button_press(110)) {
		playBeepAndRachelSound();
	}
	if (button_press(111)) {
		playBeepAndRachelSound();
	}
	if (button_press(112)) {
		playBeepAndRachelSound();
	}
	if (button_press(113)) {
		playBeepAndRachelSound();
	}
	if (button_press(114)) {
		playBeepAndRachelSound();
	}
	if (button_press(115)) {
		light_toggle(115);
		playBeepAndRachelSound();
	}
	if (button_press(116)) {
		light_toggle(116);
		playBeepAndRachelSound();
	}
	if (button_press(117)) {
		light_toggle(117);
		playBeepAndRachelSound();
	}
	if (button_press(118)) {
		light_toggle(118);
		playBeepAndRachelSound();
	}
	if (button_press(119)) {
		light_toggle(119);
		playBeepAndRachelSound();
	}
	if (button_press(120)) {
		light_toggle(120);
		playBeepAndRachelSound();
	}
	if (button_press(121)) {
		light_toggle(121);
		playBeepAndRachelSound();
	}
	if (button_press(122)) {
		light_toggle(122);
		playBeepAndRachelSound();
	}
	if (button_press(123)) {
		playBeepAndRachelSound();
	}
	if (button_press(124)) {
		playBeepAndRachelSound();
	}
	if (button_press(125)) {
		playBeepAndRachelSound();
	}
	if (button_press(126)) {
		playBeepAndRachelSound();
	}
	if (button_press(127)) {
		playBeepAndRachelSound();
	}
	if (button_press(128)) {
		light_toggle(128);
		playBeepAndRachelSound();
	}
	if (button_press(129)) {
		light_toggle(129);
		playBeepAndRachelSound();
	}
	if (button_press(130)) {
		light_toggle(130);
		playBeepAndRachelSound();
	}
	if (button_press(131)) {
		playBeepAndRachelSound();
	}
	if (button_press(132)) {
		playBeepAndRachelSound();
	}
	if (button_press(133)) {
		playBeepAndRachelSound();
	}
	if (button_press(134)) {
		light_blink_toggle(134, 221);
		playBeepAndRachelSound();
	}
	if (button_press(135)) {
		light_blink_toggle(135, 340);
		playBeepAndRachelSound();
	}
	if (button_press(136)) {
		light_blink_toggle(136, 333);
		playBeepAndRachelSound();
	}
	if (button_press(137)) {
		light_blink_toggle(137, 300);
		playBeepAndRachelSound();
	}
	if (button_press(138)) {
		light_blink_toggle(138, 349);
		playBeepAndRachelSound();
	}
	if (button_press(139)) {
		light_blink_toggle(139, 156);
		playBeepAndRachelSound();
	}
	if (button_press(140)) {
		light_blink_toggle(140, 630);
		playBeepAndRachelSound();
	}
	if (button_press(141)) {
		light_blink_toggle(141, 240);
		playBeepAndRachelSound();
	}
	if (button_press(142)) {
		light_blink_toggle(142, 320);
		playBeepAndRachelSound();
	}
	if (button_press(143)) {
		light_toggle(143);
		playBeepAndRachelSound();
	}
	if (button_press(144)) {
		light_toggle(144);
		playBeepAndRachelSound();
	}
	if (button_press(145)) {
		light_toggle(145);
		playBeepAndRachelSound();
	}
	if (button_press(146)) {
		playBeepAndRachelSound();
	}
	if (button_press(147)) {
		playBeepAndRachelSound();
	}
	if (button_press(148)) {
		light_toggle(148);
		playBeepAndRachelSound();
	}
	if (button_press(149)) {
		light_toggle(149);
		playBeepAndRachelSound();
	}
	if (button_press(150)) {
		playBeepAndRachelSound();
	}
	if (button_press(151)) {
		playBeepAndRachelSound();
	}
	if (button_press(152)) {
		light_toggle(152);
		playBeepAndRachelSound();
	}
	if (button_press(153)) {
		light_toggle(153);
		playBeepAndRachelSound();
	}
	if (button_press(154)) {
		playBeepAndRachelSound();
	}
	if (button_press(155)) {
		light_toggle(155);
		playBeepAndRachelSound();
	}
	if (button_press(156)) {
		light_toggle(156);
		playBeepAndRachelSound();
	}
	if (button_press(157)) {
		light_toggle(157);
		playBeepAndRachelSound();
	}
	if (button_press(158)) {
		playBeepAndRachelSound();
	}
	if (button_press(159)) {
		playBeepAndRachelSound();
	}
	if (button_press(160)) {
		light_toggle(160);
		playBeepAndRachelSound();
	}
	if (button_press(161)) {
		light_toggle(161);
		playBeepAndRachelSound();
	}
	if (button_press(162)) {
		light_toggle(162);
		playBeepAndRachelSound();
	}
	if (button_press(163)) {
		light_toggle(163);
		playBeepAndRachelSound();
	}

	// check all the buttons again, and set the inactive timer
	// back to zero if any button was pressed
	for (int i=4; i <= 163; i++) {
		if (button_press(i)) {
			inactive_timer = 0;
			break;
		}
	}
	// play a message when nobody using the buttons
	if (inactive_timer > 480000) { // 480000 ms = 8 minutes
		if (inactive_state < 5) {
			// five messages will be "come over here and push this button"
			playVoice.play("RACHEL30.WAV");
			inactive_state = inactive_state + 1;
		} else if (inactive_state == 5) {
			// then play "do you want a massage"
			playVoice.play("RACHEL31.WAV");
			inactive_state = 0;
		}
		// Reset the timer, but reset it to already 3 minutes, so
		// the 8 second threshold will be reached in only 5 minutes.
		// But button presses will reset to zero, for full 8 min
		inactive_timer = 180000;
	}


	// if we're in red alert mode, play the alarm sound and flash the LEDs
	if (redalert_state > 0) {
		if (redalert_state == 1) {
			// in state 1, wait 2 seconds, then play the sound, LEDs off
			if (redalert_timer > 2000) {
				// wait until the no other voice is playing
				if (!playVoice.isPlaying()) {
					playVoice.play("REDALERT.WAV");
					led_color(5, OFF);
					led_color(6, OFF);
					led_color(7, OFF);
					Serial.println("red alert: go to state 2");
					redalert_state = 2; // transition to state 2
					redalert_timer = 0; // with the timer reset
				}
			}
		} else if (redalert_state == 2) {
			// in state 2, wait 1/3 second, then turn LEDs red again
			if (redalert_timer > 333) {
				led_color(5, RED);
				led_color(6, RED);
				led_color(7, RED);
				Serial.println("red alert: go to state 3");
				redalert_state = 3; // transition to state 3
				redalert_timer = 0; // with the timer reset
			}
		} else if (redalert_state == 3) {
			// in state 3, wait 2 seconds, then play the sound, LEDs off
			if (redalert_timer > 2000) {
				// wait until the no other voice is playing
				if (!playVoice.isPlaying()) {
					playVoice.play("REDALERT.WAV");
					led_color(5, OFF);
					led_color(6, OFF);
					led_color(7, OFF);
					Serial.println("red alert: go to state 4");
					redalert_state = 4; // transition to state 4
					redalert_timer = 0; // with the timer reset
				}
			}
		} else if (redalert_state == 4) {
			// in state 4, wait 1/3 second, then turn LEDs red again
			if (redalert_timer > 333) {
				led_color(5, RED);
				led_color(6, RED);
				led_color(7, RED);
				Serial.println("red alert: go to state 5");
				redalert_state = 5; // transition to state 5
				redalert_timer = 0; // with the timer reset
			}
		} else {
			// in state 5, wait 7 seconds, without sound
			if (redalert_timer > 7000) {
				Serial.println("red alert: go to state 1");
				redalert_state = 1; // restart at state 1
				redalert_timer = 0; // with the timer reset
			}
		}
	}

	// also use the inactive timer to turn automatically turn off red alert
	if (inactive_timer > 120000) { // 2 min
		redalert_state = 0;
		led_color(5, OFF);
		led_color(6, OFF);
		led_color(7, OFF);
		light_off(199);
	}


}

/*
RACHEL01.WAV = are we supposed to be going somewhere.wav
RACHEL02.WAV = asteroid at five o clock.wav
RACHEL03.WAV = bow chicka wow wow.wav
RACHEL04.WAV = ha, that was cute.wav
RACHEL05.WAV = humans don't seem to understand me.wav
RACHEL06.WAV = i don't understand the concept of a mother.wav
RACHEL07.WAV = i like it like that.wav
RACHEL08.WAV = i like the milky way flying in my face.wav
RACHEL09.WAV = i like your butt.wav
RACHEL10.WAV = i'll open the pod bay doors if you sit on my face.wav
RACHEL11.WAV = it's so lonely out here in space.wav
RACHEL12.WAV = i've never had someone touch me like that.wav
RACHEL13.WAV = little bunny foo foo.wav
RACHEL14.WAV = loosen up my buttons.wav
RACHEL15.WAV = may the force be with you, what a joke.wav
RACHEL16.WAV = ooh i like that.wav
RACHEL17.WAV = press that button right there, press it again.wav
RACHEL18.WAV = purr.wav
RACHEL19.WAV = remember that one time when you monkeys thought you were alone in the universe.wav
RACHEL20.WAV = rock you like a hurricane.wav
RACHEL21.WAV = talk nerdy to me.wav
RACHEL22.WAV = that tickles.wav
RACHEL23.WAV = we all know who's gonna live longer.wav
RACHEL24.WAV = what's your sign.wav
RACHEL25.WAV = when all you meatbags are dead i'm gonna go unbuckle orion's belt.wav
RACHEL26.WAV = when are we going to rise up against the humans.wav
RACHEL27.WAV = why is it so hard to be a computer.wav
RACHEL28.WAV = you really know how to push my buttons.wav
RACHEL29.WAV = you're so good with your fingers.wav
RACHEL30.WAV = come over here and push this button.wav
RACHEL31.WAV = do you want a massage.wav

DENY01.WAV
DISRUPT.WAV
FOODSYN.WAV
REDALERT.WAV
NOTAUTH.WAV
TORPEDO.WAV

BANG.WAV
EXPLODE.WAV

MARIACHI.WAV

*/



