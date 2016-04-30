

unsigned int incoming=0;
elapsedMillis timeout;
elapsedMillis blinktime;
uint8_t ledstate=0;
int ch=0;
int count=0;
int sum[10];
int val[10];
int prev[10];
elapsedMillis sendall;
int sendallch=0;
elapsedMicros analogtime;

void setup()
{
	for (int i=0; i < 25; i++) {
		// all pins low, except Serial1 & A0-A9
		if (i == 2 || i == 3 || (i >= 12 && i <= 21)) continue;
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
	Serial1.begin(115200);
	Serial.println("analog...");
	for (int i=0; i < 10; i++) {
		prev[i] = 127;
	}
}

void transmit(char cmd, unsigned int num)
{
	Serial.print(num);
	Serial.println(cmd);
	Serial1.print(num);
	Serial1.print(cmd);
}

void loop()
{
	while (Serial1.available()) {
		char c = Serial1.read();
		if (c >= '0' && c <= '9') {
			incoming *= 10;
			incoming += c - '0';
		} else if (c == 'a') {
			transmit(c, incoming + 10 * 100);
		} else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
			transmit(c, incoming);
			incoming = 0;
		}
		timeout = 0;
	}
	if (timeout > 250) {
		incoming = 0;
		timeout = 0;
	}
	if (blinktime > 500) {
		blinktime -= 500;
		ledstate = (ledstate ? LOW : HIGH);
		digitalWrite(0, ledstate);
	}
	if (analogtime > 200) {  // 200us -> limit output rate to about 20 Hz
		analogtime -= 200;
		sum[ch] += analogRead(ch);
		if (++ch > 9) {
			ch = 0;
				if (++count >= 25) {  // final values to range from 0 to 99
				count = 0;
				for (int i=0; i < 10; i++) {
					const int thresh = val[i] << 8;
					const int hyst = 224;
					if (sum[i] < thresh - hyst || sum[i] > thresh + hyst) {
						val[i] = sum[i] >> 8;
					}
					sum[i] = 0;
					//Serial.printf("%4d", val[i]);
				}
				//Serial.println();
				for (int i=0; i < 10; i++) {
					if (val[i] != prev[i]) {
						transmit('A', val[i] + i * 100);
						prev[i] = val[i];
					}
				}
			}
		}
	}
	if (sendall > 1000) {
		sendall = 0;
		transmit('a', val[sendallch] + sendallch * 100);
		if (++sendallch > 9) sendallch = 0;
	}
}
