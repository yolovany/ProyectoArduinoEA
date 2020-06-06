
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define PORT 81
#define SENSOR_HS A1

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
EthernetServer server(PORT);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F,16,4);
int rele=8;
int pinBuzzer = 7;

void setup() {
  pinMode(SENSOR_HS, INPUT);
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(rele, OUTPUT);
  lcd.init();  
  lcd.backlight();

  lcd.setCursor(8,1);
  lcd.print("HOLA!");
  tone(pinBuzzer, 700);
  delay(150);
  noTone(pinBuzzer);
  tone(pinBuzzer, 800);
  delay(150);
  noTone(pinBuzzer);
  tone(pinBuzzer, 900);
  delay(150);
  noTone(pinBuzzer);
  tone(pinBuzzer, 1200);
  delay(150);
  noTone(pinBuzzer);
  delay(1000);
  
  lcd.clear();
  lcd.print("Electronica Avanzada");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Proyecto final");
  delay(1000);
  lcd.setCursor(0,2);
  lcd.print("Por Jovany Hernandez");
  delay(2000);
  
  lcd.clear();
  lcd.print("Iniciando");
  delay(750);
  lcd.print(".");
  delay(250);
  lcd.print(".");
  delay(250);
  lcd.print(".");
  delay(250);
  lcd.setCursor(0,1);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());
  delay(500);
  lcd.setCursor(0,2);
  lcd.print("Puerto: ");
  lcd.print(PORT);
  lcd.setCursor(0,3);
  delay(500);
  lcd.print("Servidor iniciado!");
  delay(2500);
  lcd.clear();
  
  Serial.begin(9600);
  dht.begin();
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int valorHumedad = map(analogRead(SENSOR_HS), 0, 1023, 100, 0);

  if(t>23){
    digitalWrite(rele, HIGH); // turn the LED on (HIGH is the voltage level)
  }
  else {
    digitalWrite(rele, LOW); // turn the LED off by making the voltage LOW
  }

  lcd.clear();
  lcd.backlight();
  lcd.print("Humedad: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temperatura: ");
  lcd.print(t);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("Agua: ");
  lcd.print(analogRead(A0));
  lcd.print("/1024");
  lcd.setCursor(0,3);
  lcd.print("Humedad suelo: ");
  lcd.print(valorHumedad);
  lcd.print("%");

  /*if(analogRead(A0) <= 300){
    lcd.clear();
    lcd.print("ATENCION");
    lcd.setCursor(0,1);
    lcd.print("Nivel de agua bajo.");
    tone(pinBuzzer, 523);
    while(analogRead(A0) <= 250){}
    noTone(pinBuzzer);
  }
  else
  {
    
  }*/
  
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        int valorHumedad = map(analogRead(SENSOR_HS), 0, 1023, 100, 0);

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t)) {
          lcd.setCursor(0,2);
          lcd.print(F("Error DHT :("));
          return;
        }

        char c = client.read();
        Serial.write(c);
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        if (c == '\n' && currentLineIsBlank) {
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          
          client.print("HUMEDAD");
          client.print(h);
          client.print("%TEMPERATURA");
          client.print(t);
          client.print("CHUMEDADSUELO");
          client.print(valorHumedad);
          client.print("%");
          client.println("<html>");

          lcd.clear();
          lcd.setCursor(2,1);
          lcd.print("LECTURA ENVIADA");
          tone(pinBuzzer, 800);
          delay(100);
          noTone(pinBuzzer);
          tone(pinBuzzer, 900);
          delay(100);
          noTone(pinBuzzer);
          tone(pinBuzzer, 1200);
          delay(100);
          noTone(pinBuzzer);
          delay(1000);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    client.stop();
  }
}
