int detector = 47; // define the obstacle avoidance sensor interface
int pinBuzzer = 49;
int val ;

int led_verde_G = 51;
int led_rojo_R = 52;
int led_azul_B = 50;

void setup() {   
  pinMode (detector, INPUT) ;// define the obstacle avoidance sensor output interface
  pinMode(led_verde_G, OUTPUT);  
  pinMode(led_rojo_R, OUTPUT); 
  pinMode(led_azul_B, OUTPUT); 
}

void loop() {
  val = digitalRead (detector) ;
  if (val == HIGH) // When the obstacle avoidance sensor detects a signal, LED flashes
  {
    tone(pinBuzzer, 500);
    digitalWrite(led_verde_G, HIGH);   // Se enciende el LED VERDE
    digitalWrite(led_rojo_R, LOW);
    digitalWrite(led_azul_B, LOW);
    while(val == HIGH){
      val = digitalRead (detector) ;
    }
    noTone(pinBuzzer);
  }

  digitalWrite(led_verde_G, LOW);   // Se enciende el LED ROJO
  digitalWrite(led_rojo_R, HIGH);
  digitalWrite(led_azul_B, LOW);

  delay(250);

  digitalWrite(led_verde_G, LOW);   // Se enciende el LED AZUL
  digitalWrite(led_rojo_R, LOW);
  digitalWrite(led_azul_B, HIGH);
  
  
  delay(250);
  
  digitalWrite(led_verde_G, HIGH);   // Se enciende el LED VERDE
  digitalWrite(led_rojo_R, LOW);
  digitalWrite(led_azul_B, LOW);
  
  delay(250);
}
