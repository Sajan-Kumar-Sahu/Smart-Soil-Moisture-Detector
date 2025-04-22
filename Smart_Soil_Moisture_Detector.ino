#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL317xGH2dF"
#define BLYNK_TEMPLATE_NAME "Smart Soil Moisture Detector"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "KslgFx96c4ASNBhn_zfYKgkT8bzFxYOi"; // Auth token
char ssid[] = "Sajan";                            // WiFi SSID
char pass[] = "kumarSajan";                       // WiFi Password

#define sensor A0
#define waterPump D3

BlynkTimer timer;

bool Relay = 0;
bool autoMode = false;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH); // motor OFF initially

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  timer.setInterval(1000L, soilMoistureSensor);
}

// Auto/Manual Mode Switch
BLYNK_WRITE(V3) {
  autoMode = param.asInt();
  Serial.print("Mode: ");
  Serial.println(autoMode ? "Auto" : "Manual");
}

// Manual Control Button
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (!autoMode) {
    digitalWrite(waterPump, Relay ? LOW : HIGH);
  }
}

void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value); // Display moisture % on Blynk

  if (autoMode) {
    if (value < 40) {
      digitalWrite(waterPump, LOW);  // Turn motor ON
      Blynk.virtualWrite(V1, 1);     // Reflect in manual button
    } else {
      digitalWrite(waterPump, HIGH); // Turn motor OFF
      Blynk.virtualWrite(V1, 0);     // Reflect in manual button
    }
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
