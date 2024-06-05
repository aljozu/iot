#include<LiquidCrystal.h>
#include <Keypad.h>

//sensor
const int trigger = 9;
const int echo = 8;
int tiempo, distancia;

//amarillo, rojo, verde
const int LEDs[3] = {5,6,7};

const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Code that shows the keypad connections to the arduino terminals
byte rowPins[numRows] = {22,23,24,25}; //Rows 0 to 3
byte colPins[numCols] = {26,27,28,29}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//initiliazes an instance of the LiquidCrystal class
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);


void setup()
{
  lcd.begin(16, 2);
  lcd.noDisplay();
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  for(auto i: LEDs) pinMode(i, OUTPUT);
  Serial.begin(9600);
  for(auto i: LEDs) digitalWrite(i, LOW);
}

void sensor_setup() {
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
}

void loop()
{
  sensor_setup();
  static char PASSWORD[4] = { '1', '2', '3', '4' };
  static char inputBuffer[4] = {0}; // Array to store key inputs
  static int bufferIndex = 0;       // Index to keep track of inputBuffer

  tiempo=pulseIn(echo,HIGH); //duration of the pulse 
  distancia= tiempo/58; // dividing by 58 converts the distance in cm

  char keypressed = myKeypad.getKey();

  if(distancia<=30){
    lcd.clear();
    lcd.display();
    digitalWrite(LEDs[0], HIGH);
    lcd.print("Ingrese su contraseña:      ");
    if (keypressed != NO_KEY) {
      inputBuffer[bufferIndex] = keypressed; // Store the key in the buffer
      bufferIndex++; // Increment buffer index
      Serial.println(keypressed);
      if (bufferIndex == 4){ 
        Serial.println(inputBuffer);
        if(memcmp(PASSWORD, inputBuffer, sizeof(PASSWORD)) == 0){// check if the passwords match
          lcd.clear();
          lcd.print("Contraseña correcta.");
          digitalWrite(LEDs[2], HIGH); // turn on green led
          delay(3000);
          digitalWrite(LEDs[2], LOW);
        } else {
          lcd.clear();
          lcd.print("Contraseña incorrecta, vuelva a intentar.");
          digitalWrite(LEDs[1], HIGH); // turn on red led
          delay(1000);
          digitalWrite(LEDs[1], LOW);
        }
        bufferIndex = 0;
        memset(inputBuffer, 0, sizeof(inputBuffer)); // Reset inputBuffer
      }
    }
  } else {
    lcd.clear();
    lcd.noDisplay();
    digitalWrite(LEDs[0], LOW);
    bufferIndex = 0;
    memset(inputBuffer, 0, sizeof(inputBuffer)); // Reset inputBuffer
  }
}