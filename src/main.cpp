#include <Arduino.h>

#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

void setup()
{
  Serial.begin(9600);

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
}