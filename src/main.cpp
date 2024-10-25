#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <EasyNextionLibrary.h>
#include <SPI.h>
#include <Preferences.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

Preferences Settings;

EasyNex myNex(Serial2);
uint16_t lengthGraph;

double parameters[3] = {1, 1, 150};

  int16_t adc0;
  float volts0;

void saveSettings()
{
  Settings.putDouble("low", parameters[0]);
  Settings.putDouble("high", parameters[1]);
  Settings.putDouble("length", parameters[2]);
}
void loadSettings()
{
  parameters[0] = Settings.getDouble("low");
  parameters[1] = Settings.getDouble("high");
  parameters[2] = Settings.getDouble("length");
}

const int REFRESH_TIME = 500;           // time to refresh the Nextion page every 100 ms
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

  Settings.begin("parameters", false);
  // saveSettings();
  loadSettings();
  
  myNex.writeStr("page main");
}

void loop()
{
  myNex.NextionListen();
  adc0 = ads.readADC_SingleEnded(0);
  if ((millis() - refresh_timer) > REFRESH_TIME)
  {
    Serial.println("-----------------------------------------------------------");
    Serial.print("AIN0: ");
    Serial.println(adc0);
    lengthGraph = map(adc0, parameters[0], parameters[1], 0, parameters[2]);
    Serial.print("Distance : ");
    Serial.println(lengthGraph);
    myNex.writeNum("vRaw.val", lengthGraph);
    myNex.writeNum("vStroke.val", parameters[2]);
    myNex.writeStr("txtbL.txt", String(lengthGraph) + " mm");

    myNex.writeStr("curMin.txt", String(parameters[0]));
    myNex.writeStr("curMax.txt", String(parameters[1]));
    myNex.writeStr("curStroke.txt", String(parameters[2])+" mm");
    refresh_timer = millis();

    Serial.print("Min :");
    Serial.println(parameters[0]);
    Serial.print("Max :");
    Serial.println(parameters[1]);
    Serial.print("Stroke :");
    Serial.println(parameters[2]);
  }
}

// printh 23 02 54 00
// Select Settings - 00
void trigger0()
{
  myNex.writeStr("page runAuto");
  double Lmultiplier = parameters[2] / 8;
  myNex.writeStr("w9L.txt", String(round(Lmultiplier * 8), 0) + " mm");
  myNex.writeStr("w8L.txt", String(round(Lmultiplier * 7), 0) + " mm");
  myNex.writeStr("w7L.txt", String(round(Lmultiplier * 6), 0) + " mm");
  myNex.writeStr("w6L.txt", String(round(Lmultiplier * 5), 0) + " mm");
  myNex.writeStr("w5L.txt", String(round(Lmultiplier * 4), 0) + " mm");
  myNex.writeStr("w4L.txt", String(round(Lmultiplier * 3), 0) + " mm");
  myNex.writeStr("w3L.txt", String(round(Lmultiplier * 2), 0) + " mm");
  myNex.writeStr("w2L.txt", String(round(Lmultiplier * 1), 0) + " mm");
  myNex.writeStr("w1L.txt", String(round(Lmultiplier * 0), 0) + " mm");
  Serial.println(Lmultiplier);
}

// printh 23 02 54 01
// Select Settings - 01
void trigger1()
{
  myNex.writeStr("page main");
}

// printh 23 02 54 02
// Select Settings - 02
void trigger2()
{
  myNex.writeStr("page calibration");
}

// printh 23 02 54 03
// Select Settings - 03
void trigger3()
{
  // Save
  saveSettings();
  loadSettings();
}

// printh 23 02 54 04
// Select Settings - 04
void trigger4()
{
  // Set Minimum
  parameters[0] = adc0;
  myNex.writeNum("lowerLimit.val",parameters[0]);
}

// printh 23 02 54 05
// Select Settings - 05
void trigger5()
{
  // Set Max
  parameters[1] = adc0;
  myNex.writeNum("upperLimit.val",parameters[1]);
}

// printh 23 02 54 06
// Select Settings - 06
void trigger6()
{
  // Set Stroke
  parameters[2] = myNex.readNumber("totalStroke.val");
}

// printh 23 02 54 07
// Select Settings - 07
void trigger7()
{
  myNex.writeStr("page main");
}