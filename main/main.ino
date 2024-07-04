// Made by KevinDev64
// GitHub: https://github.com/kevindev64

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <iarduino_RTC.h>
#include <string.h>

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////


// определение режима соединения и подключение библиотеки RemoteXY 
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// настройки соединения 
#define REMOTEXY_SERIAL_RX A4
#define REMOTEXY_SERIAL_TX A5
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_ACCESS_PASSWORD "ecosystem"


#include <RemoteXY.h>

// конфигурация интерфейса RemoteXY  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 550 bytes
  { 255,0,0,23,0,31,2,17,0,0,0,31,1,200,80,1,1,30,0,130,
  0,0,63,38,0,137,130,63,254,96,40,0,167,130,159,255,42,81,0,82,
  130,0,38,159,11,0,16,130,255,49,160,33,0,106,70,3,62,15,15,32,
  26,37,135,2,129,3,54,19,6,24,208,161,208,162,208,144,208,162,0,70,
  73,62,15,15,16,26,135,0,70,25,62,15,15,16,26,135,0,70,49,62,
  15,15,16,26,135,0,129,74,54,15,6,24,208,146,208,157,208,162,0,129,
  21,54,27,6,24,208,157,208,144,208,147,208,160,45,208,146,0,129,46,54,
  27,6,24,208,157,208,144,208,147,208,160,45,208,151,0,70,95,62,15,15,
  16,26,135,0,129,95,54,17,6,24,208,159,208,156,208,159,0,129,2,40,
  163,6,24,208,161,208,180,208,181,208,187,208,176,208,189,208,190,32,75,101,
  118,105,110,68,101,118,54,52,32,209,129,32,208,187,209,142,208,177,208,190,
  208,178,209,140,209,142,32,208,186,32,65,114,100,117,105,110,111,32,58,41,
  0,70,119,62,15,15,16,26,135,0,70,141,62,15,15,16,26,135,0,129,
  121,54,11,6,24,208,148,208,161,0,129,143,54,11,6,24,208,157,208,161,
  0,71,63,0,40,40,56,0,2,24,75,0,0,160,193,0,0,72,66,0,
  0,32,65,0,0,160,64,0,0,128,63,24,208,162,208,181,208,188,208,191,
  208,181,209,128,208,176,209,130,209,131,209,128,208,176,32,208,146,208,190,208,
  183,208,180,209,131,209,133,208,176,0,71,107,255,42,42,56,0,2,24,75,
  0,0,160,193,0,0,72,66,0,0,32,65,0,0,160,64,0,0,128,63,
  24,208,162,208,181,208,188,208,191,208,181,209,128,208,176,209,130,209,131,209,
  128,208,176,32,208,151,208,181,208,188,208,187,208,184,0,66,164,7,32,14,
  131,2,24,66,163,40,32,14,131,2,24,129,163,23,45,3,24,208,146,208,
  187,208,176,208,182,208,189,208,190,209,129,209,130,209,140,32,208,146,208,190,
  208,183,208,180,209,131,209,133,208,176,0,129,164,59,41,3,24,208,146,208,
  187,208,176,208,182,208,189,208,190,209,129,209,130,209,140,32,208,151,208,181,
  208,188,208,187,208,184,0,67,3,8,22,23,5,2,26,3,129,28,11,9,
  17,24,47,0,67,37,8,22,23,5,2,26,3,129,20,0,27,7,24,208,
  146,209,128,208,181,208,188,209,143,0 };
  
// структура определяет все переменные и события вашего интерфейса управления 
struct {

