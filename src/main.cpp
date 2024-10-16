#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <EasyNextionLibrary.h>
#include <SPI.h>
#include <Preferences.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

Preferences Settings;

EasyNex myNex(Serial2);
uint16_t lengthGraph;

int parameters[3] = {1, 1, 1};

void saveSettings()
{
  Settings.putInt("low", parameters[0]);
  Settings.putInt("high", parameters[1]);
  Settings.putInt("length", parameters[2]);
}
void loadSettings()
{
  parameters[0] = Settings.getInt("low");
  parameters[1] = Settings.getInt("high");
  parameters[2] = Settings.getInt("length");
}

const int REFRESH_TIME = 100;           // time to refresh the Nextion page every 100 ms
unsigned long refresh_timer = millis(); // timer for refreshing Nextion's page

void setup()
{
  Serial.begin(9600);
  myNex.begin(9600);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
}

void loop()
{
  myNex.NextionListen();
  int16_t adc0;
  float volts0;

  adc0 = ads.readADC_SingleEnded(0);
  volts0 = ads.computeVolts(adc0);

  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: ");
  Serial.print(adc0);
  Serial.print("  ");
  Serial.print(volts0);
  Serial.println("V");
  delay(1000);

  if ((millis() - refresh_timer) > REFRESH_TIME)
  {
    lengthGraph = map(adc0, 3, 17574, 0, 400);
    Serial.print("Distance : ");
    Serial.println(lengthGraph);
    myNex.writeNum("vRaw.val", lengthGraph);
    refresh_timer = millis();
  }
}

// Select Settings - 00
void trigger0()
{
    myNex.writeStr("page pSettings");
}