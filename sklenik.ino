
// DHT22 requihttps://github.com/adafruit/Adafruit_Sensor/releases
#include <DHT.h>
#include <DHT_U.h>

// clock
#include <Wire.h>
#include "RTClib.h"

// DHT22 Settings
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// clock Settings
//DS3231 rtc;
RTC_DS3231 rtc;

// outputs
const int PIN_OUT_LED = 6;
const int PIN_OUT_PUMP = 9;
const int PIN_OUT_FAN = 10;
const int PIN_OUT_HUM = 11;

const int MAX_FAN = 255;
const int MAX_PUMP = 255;

const int PUMP_SECOND_PHASE = 70;
const int FAN_SECOND_PHASE = 120;

const int HUM_THRESHOLD = 95;

// variables
DateTime dateTime;
float hum;
float temp;
int pump_seconds = 0;
int fan_seconds = 0;
boolean pump_started = false;


void setup()
{
  Serial.begin(9600);

  // init DHT
  dht.begin();

  // init clock
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2020, 3, 1, 4, 59, 20));
}

void loop()
{
  // read data from DHT22
  if (dateTime.second() == 0) {
    // measure once a minute
    hum = dht.readHumidity();
    temp = dht.readTemperature();
  }

  // read data from clock
  dateTime = rtc.now();

  // LIGHT Section
  if (dateTime.hour() >= 5 && dateTime.hour() <= 22) {
    analogWrite(PIN_OUT_LED, 255);
  } else {
    analogWrite(PIN_OUT_LED, 0);
  }

  // PUMP Section
  if (dateTime.second() == 0 && (dateTime.minute() == 5 || dateTime.minute() == 35)) {
    pump_seconds = PUMP_SECOND_PHASE;
  }

  if (pump_seconds > 0) {
    analogWrite(PIN_OUT_PUMP, MAX_PUMP);
    pump_seconds -= 1;
  } else {
    analogWrite(PIN_OUT_PUMP, 0);
  }

  // FAN and huminity Section
  // run every 15 minutes
  if (hum < HUM_THRESHOLD || !(dateTime.minute() % 15)) {
    fan_seconds = FAN_SECOND_PHASE;
  }

 if (fan_seconds > 0) {
    analogWrite(PIN_OUT_FAN, MAX_FAN);
    analogWrite(PIN_OUT_HUM, 255);
    fan_seconds -= 1;
  } else {
    // no ventilation time and huminity is ok, switch off the fan and huminizer
    analogWrite(PIN_OUT_HUM, 0);
    analogWrite(PIN_OUT_FAN, 0);
  }
  

  // vypíšeme informace po sériové lince
  Serial.print("Vlhkost: ");
  Serial.print(hum);
  Serial.print(" %, Teplota: ");
  Serial.print(temp);
  Serial.println(" Celsius");

  Serial.print("Datum a cas DS3231: ");
  Serial.print(dateTime.year());   Serial.print("-");
  Serial.print(dateTime.month());  Serial.print("-");
  Serial.print(dateTime.day());    Serial.print(" ");
  Serial.print(dateTime.hour());   Serial.print(":");
  Serial.print(dateTime.minute()); Serial.print(":");
  Serial.print(dateTime.second()); Serial.println("");


  delay(1000);
}

