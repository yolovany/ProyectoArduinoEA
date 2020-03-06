#include "DHT.h"
#include<DS3231.h>
#include<Wire.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
DS3231 Clock;
byte year, month, date, DoW, hour, minute, second;

void InitDatetime(int year, int month, int day, int hour, int minute, int second, bool is24H);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Wire.begin();
  InitDatetime(20,3,6,8,0,0,false);
}

void InitDatetime(int year, int month, int day, int hour, int minute, int second, bool is24H)
{
  Clock.setClockMode(!is24H); 
Clock.setYear((byte)year);
Clock.setMonth((byte)month);
Clock.setDate((byte)day);
Clock.setHour((byte)hour);
Clock.setMinute((byte)minute);
Clock.setSecond((byte)second);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

   //Se rescata la información
Clock.getTime(year, month, date, DoW, hour, minute, second);
//Se imprime
Serial.print(date, DEC);
Serial.print("/");
Serial.print(month, DEC);
Serial.print("/");
Serial.print(year, DEC);
Serial.print("  ");
Serial.print(hour, DEC);
Serial.print(":");
Serial.print(minute, DEC);
Serial.print(":");
Serial.println(second, DEC);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

}
