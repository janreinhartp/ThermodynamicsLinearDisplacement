#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <EasyNextionLibrary.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

EasyNex myNex(Serial2);
uint16_t lengthGraph;

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
    lengthGraph = map(volts0, 0, 1024, 0, 400);
    myNex.writeNum("NvoltageGraph.val", lengthGraph); 
    refresh_timer = millis();
  }
}