    // output variables
  uint8_t screen_status; // состояние светодиода, =0 отключен
  uint8_t screen_vent_led; // состояние светодиода, =0 отключен
  uint8_t screen_heat_air_led; // состояние светодиода, =0 отключен
  uint8_t screen_heat_ground_led; // состояние светодиода, =0 отключен
  uint8_t screen_water_led; // состояние светодиода, =0 отключен
  uint8_t screen_day_light_led; // состояние светодиода, =0 отключен
  uint8_t screen_night_light_led; // состояние светодиода, =0 отключен
  float screen_air_temp; // oт -20 до 50
  float screen_ground_temp; // oт -20 до 50
  int8_t screen_air_hum; // oт 0 до 100
  int8_t screen_ground_hum; // oт 0 до 100
  char screen_hours[3]; // =строка UTF8 оканчивающаяся нулем
  char screen_minutes[3]; // =строка UTF8 оканчивающаяся нулем

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define DHTPIN 11
#define ONE_WIRE_BUS 12
#define GROUND_HUM_PIN A0
#define GROUND_HUM_MAX 1028
#define GROUND_HUM_MIN 128
#define RTC_RST_PIN 8
#define RTC_CLK_PIN 9
#define RTC_DAT_PIN 10

// relays pins
#define DAY_LIGHT_RELAY_PIN 2
#define NIGHT_LIGHT_RELAY_PIN 3
#define AIR_HEAT_RELAY_PIN 4
#define GROUND_HEAT_RELAY_PIN 5
#define WATER_RELAY_PIN 6
#define VENT_RELAY_PIN 7

#define VENT_PERIOD 51200000 // every 12 hours
#define VENT_TIME 300000 // work 5 minutes

// threshold values
float air_temp_min = 25.0;
float air_temp_max = 30.0;
float air_temp_max_crit = 32.0;

float ground_temp_min = 25.0;
float ground_temp_max = 30.0;

float ground_hum_min = 30.0;
float ground_hum_max = 60.0;

// values now
int rtc_mins, rtc_hours;
float air_temp, ground_temp;
int air_hum, ground_hum;
uint64_t vent_timer;

// Impact Flags
bool night_light_flag, day_light_flag;
bool vent_flag;
bool water_flag;
bool air_heat_flag, ground_heat_flag;
bool air_cool_flag;
String system_status;

iarduino_RTC rtc(RTC_DS1302, RTC_RST_PIN, RTC_CLK_PIN, RTC_DAT_PIN);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ground_temp_sens(&oneWire);
DHT air_sens(DHTPIN, DHT11);
// LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() 
{ 
  rtc.begin();
  air_sens.begin();
  ground_temp_sens.begin();
  ground_temp_sens.setResolution(12);

  digitalWrite(LED_BUILTIN, LOW);

  for (int i = 2; i<= 7; i++){
    pinMode(i, OUTPUT);
  }

  // off all RELAY
  digitalWrite(DAY_LIGHT_RELAY_PIN, HIGH);
  digitalWrite(NIGHT_LIGHT_RELAY_PIN, HIGH);
  digitalWrite(AIR_HEAT_RELAY_PIN, HIGH);
  digitalWrite(GROUND_HEAT_RELAY_PIN, HIGH);
  digitalWrite(WATER_RELAY_PIN, HIGH);
  digitalWrite(VENT_RELAY_PIN, HIGH);
  delay(3000);

  // check all relays
  digitalWrite(DAY_LIGHT_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(DAY_LIGHT_RELAY_PIN, HIGH);
  delay(1500);

  digitalWrite(NIGHT_LIGHT_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(NIGHT_LIGHT_RELAY_PIN, HIGH);
  delay(1500);

  digitalWrite(AIR_HEAT_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(AIR_HEAT_RELAY_PIN, HIGH);
  delay(1500);

  digitalWrite(GROUND_HEAT_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(GROUND_HEAT_RELAY_PIN, HIGH);
  delay(1500);

  digitalWrite(WATER_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(WATER_RELAY_PIN, HIGH);
  delay(1500);

  digitalWrite(VENT_RELAY_PIN, LOW);
  delay(1500);
  digitalWrite(VENT_RELAY_PIN, HIGH);
  delay(2000);

  delay(1500);

  digitalWrite(13, HIGH);
  vent_timer = millis();
  RemoteXY_Init (); 
}

void loop() 
{
  RemoteXY_Handler ();
  RemoteXY_delay(1000);
  if (millis() - vent_timer >= VENT_PERIOD) {
    vent_timer = millis();
    // stop all program and start vent
    digitalWrite(VENT_RELAY_PIN, LOW);
    // clear lcd and print "Venting..." on it
    RemoteXY_delay(VENT_TIME); // turn off all & wait 5 minutes
    digitalWrite(VENT_RELAY_PIN, HIGH);
  }

  get_rtc_time();
  read_air_sens();
  read_ground_temp();
  read_ground_hum();

  check_state();
  apply_flags();
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
  if (day_light_flag == true)      RemoteXY.screen_day_light_led = 1;
  if (day_light_flag == false)     RemoteXY.screen_day_light_led = 0;
  
  if (night_light_flag == true)    RemoteXY.screen_night_light_led = 1;
  if (night_light_flag == false)   RemoteXY.screen_night_light_led = 0;

  if (air_heat_flag == true)       RemoteXY.screen_heat_air_led = 1;
  if (air_heat_flag == false)      RemoteXY.screen_heat_air_led = 0;

  if (ground_heat_flag == true)    RemoteXY.screen_heat_ground_led = 1;
  if (ground_heat_flag == false)   RemoteXY.screen_heat_ground_led = 0;

  if (air_cool_flag == true)       RemoteXY.screen_vent_led = 1;
  if (air_cool_flag == false)      RemoteXY.screen_vent_led = 0;

  if (water_flag == true)          RemoteXY.screen_water_led = 1;
  if (water_flag == false)         RemoteXY.screen_water_led = 0;

  sprintf (RemoteXY.screen_hours, rtc_hours);
  sprintf (RemoteXY.screen_minutes, rtc_mins);

  RemoteXY.screen_air_temp = air_temp;
  RemoteXY.screen_ground_temp = ground_temp;

  RemoteXY.screen_air_hum = air_hum;
  RemoteXY.screen_ground_hum = ground_hum;
}

void get_rtc_time() {
  rtc.gettime();
  rtc_hours = rtc.Hours;
  rtc_mins = rtc.minutes;
}

void check_state()
{
  if (rtc_hours >= 0 and rtc_hours < 3)                          { 
                                                                   day_light_flag = false;
                                                                   night_light_flag = false; }
  if (rtc_hours >= 3 and rtc_hours < 7)                          {
                                                                   day_light_flag = false;
                                                                   night_light_flag = true;  }
  if (rtc_hours >= 7 and rtc_hours < 18)                         {
                                                                   day_light_flag = true;
                                                                   night_light_flag = true;  }
  if (rtc_hours >= 18 and rtc_hours < 21)                        {
                                                                   day_light_flag = false;
                                                                   night_light_flag = true;  }
  if (rtc_hours >= 21)                                           {
                                                                   day_light_flag = false;
                                                                   night_light_flag = false; }

  if (air_temp <= air_temp_min and air_cool_flag == false)         air_heat_flag = true;
  if (air_heat_flag == true and air_temp >= air_temp_max)          air_heat_flag = false;
  if (air_temp >= air_temp_max_crit and air_heat_flag == true)     air_cool_flag = true;
  if (air_cool_flag == true and air_temp <= air_temp_max)          air_cool_flag = false;
  
  if (ground_temp <= ground_temp_min)                              ground_heat_flag = true;
  if (ground_heat_flag == true and ground_temp >= ground_temp_max) ground_heat_flag = false;

  if (ground_hum <= ground_hum_min)                                water_flag = true;
  if (water_flag == true and ground_hum >= ground_hum_max)         water_flag = false;
}

void apply_flags()
{
  if (day_light_flag == false)   digitalWrite(DAY_LIGHT_RELAY_PIN, HIGH);
  if (day_light_flag == true)    digitalWrite(DAY_LIGHT_RELAY_PIN, LOW);

  if (night_light_flag == false) digitalWrite(NIGHT_LIGHT_RELAY_PIN, HIGH);
  if (night_light_flag == true)  digitalWrite(NIGHT_LIGHT_RELAY_PIN, LOW);

  if (air_heat_flag == false)    digitalWrite(AIR_HEAT_RELAY_PIN, HIGH);
  if (air_heat_flag == true)     digitalWrite(AIR_HEAT_RELAY_PIN, LOW);

  if (air_cool_flag == false)    digitalWrite(VENT_RELAY_PIN, HIGH);
  if (air_cool_flag == true)     digitalWrite(VENT_RELAY_PIN, LOW);

  if (ground_heat_flag == false) digitalWrite(GROUND_HEAT_RELAY_PIN, HIGH);
  if (ground_heat_flag == true)  digitalWrite(GROUND_HEAT_RELAY_PIN, LOW);

  if (water_flag == false)       digitalWrite(WATER_RELAY_PIN, HIGH);
  if (water_flag == true)        digitalWrite(WATER_RELAY_PIN, LOW);

  if (air_heat_flag == true or air_cool_flag == true or ground_heat_flag == true or water_flag == true) RemoteXY.screen_status = 1;
  if (air_heat_flag == false and air_cool_flag == false and ground_heat_flag == false and water_flag == false) RemoteXY.screen_status = 2;
}
