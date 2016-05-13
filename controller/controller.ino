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

#define SDCARD_CS_PIN    4
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

void setup() {
  Serial1.begin(117647);
  Serial2.setRX(26); // don't take pin 9 away from audio shield
  Serial2.begin(250000);
  Serial3.begin(9600);

  AudioMemory(8);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  }
}


void loop() {
  playFile("SDTEST1.WAV");  // filenames are always uppercase 8.3 format
  delay(500);
  playFile("SDTEST2.WAV");
  delay(500);
  playFile("SDTEST3.WAV");
  delay(500);
  playFile("SDTEST4.WAV");
  delay(1500);
}

