#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

const char* ssid = "TP-Link_38E2";
const char* password = "40377506";
const char* iftttKey = "Lw_hsjG8BXGtq715socge";
const char* iftttEventStart = "sunlight_start";
const char* iftttEventStop = "sunlight_stop";

int lightPin = A0;
int threshold = 400;
bool sunlight = false;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, "maker.ifttt.com", 80);

void setup() {
  Serial.begin(9600);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
}

void loop() {
  int lightValue = analogRead(lightPin);
  Serial.print("Light: "); Serial.println(lightValue);

  if (!sunlight && lightValue > threshold) {
    sunlight = true;
    triggerIFTTT(iftttEventStart);
  } else if (sunlight && lightValue <= threshold) {
    sunlight = false;
    triggerIFTTT(iftttEventStop);
  }

  delay(5000); // Check every 5 seconds
}

void triggerIFTTT(const char* event) {
  String url = "/trigger/" + String(event) + "/with/key/" + iftttKey;
  client.get(url);
  Serial.println("IFTTT Triggered: " + String(event));
}

