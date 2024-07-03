#include <WiFi.h>
#include <PubSubClient.h>

// Define HC-SR04 pins
#define TRIG_PIN 22
#define ECHO_PIN 23

int tiempo, distancia;

// Wi-Fi credentials
const char* ssid = SSID;
const char* password = PASSWORD;

// MQTT broker address
const char* mqtt_server = BROKER_IP;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Configure the trigger pin as an output and the echo pin as an input
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to Wi-Fi
  setup_wifi();

  // Set up MQTT
  client.setServer(mqtt_server, 1883);
  client.setSocketTimeout(30);
  reconnect();
}

void setup_wifi() {
  delay(10);
  // Start connecting to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sensor_setup() {
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  sensor_setup();

  tiempo = pulseIn(ECHO_PIN, HIGH); //duration of the pulse 
  distancia = tiempo / 58; // dividing by 58 converts the distance in cm
  
  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Send the distance to the MQTT broker
  char msg[50];
  snprintf(msg, 50, "%d", distancia);
  client.publish("sensor/distance", msg);
  
  // Wait for 1 second before the next measurement
  delay(1000);
}

//.\mosquitto.exe -c "C:\Program Files\mosquitto\mosquitto.conf" -v