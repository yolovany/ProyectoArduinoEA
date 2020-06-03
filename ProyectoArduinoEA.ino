// PROYECTO DE ELETRÓNICA AVANZADA.
// OBTENCIÓN DE PARÁMETROS DE SENSORES PARA INVERNADERO 
// Autores: Luís Macías y Jovany Hernández

bool flag;
#include <DHT.h>
#include <RTClib.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4   
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F,16,2);

void SerialPrint(float h, float t, float f);
void LCDPrint(bool esBienvenida, bool flag, float h, float t, float f);

void setup() {
  flag = false;
  Serial.begin(9600);
  dht.begin();
  Wire.begin();
  lcd.init();
  rtc.begin();
  LCDPrint(true, false, 0, 0, 0);
}

void SerialPrint(float h, float t, float f)
{
  DateTime datetime = rtc.now();
  Serial.print(datetime.day(), DEC);
  Serial.print("/");
  Serial.print(datetime.month(), DEC);
  Serial.print("/");
  Serial.print(datetime.year(), DEC);
  Serial.print(" ");
  Serial.print(datetime.hour(), DEC);
  Serial.print(":");
  Serial.print(datetime.minute(), DEC);
  Serial.print(":");
  Serial.println(datetime.second(), DEC);

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print("% ");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(f);
}

void LCDPrint(bool esBienvenida, bool flag, float h, float t, float f){
  lcd.clear();
  if(!esBienvenida){
    DateTime datetime = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(datetime.day());
    lcd.print("/");
    lcd.print(datetime.month());
    lcd.print("/");
    lcd.print(datetime.year());
    lcd.print(" ");
    lcd.print(datetime.hour());
    lcd.print(":");
    lcd.print(datetime.minute());

    lcd.setCursor(0, 1);
    if(flag){
      lcd.print("H: ");
      lcd.print(h);
      lcd.print("%");
    }
    else
    {
      lcd.print("T: ");
      lcd.print(t);
      lcd.print(" C"); 
    }
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("HOLA MUNDO CRUEL");
    lcd.setCursor(0,1);
    lcd.print("    Y DESPIADADO :)");
    delay(2000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    delay(2000);
    lcd.clear();
  } 
}

void loop() {
  delay(2000);
  flag = !flag;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    h = 0;
    t = 0;
    f = 0;
  }

  SerialPrint(h, t, f);
  LCDPrint(false, flag, h, t, f);
}
