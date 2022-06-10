
#include "MPU9250.h"
#include <SPI.h>
#include <SD.h>

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


#define CS 10
#define BUZZER 2
#define TEN_SEC 1000
#define BACKLIGHT_PIN 3


// Create instance
File dataFile;
MPU9250 IMU(Wire,0x69);
uint16_t buzz_detect = 500;
tmElements_t tm;
uint16_t died_signal = 0;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2); //adres i rozmiar wyswietlacza

SoftwareSerial BLE_serial(8,7); // (RX,TX) - zweryfikować przy podłączaniu



const char *monthName[12] = 
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


void setup() { 
  pinMode(BUZZER, OUTPUT); // Set buzzer - pin 2 as an output
  
    Serial.begin(115200); // Start console
    BLE_serial.begin(115200);
    //SD card initialization
    if (SD.begin(CS)) {
 //   Serial.println("initialization done.");
      dataFile =SD.open("dataFile.txt",FILE_WRITE); //open file
      if(!dataFile){ 
//      Serial.println("Couldn't open file."); 
      } 
    }else{
        lcd.init();
        lcd.backlight(); 
        lcd.print("no card errror");
        while(1){}
    }

    IMU.begin(); // Set registers - Always required
    // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
//    imu.setBias(); // Initial calibration of gyro

    //Interrupts for buzzer

  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000001;        // no prescalar, just clk_IO 
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  /*4. Set the value of register A to 31250*/
  OCR1A = 31250;             //Finally we set compare register A to this value  
  sei();                     //Enable back the interrupts



  lcd.init();
  lcd.backlight();
}

void loop() { 
  // Update gyro calibration 
//  imu.updateBias();

  IMU.readSensor();
  RTC.read(tm);
   
  //-- Scaled and calibrated output:
  // Accel
dataFile.print(tm.Day); dataFile.print("."); dataFile.print(tm.Month); dataFile.print("."); dataFile.print(tmYearToCalendar(tm.Year)); dataFile.print("\t");
  dataFile.print(tm.Hour); dataFile.print(":"); dataFile.print(tm.Minute); dataFile.print(":"); dataFile.print(tm.Second); dataFile.print("\t");
  
  dataFile.print(IMU.getAccelX_mss(),6);dataFile.print("\t");
  dataFile.print(IMU.getAccelY_mss(),6);dataFile.print("\t");
  dataFile.print(IMU.getAccelZ_mss(),6);dataFile.print("\t");
  

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

  dataFile.print(IMU.getGyroX_rads());dataFile.print("\t");
  dataFile.print(IMU.getGyroY_rads());dataFile.print("\t");
  dataFile.print(IMU.getGyroZ_rads());dataFile.print("\t");
  dataFile.println(buzz_detect);



buzz_detect = ((IMU.getGyroX_rads() <0.001 || IMU.getGyroY_rads() <0.001 || IMU.getGyroZ_rads() <0.001) && buzz_detect <1000 ) ? buzz_detect+1 : 0+died_signal;

if(buzz_detect>950){ 

  digitalWrite(BUZZER, LOW);
  

}

//// Serial.print(tm.Day); Serial.print(tm.Month); Serial.print(tmYearToCalendar(tm.Year));Serial.print("\t");
////  Serial.print(tm.Hour); Serial.print(tm.Minute); Serial.print(tm.Second);Serial.print("\t");
//  
 Serial.print(IMU.getAccelX_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(),6);
   Serial.print("\t");
  Serial.println(buzz_detect);
  

//save,  close, open file again
  dataFile.flush();


if(buzz_detect==950){
  BLE_serial.write("0"); // Aplikacja ma identyczną notację
                         // czyli "0" - trup, gdy przesłane coś innego, to jest ignorowane.
  BLE_serial.end();

}


}



ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  if(buzz_detect > 900 && buzz_detect <950){  digitalWrite(BUZZER, HIGH); died_signal =1000; } // Send 10KHz sound signal to 2 pin buzzer
}



void LCDprint2digits(int number) {
  if(number>=0 && number <10){
    lcd.write('0');
  }
  lcd.print(number);
}
