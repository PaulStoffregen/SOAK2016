
#include <Bounce.h>

Bounce button00(0, 50);
Bounce button01(1, 50);
Bounce button02(4, 50);
Bounce button03(5, 50);
Bounce button04(7, 50);
Bounce button05(8, 50);
Bounce button06(9, 50);
Bounce button07(10, 50);
Bounce button08(11, 50);
Bounce button09(12, 50);
Bounce button10(13, 50);
Bounce button11(14, 50);
Bounce button12(15, 50);
Bounce button13(16, 50);
Bounce button14(17, 50);
Bounce button15(18, 50);
Bounce button16(19, 50);
Bounce button17(20, 50);
Bounce button18(21, 50);
Bounce button19(22, 50);
Bounce button20(23, 50);
Bounce button21(24, 50);
Bounce button22(25, 50);
Bounce button23(26, 50);
Bounce button24(27, 50);
Bounce button25(28, 50);
Bounce button26(29, 50);
Bounce button27(30, 50);
Bounce button28(31, 50);
Bounce button29(32, 50);
Bounce button30(33, 50);
Bounce button31(34, 50);
Bounce button32(35, 50);
Bounce button33(38, 50);
Bounce button34(39, 50);
Bounce button35(40, 50);
Bounce button36(41, 50);
Bounce button37(42, 50);
Bounce button38(43, 50);
Bounce button39(44, 50);
Bounce button40(45, 50);

void setup()
{
	for (int i=0; i < 46; i++) {
		// buttons on all pins, except Serial1 & LED
		if (i == 2 || i == 3 || i == 6) continue;
		pinMode(i, INPUT_PULLUP);
	}
	pinMode(6, OUTPUT); // LED
	Serial1.begin(115200);
}

void transmit(const char *str)
{
	Serial.println(str);
	Serial1.print(str);
}

unsigned int incoming=0;
elapsedMillis timeout;
elapsedMillis blinktime;
uint8_t ledstate=0;

void loop()
{
	if (Serial1.available()) {
		timeout = 0;
		char c = Serial1.read();
		if (c >= '0' && c <= '9') {
			incoming *= 10;
			incoming += c - '0';
		} else {
			incoming += 41;
			Serial1.print(incoming);
			Serial1.print(c);
			Serial.print(incoming);
			Serial.println(c);
			incoming = 0;
		}
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
	button00.update();
	button01.update();
	button02.update();
	button03.update();
	button04.update();
	button05.update();
	button06.update();
	button07.update();
	button08.update();
	button09.update();
	button10.update();
	button11.update();
	button12.update();
	button13.update();
	button14.update();
	button15.update();
	button16.update();
	button17.update();
	button18.update();
	button19.update();
	button20.update();
	button21.update();
	button22.update();
	button23.update();
	button24.update();
	button25.update();
	button26.update();
	button27.update();
	button28.update();
	button29.update();
	button30.update();
	button31.update();
	button32.update();
	button33.update();
	button34.update();
	button35.update();
	button36.update();
	button37.update();
	button38.update();
	button39.update();
	button40.update();

	if (button00.fallingEdge()) transmit("0B");
	if (button00.risingEdge())  transmit("0b");
	if (button01.fallingEdge()) transmit("1B");
	if (button01.risingEdge())  transmit("1b");
	if (button02.fallingEdge()) transmit("2B");
	if (button02.risingEdge())  transmit("2b");
	if (button03.fallingEdge()) transmit("3B");
	if (button03.risingEdge())  transmit("3b");
	if (button04.fallingEdge()) transmit("4B");
	if (button04.risingEdge())  transmit("4b");
	if (button05.fallingEdge()) transmit("5B");
	if (button05.risingEdge())  transmit("5b");
	if (button06.fallingEdge()) transmit("6B");
	if (button06.risingEdge())  transmit("6b");
	if (button07.fallingEdge()) transmit("7B");
	if (button07.risingEdge())  transmit("7b");
	if (button08.fallingEdge()) transmit("8B");
	if (button08.risingEdge())  transmit("8b");
	if (button09.fallingEdge()) transmit("9B");
	if (button09.risingEdge())  transmit("9b");
	if (button10.fallingEdge()) transmit("10B");
	if (button10.risingEdge())  transmit("10b");
	if (button11.fallingEdge()) transmit("11B");
	if (button11.risingEdge())  transmit("11b");
	if (button12.fallingEdge()) transmit("12B");
	if (button12.risingEdge())  transmit("12b");
	if (button13.fallingEdge()) transmit("13B");
	if (button13.risingEdge())  transmit("13b");
	if (button14.fallingEdge()) transmit("14B");
	if (button14.risingEdge())  transmit("14b");
	if (button15.fallingEdge()) transmit("15B");
	if (button15.risingEdge())  transmit("15b");
	if (button16.fallingEdge()) transmit("16B");
	if (button16.risingEdge())  transmit("16b");
	if (button17.fallingEdge()) transmit("17B");
	if (button17.risingEdge())  transmit("17b");
	if (button18.fallingEdge()) transmit("18B");
	if (button18.risingEdge())  transmit("18b");
	if (button19.fallingEdge()) transmit("19B");
	if (button19.risingEdge())  transmit("19b");
	if (button20.fallingEdge()) transmit("20B");
	if (button20.risingEdge())  transmit("20b");
	if (button21.fallingEdge()) transmit("21B");
	if (button21.risingEdge())  transmit("21b");
	if (button22.fallingEdge()) transmit("22B");
	if (button22.risingEdge())  transmit("22b");
	if (button23.fallingEdge()) transmit("23B");
	if (button23.risingEdge())  transmit("23b");
	if (button24.fallingEdge()) transmit("24B");
	if (button24.risingEdge())  transmit("24b");
	if (button25.fallingEdge()) transmit("25B");
	if (button25.risingEdge())  transmit("25b");
	if (button26.fallingEdge()) transmit("26B");
	if (button26.risingEdge())  transmit("26b");
	if (button27.fallingEdge()) transmit("27B");
	if (button27.risingEdge())  transmit("27b");
	if (button28.fallingEdge()) transmit("28B");
	if (button28.risingEdge())  transmit("28b");
	if (button29.fallingEdge()) transmit("29B");
	if (button29.risingEdge())  transmit("29b");
	if (button30.fallingEdge()) transmit("30B");
	if (button30.risingEdge())  transmit("30b");
	if (button31.fallingEdge()) transmit("31B");
	if (button31.risingEdge())  transmit("31b");
	if (button32.fallingEdge()) transmit("32B");
	if (button32.risingEdge())  transmit("32b");
	if (button33.fallingEdge()) transmit("33B");
	if (button33.risingEdge())  transmit("33b");
	if (button34.fallingEdge()) transmit("34B");
	if (button34.risingEdge())  transmit("34b");
	if (button35.fallingEdge()) transmit("35B");
	if (button35.risingEdge())  transmit("35b");
	if (button36.fallingEdge()) transmit("36B");
	if (button36.risingEdge())  transmit("36b");
	if (button37.fallingEdge()) transmit("37B");
	if (button37.risingEdge())  transmit("37b");
	if (button38.fallingEdge()) transmit("38B");
	if (button38.risingEdge())  transmit("38b");
	if (button39.fallingEdge()) transmit("39B");
	if (button39.risingEdge())  transmit("39b");
	if (button40.fallingEdge()) transmit("40B");
	if (button40.risingEdge())  transmit("40b");
}
