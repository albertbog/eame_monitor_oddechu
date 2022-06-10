#include <SoftwareSerial.h>

SoftwareSerial BLE_serial(11,10); // (RX,TX) - zweryfikować przy podłączaniu
boolean isDead = false;

void setup()
{
BLE_serial.begin(9600); // BLE
Serial.begin(9600);   // Serial Monitor - usunąć przy łączeniu kodu.
delay(100);
}
void loop()
{
  while(!Serial.available()) {} // dopóki ktoś nie wklepie czegoś do konsoli

  // Trup czy nie trup? (Jak zwraca co innego niż 0 to ma się dobrze)
  // Przy łączeniu kodu zastąpić np. tym samym warunkiem co do buzzera
  if(Serial.read() == '0')
  {
    isDead = true;
  }

  // Wrzucamy do BLE komunikat zależnie od wartości zmiennej.
  if(isDead == true)
  BLE_serial.write("0"); // Aplikacja ma identyczną notację
                         // czyli "0" - trup, gdy przesłane coś innego, to jest ignorowane.
  isDead = false;
}
