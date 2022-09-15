#include <Wire.h>
#include <rom/rtc.h>
#include "clock.hpp"
#include "bt.hpp"

BluetoothSerial SerialBT;

void bt_init(void)
{
  Serial.begin(115200);
  SerialBT.begin("Jumble");
  Serial.println("Bluetooth active. Device name is Jumble");
}


void bt_loop() 
{
  // Do nothing (for now).
}

