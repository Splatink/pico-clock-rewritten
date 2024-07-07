#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <pico/stdlib.h>
#include <Adafruit_BME280.h>

#define SCK 17
#define SDA 16
#define SUMMER 18
#define BATT A1
#define BUTTON 14

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* ssid = "Limbo";
const char* password = "95H2O862U235@#";

bool lpmode;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "gr.pool.ntp.org", 0, 1296000000);
Adafruit_BME280 bme;

int batteryReader()
{
  static int battery, readings, batterya;
  int batteryv = analogRead(BATT);
  batterya = batterya + batteryv;
  readings++;
  if (readings >= 10) 
  {
    battery = batterya / 10;
    batterya = 0;
    readings = 0;
  }
  return battery;
}

float getBatteryVoltage()
{
  float battv = (batteryReader() * 3.3 / 4095) * 2;
  return battv;
}

float getBatteryPrecentage()
{
  float battp = (getBatteryVoltage() - 2.5) / (4.2 - 2.5) * 100;
  if (battp <= 0) 
  {
    battp = 1;
  }
  if (battp > 99) 
  {
    lpmode = false;
  }
  else 
  {
    lpmode = true;
  }
  return battp;
}

int getWifiRSSI()
{
  static int RSSI, RSSIavg, readings;
  int rssir = WiFi.RSSI();
  RSSIavg = RSSIavg + rssir;
  readings++;
  if (readings >= 10) 
  {
    RSSI = RSSIavg / 10;
    RSSIavg = 0;
    readings = 0;
  }
  return RSSI; 
}

int getWifiStatus(){
  int RSSI = getWifiRSSI();
  int wifiStatus;
  if (RSSI >= -55 && WiFi.status() == WL_CONNECTED) 
  {
    wifiStatus = 0;
  }
  else if (RSSI >= -67 && WiFi.status() == WL_CONNECTED) 
  {
    wifiStatus = 1;
  }
  else if (RSSI >= -90 && WiFi.status() == WL_CONNECTED) 
  {
    wifiStatus = 2;
  }
  else if (WiFi.status() != WL_CONNECTED && lpmode == true) 
  {
    wifiStatus = 3;
  }
  else
  {
    wifiStatus = 4;
    WiFi.begin(ssid, password);
  }
  return wifiStatus;
}

const char* getDay()
{
  const char* day; 
  switch (timeClient.getDay()) 
  {
    case 0:
      day = "Sun";
      break;
    case 1:
      day = "Mon";
      break;
    case 2:
      day = "Tue";
      break;
    case 3:
      day = "Wed";
      break;
    case 4:
      day = "Thu";
      break;
    case 5:
      day = "Fri";
      break;
    case 6:
      day = "Sat";
      break;
  }
  return day;
}

bool checkBrightness()
{
  static bool brightness = lpmode;
  if (digitalRead(BUTTON) == true) 
  {
    brightness = !brightness; 
  }
  return brightness;
}

float readTemperature()
{
  static int counter;
  float temp = bme.readTemperature();
  static float tempAvg, returnTempAvg; 
  tempAvg = tempAvg + temp;
  counter++;
  if (counter >= 10)
  {
    returnTempAvg = tempAvg / 10;
    tempAvg = 0;
    counter = 0;
  }
  return returnTempAvg;
}

float readHumidity()
{
  static int counter;
  float hum = bme.readHumidity();
  static float humAvg, returnHumAvg; 
  humAvg = humAvg + hum;
  counter++;
  if (counter >= 10)
  {
    returnHumAvg = humAvg / 10;
    humAvg = 0;
    counter = 0;
  }
  return returnHumAvg;
}

void displayClock(const char *day, float battp, int wifiStatus, int hour, int minute, bool brightness, float temp, float hum)
{
  static bool blink;
  
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_spleen16x32_mn);
  if (hour < 10) 
  {
    u8g2.setCursor(24, 41);
    u8g2.print("0");
    u8g2.setCursor(40, 41);
    u8g2.print(hour);
  }
  else
  {
    u8g2.setCursor(24, 41);
    u8g2.print(hour);
  } 
  u8g2.setCursor(56, 41);
  switch (blink)
  { 
    case true: 
      u8g2.print(":");
      break;
  case false:
      u8g2.print("");
      break;
  }
  if (minute < 10) 
  {
    u8g2.setCursor(72, 41);
    u8g2.print("0");
    u8g2.setCursor(88, 41);
    u8g2.print(minute);
  }
  else 
  {
    u8g2.setCursor(72, 41);
    u8g2.print(minute);
  }
 
  u8g2.setFont(u8g2_font_7x14_mf);
  u8g2.setCursor(0, 60);
  u8g2.print(day);
  
  u8g2.setFont(u8g2_font_siji_t_6x10);
  switch (wifiStatus) 
  { 
    case 0: 
      u8g2.drawGlyph(0, 10, 57882);
      break;
    case 1:
      u8g2.drawGlyph(0, 10, 57881);
      break;
    case 2: 
      u8g2.drawGlyph(0, 10, 57880);
      break;
    case 3:
      u8g2.drawGlyph(0, 10, 57907);
      break;
    case 4: 
      u8g2.drawGlyph(0, 10, 57879);
      break;
  }
  
  if (battp < 99) 
  {
    if (battp >= 70) 
    {
      u8g2.drawGlyph(92, 10, 57487);
    }
    else if (battp >= 30) 
    {
      u8g2.drawGlyph(92, 10, 57486);
    } 
    else 
    {
      u8g2.drawGlyph(92, 10, 57485);
    } 
    u8g2.setCursor(104, 10);
    u8g2.print(battp, 0);
    u8g2.drawStr(116, 10, "%");
  }
  else 
  {
    u8g2.drawGlyph(116, 10, 57409);
  }
  
  u8g2.drawGlyph(44, 10, 57550);
  u8g2.setCursor(56, 10);
  u8g2.print(temp, 1);
  u8g2.drawGlyph(44, 60, 57549);
  u8g2.setCursor(56, 60);
  u8g2.print(hum, 1);
  
  switch (brightness) 
  {
    case false:
      u8g2.setContrast(255);
      u8g2.drawGlyph(114, 60, 57794);
      break;
    case true:
      u8g2.setContrast(50);
      u8g2.drawGlyph(114, 60, 57795);
      break;
  }
  
  blink = !blink;
  u8g2.sendBuffer();
}

void setup()
{
  pinMode(SUMMER, INPUT_PULLDOWN);
  pinMode(BUTTON, INPUT_PULLDOWN);
  analogReadResolution(12);
  Wire.setSCL(SCK);
  Wire.setSDA(SDA);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("pico-clock");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  Wire.begin();
  u8g2.begin();
  timeClient.begin();
  bme.begin(0x76);
  if (digitalRead(SUMMER) == 1) 
  {
    timeClient.setTimeOffset(10800); //UTC+3
  }
  else
  {
    timeClient.setTimeOffset(7200); //UTC+2
  }
  for (int i = 0; i < 10; i++)
  {
    getBatteryPrecentage();
    getWifiRSSI();
  }
}

void loop()
{
  bool disconnected;
  timeClient.update();
  displayClock(getDay(), getBatteryPrecentage(), getWifiStatus(), timeClient.getHours(), timeClient.getMinutes(), checkBrightness(), readTemperature(), readHumidity());
  sleep_ms(1000);
  if (lpmode == true && disconnected == false)
  {
    WiFi.disconnect();
    set_sys_clock_khz(20000, true);
    disconnected = true;
  } 
}