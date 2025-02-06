// Define Blynk template ID and name 
#define BLYNK_TEMPLATE_ID           "TMPL6lrPdsUaS"
#define BLYNK_TEMPLATE_NAME         "Smart Door Lock System"
#define BLYNK_AUTH_TOKEN            "acMXt-XMnF6NiASRw9iFqJ5SSWZfA_QK"

// Define Libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Keypad.h>
#include <ESP32Servo.h>

// Pin Definitions
#define BUZZER_PIN 2
#define LED_PIN 4
#define PIR_PIN 13
#define TRIG_PIN 25
#define ECHO_PIN 26
#define LDR_PIN 34
#define BUTTON_PIN 12  // Push button pin definition

// Blynk Authentication
char ssid[] = "Wokwi-GUEST";       // Replace with your Wi-Fi SSID
char pass[] = "";                   // Replace with your Wi-Fi Password

// Blynk Timer for virtual pin updates
BlynkTimer timer;

// Virtual Pins for Blynk
#define VIRTUAL_ULTRASONIC V1
#define VIRTUAL_MOTION V2
#define VIRTUAL_LOCK_STATUS V3
#define VIRTUAL_LIGHT_LEVEL V4
#define VIRTUAL_MANUAL_DOOR_CONTROL V5

// Keypad Setup
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {19, 18, 17, 16}; 
byte colPins[COLS] = {23, 22, 21, 5}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo myServo;
String password = "234#";
String input_password;
boolean doorUnlocked = false;
boolean blynkButtonState = false;
boolean manualButtonPressed = false;

// Ultrasonic Setup
long duration;
int distance;

// Timing for Ultrasonic Readings
unsigned long previousMillis = 0;
const long interval = 1000;

// PIR Setup
boolean motionDetected = false;

// Setup
void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  myServo.attach(33);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  myServo.write(0);
  Serial.println("Smart Door Lock System Initialized.");
}

void loop() {
  Blynk.run();

  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    input_password += key;

    if (key == '#') {
      Serial.print("Entered Password: ");
      Serial.println(input_password);

      if (input_password == password) {
        unlockDoor();
      } else {
        wrongPassword();
      }

      input_password = "";
    }
  }

  motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    Serial.println("Motion detected near the keypad!");
    Blynk.virtualWrite(VIRTUAL_MOTION, "Motion Detected Near The Keypad");
  } else {
    Blynk.virtualWrite(VIRTUAL_MOTION, "No Motion");
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    distance = measureDistance();
    Serial.print("Ultrasonic Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    Blynk.virtualWrite(VIRTUAL_ULTRASONIC, distance);
  }

    int lightLevel = analogRead(LDR_PIN);
  Serial.print("Current light level (LDR): ");
  Serial.println(lightLevel);
  
  if (lightLevel < 200) {
    digitalWrite(LED_PIN, HIGH);  // Turn on LED if light level is below 200
    Serial.println("LED turned ON due to low light level.");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off LED if light level is above 200
    Serial.println("LED turned OFF as light level is sufficient.");
  }

  Blynk.virtualWrite(VIRTUAL_LIGHT_LEVEL, lightLevel);

  if (digitalRead(BUTTON_PIN) == LOW) {
    manualButtonPressed = !manualButtonPressed;
    delay(500);

    if (manualButtonPressed && !doorUnlocked) {
      unlockDoor();
    } else if (!manualButtonPressed && doorUnlocked) {
      if (distance > 150) {
        lockDoor();
      } else {
        Serial.println("Cannot lock door: Object/person detected too close.");
        Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Lock Failed: Obstacle Near Door");
      }
    }
  }

  if (!blynkButtonState && doorUnlocked) {
    if (distance > 150) {
      lockDoor();
    } else {
      Serial.println("Cannot lock door: Object/person detected too close.");
      Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Lock Failed: Obstacle Near Door");
    }
  }
}

BLYNK_WRITE(VIRTUAL_MANUAL_DOOR_CONTROL) {
  blynkButtonState = param.asInt();

  if (blynkButtonState == 1 && !doorUnlocked) {
    unlockDoor();
  } else if (blynkButtonState == 0 && doorUnlocked) {
    if (distance > 150) {
      lockDoor();
    } else {
      Serial.println("Cannot lock door: Object/person detected too close.");
      Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Lock Failed: Obstacle Near Door");
    }
  }
}

void unlockDoor() {
  Serial.println("Correct password, unlocking door...");
  myServo.write(90);
  doorUnlocked = true;
  Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Door Unlocked");
}

void lockDoor() {
  if (distance <= 30) {
    Serial.println("Cannot lock the door: Object/person is too close.");
    Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Lock Failed: Obstacle Near Door");
  } else {
    Serial.println("Locking door...");
    myServo.write(0);
    doorUnlocked = false;
    Blynk.virtualWrite(VIRTUAL_LOCK_STATUS, "Door Locked");
  }
}

void wrongPassword() {
  Serial.println("Wrong password, triggering buzzer...");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
