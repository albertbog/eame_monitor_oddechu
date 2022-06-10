/*
 Get scaled and calibrated output of MPU6050
 */

#include <basicMPU6050.h> 
#include <SPI.h>
#include <SD.h>


#define CS 10
#define BUZZER 4
#define TEN_SEC 1000

// Create instance
File dataFile;
basicMPU6050<> imu;
uint16_t buzz_detect = 0;


void setup() { 
  pinMode(BUZZER, OUTPUT); // Set buzzer - pin 9 as an output
  
    Serial.begin(115200); // Start console
    //SD card initialization
   // if (SD.begin(CS)) {
   // Serial.println("initialization done.");
    //  dataFile =SD.open("dataFile.txt",FILE_WRITE); //open file
     // if(!dataFile){ 
//      Serial.println("Couldn't open file."); 
      } 
//    }else{
//      Serial.println("initialization failed!");
 //     while (1){tone(BUZZER, 1000);};
 //   }

    imu.setup(); // Set registers - Always required
    imu.setBias(); // Initial calibration of gyro

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
}

void loop() { 
  // Update gyro calibration 
  imu.updateBias();
  
   
  //-- Scaled and calibrated output:
  // Accel
 // dataFile.print(imu.ax());dataFile.print("\t");
  //dataFile.print(imu.ay() );dataFile.print("\t");
  //dataFile.print(imu.az());dataFile.print("\t");

  
  // Gyro
  //dataFile.print(imu.gx());dataFile.print("\t");
  //dataFile.print(imu.gy());dataFile.print("\t");
  //dataFile.println(imu.gz());

 buzz_detect = (imu.gx() <0.01 && imu.gy() <0.01 && imu.gz() <0.01 && buzz_detect <1024 ) ? buzz_detect+1 : 0;
  
  Serial.print(imu.ax());Serial.print("\t");
  Serial.print(imu.ay() );Serial.print("\t");
  Serial.print(imu.az());Serial.print("\t");

  
  // Gyro
  Serial.print(imu.gx());Serial.print("\t");
  Serial.print(imu.gy());Serial.print("\t");
  Serial.print(imu.gz());;Serial.print("\t"); Serial.println(buzz_detect);
  

//save,  close, open file again
//  dataFile.flush();
}



ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  if(buzz_detect > TEN_SEC) tone(BUZZER, 10000); // Send 10KHz sound signal to 4 pin buzzer
}
