#include <TimeLib.h>

/*
 Get scaled and calibrated output of MPU6050
 */

#include <basicMPU6050.h> 
#include <SPI.h>
#include <SD.h>



#define CS 10

// Create instance
File dataFile;
basicMPU6050<> imu;
static counter = 0;

void setup() { 
  pinMode(4, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(13, OUTPUT); //set CS of SPI on pin 13

     Serial.begin(38400); // Start console
    //SD card initialization
    if (!SD.begin(CS)) {
      Serial.println("initialization failed!");
      while (1);
    }Serial.println("initialization done.");
     
    imu.setup(); // Set registers - Always required
    imu.setBias(); // Initial calibration of gyro
   
  
   
    dataFile =SD.open("dataFile.txt",FILE_WRITE); //open file
    if(!dataFile){ Serial.println("Couldn't open file."); while (1);}

}

void loop() { 
  // Update gyro calibration 
  imu.updateBias();
  
   digitalWrite(13, 1); 
   
  //-- Scaled and calibrated output:
  // Accel
  dataFile.print(imu.ax());Serial.print("\t");
  dataFile.print(imu.ay() );Serial.print("\t");
  dataFile.println(imu.az());Serial.print("\t");

  
  // Gyro
  dataFile.print(imu.gx());Serial.print("\t");
  dataFile.print(imu.gy());Serial.print("\t");
  dataFile.println( imu.gz());

//save,  close, open file again
  dataFile.flush();
}




void loop(){
 
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
  
}
