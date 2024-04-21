#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include "Adafruit_MAX31855.h"

TwoWire i2c1 = TwoWire(PB7 , PB6);
Adafruit_SSD1306 display(128, 32, &i2c1, -1);

//SPIClass spi1(PA7 , PA6 , PA5 , PA4);
Adafruit_MAX31855 thermocouple(PA5, PA4 , PA6);

uint8_t blink_flag = 0;

void setup() {
	i2c1.begin();
	Serial.begin(115200); //打开串口通讯，设置传输速率为115200字节每秒
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;);
    }
	display.display();

	delay(2000);
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(SSD1306_WHITE);
	display.display();
	Serial.println(F("Done cleaning."));

	Serial.println("MAX31855 test");
	delay(500);
	Serial.print("Initializing sensor...");
	if (!thermocouple.begin()) {
		Serial.println("ERROR.");
		while (1) delay(10);
	}
	Serial.println(F("Done start up."));
	thermocouple.setFaultChecks(MAX31855_FAULT_ALL); 

    pinMode(PC13 , OUTPUT);
}

void loop() {
	display.clearDisplay();
	display.setCursor(0,0);
	display.print("Int:");
	display.println(thermocouple.readInternal());

	double c = thermocouple.readCelsius();
	if (isnan(c)) {
		display.clearDisplay();
		display.setCursor(0,0);
		uint8_t e = thermocouple.readError();
		if (e & MAX31855_FAULT_OPEN) display.println("FAULT: no connection.");
		if (e & MAX31855_FAULT_SHORT_GND) display.println("FAULT: short to GND.");
		if (e & MAX31855_FAULT_SHORT_VCC) display.println("FAULT: short to VCC.");
	} else {
		display.print("Cur:");
		display.println(c);
	}
	display.display();

	if (blink_flag < 8){
		if (blink_flag <4){
			digitalWrite(PC13 , HIGH);
		}else{
			digitalWrite(PC13 , LOW);
		}
		delay(200);
		blink_flag++;
	}else{
		blink_flag = 0;
	}
}
