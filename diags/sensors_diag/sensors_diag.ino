#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

#define DHTPIN 11
#define ONE_WIRE_BUS 12
#define GROUND_HUM_PIN A0
#define GROUND_HUM_MAX 255
#define GROUND_HUM_MIN 420

float air_temp, ground_temp;
int air_hum, ground_hum;
int raw_ground_hum;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ground_temp_sens(&oneWire);
DHT air_sens(DHTPIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();

  Serial.begin(9600);
  air_sens.begin();
  ground_temp_sens.begin();
  ground_temp_sens.setResolution(12);

  digitalWrite(LED_BUILTIN, LOW);

  lcd.print("ECOSYSTEM DIAG KIT");
  lcd.setCursor(0, 1);
  lcd.print("v. 0.1");
  lcd.setCursor(0, 2);
  lcd.print("### Sensors Diag ###");
  lcd.setCursor(0, 3);
  lcd.print("Made by KevinDev64 \x94");
  delay(3000);
}

void loop() {
  read_air_sens();
  read_ground_temp();
  read_ground_hum();

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
  raw_ground_hum = analogRead(GROUND_HUM_PIN);
  ground_hum = map(raw_ground_hum, GROUND_HUM_MIN, GROUND_HUM_MAX, 0, 100);
}

void print_info() {
  lcd.clear();
  lcd.print("Sensors Diag -------");
  lcd.setCursor(0, 1);
  lcd.print("A t/h ");
  lcd.print(String(air_temp));
  lcd.print("/");
  lcd.print(String(air_hum));
  lcd.setCursor(0, 2);
  lcd.print("G r/m ");
  lcd.print(String(raw_ground_hum));
  lcd.print("/");
  lcd.print(String(ground_hum));
  lcd.setCursor(0, 3);
  lcd.print("G t ");
  lcd.print(String(ground_temp));
  delay(3000);
}
