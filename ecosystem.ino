// Made by KevinDev64
// GitHub: https://github.com/kevindev64

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define DHTPIN 4
#define ONE_WIRE_BUS 5
#define GROUND_HUM_PIN A6
#define GROUND_HUM_MAX 1028
#define GROUND_HUM_MIN 128

float air_temp, air_hum, ground_temp;
int ground_hum;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ground_temp_sens(&oneWire);
DHT air_sens(DHTPIN, DHT11);

void setup() 
{
  air_sens.begin();
  Serial.begin(9600);
  ground_temp_sens.begin();
  ground_temp_sens.setResolution(12);
}

void loop() 
{
  delay(2000);
  read_air_sens();
  read_ground_temp();
  read_ground_hum();
  // do other tasks
  print_info();
}

void read_air_sens()
{
  air_temp = air_sens.readTemperature();
  air_hum = air_sens.readHumidity();
}

void read_ground_temp()
{
  ground_temp_sens.requestTemperatures();
  ground_temp = ground_temp_sens.getTempCByIndex(0);
}

void read_ground_hum()
{
  int raw_ground_hum;
  raw_ground_hum = analogRead(GROUND_HUM_PIN);
  ground_hum = map(raw_ground_hum, GROUND_HUM_MIN, GROUND_HUM_MAX, 0, 100);
}

void print_info()
{
  // TODO: info on lcd screen
}