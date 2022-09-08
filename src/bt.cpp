#include <Wire.h>
#include <rom/rtc.h>
#include "clock.hpp"
#include "bt.hpp"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void bt_init(void)
{
  Serial.begin(115200);
  SerialBT.begin("Jumble");
  Serial.println("The device started, now you can pair it with bluetooth!");
}


void bt_loop() 
{

  SerialBT.print("!");
}

