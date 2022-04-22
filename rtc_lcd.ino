#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2); //adres i rozmiar wyswietlacza

const char *monthName[12] = 
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {

    lcd.setCursor(0,0);
    LCDprint2digits(tm.Hour);
    lcd.print(':');
    LCDprint2digits(tm.Minute);
    lcd.print(':');
    LCDprint2digits(tm.Second);
    
    lcd.setCursor(0,1);
    lcd.print(tm.Day);
    lcd.print('/');
    lcd.print(tm.Month);
    lcd.print('/');
    lcd.print(tmYearToCalendar(tm.Year));
 
  } else {
    if (RTC.chipPresent()) {
      lcd.print("DS1307 stopped- run the SetTime");
    } else {
      lcd.println("DS1307 read error");
      lcd.println();
    }
    delay(9000);
  }
  delay(1000);
}

void LCDprint2digits(int number) {
  if(number>=0 && number <10){
    lcd.write('0');
  }
  lcd.print(number);
}
