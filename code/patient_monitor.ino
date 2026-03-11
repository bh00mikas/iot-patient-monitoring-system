#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Create the LCD object using MCP23008 at address 0x20
Adafruit_LiquidCrystal lcd(0);

#define HR_SENSOR A0
#define TEMP_SENSOR A1
#define BUZZER 8

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);     // Set up the LCD's number of columns and rows
  lcd.setBacklight(1);  // Turn on backlight

  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Simulated heart rate and SpO2
  int hrRaw = analogRead(HR_SENSOR);
  int heartRate = map(hrRaw, 0, 1023, 60, 120);
  int spo2 = map(hrRaw, 0, 1023, 90, 100);

  // Read temperature from TMP36
  int tempRaw = analogRead(TEMP_SENSOR);
  float voltage = tempRaw * 5.0 / 1023.0;
  float tempC = (voltage - 0.5) * 100.0;

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HR:");
  lcd.print(heartRate);
  lcd.print(" SpO2:");
  lcd.print(spo2);

  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(tempC, 1);
  lcd.print(" C");

  // Emergency alert logic
  bool emergency = false;
  if (heartRate < 60 || heartRate > 100) emergency = true;
  if (spo2 < 95) emergency = true;
  if (tempC > 38.0) emergency = true;

  if (emergency) {
    digitalWrite(BUZZER, HIGH);
    Serial.println("ALERT: Vitals out of safe range!");
  } else {
    digitalWrite(BUZZER, LOW);
    Serial.println("Vitals normal.");
  }

  Serial.print("HR: ");
  Serial.print(heartRate);
  Serial.print(" bpm, SpO2: ");
  Serial.print(spo2);
  Serial.print(" %, Temp: ");
  Serial.print(tempC);
  Serial.println(" C");

  delay(3000);
}
