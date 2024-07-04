#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup() {
  // put your setup code here, to run once:
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.clear();

  lcd.print("LCD screen");
  lcd.setCursor(0, 1);
  lcd.print("diag");
  lcd.setCursor(0, 2);
  lcd.print("####################");
  lcd.setCursor(0, 3);
  lcd.print("0123456789 ");
  lcd.print("\x94");
  lcd.print(" xX");
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}
