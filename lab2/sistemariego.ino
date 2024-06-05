#include <LiquidCrystal.h>

// Inicializar el objeto LCD con los pines apropiados
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int sensor = A0;
const int BLUE_LED = 2;
const int RED_LED = 3;
const int buzzer = 4;
const int relay = 5;

int sensorValue = 0;
int humidity = 0;


void setup() {

  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Sistema de Riego");
  delay(2000);
}

void loop() {

  sensorValue = analogRead(sensor);

  humidity = map(sensorValue, 0, 1023, 0, 100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humidity);
  lcd.print("%");

  // Si la humedad es menor que el 20%, activar el riego
  if (humidity > 80) {
    lcd.setCursor(0, 1);
    lcd.print("Riego Activado");
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(relay, HIGH);
    //Tiempo de regado
    delay(10000);
    digitalWrite(relay, LOW);
    digitalWrite(BLUE_LED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Inactivo");
    //Tiempo inactivo
    delay(15000);
  } 
  // Si la humedad es mayor que el 80%, activar la alarma
  else if (humidity < 20) {
    lcd.setCursor(0, 1);
    lcd.print("Sobrehidratado");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(5000);
    digitalWrite(RED_LED, LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(1000);
}