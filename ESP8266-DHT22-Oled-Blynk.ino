#define BLYNK_TEMPLATE_ID "TMPL6D27F0kXS"
#define BLYNK_TEMPLATE_NAME "ESP8266 DHT22 Oled"

#define BLYNK_AUTH_TOKEN "e8BEUcmWMfIukFf-UCFMStXQCnuY_Jvh"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DHT.h>

// ======================
// WIFI
// ======================
char ssid[] = "muktafi";
char pass[] = "12443411";

// ======================
// DHT22
// ======================
#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ======================
// OLED
// ======================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ======================
// TIMER
// ======================
BlynkTimer timer;

// ======================
// FUNCTION
// ======================
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // cek gagal baca sensor
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca DHT22");
    return;
  }

  // kirim ke serial monitor
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.print(" C  ");

  Serial.print("Kelembapan: ");
  Serial.print(h);
  Serial.println(" %");

  // kirim ke Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // tampil OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("MONITORING DHT22");

  display.setTextSize(2);

  display.setCursor(0, 20);
  display.print(t);
  display.println(" C");

  display.setCursor(0, 45);
  display.print(h);
  display.println(" %");

  display.display();
}

void setup()
{
  Serial.begin(115200);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal"));
    for(;;);
  }

  display.clearDisplay();
  display.display();

  // DHT
  dht.begin();

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // timer baca sensor tiap 2 detik
  timer.setInterval(2000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}