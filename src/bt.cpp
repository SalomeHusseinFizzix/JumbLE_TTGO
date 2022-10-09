#include <Wire.h>
#include <rom/rtc.h>
#include "clock.hpp"
#include "bt.hpp"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;

void bt_init(void)
{
  char name[200] = "";

  Serial.begin(115200);
  SerialBT.begin(name);

  /* 
   * Note that the BT MAC isn't unavailable until BT is initialised.
   * This happens in the above begin call, shich takes the device name.
   * So we have to update the device name once we get access to the MAC.
   */
  const uint8_t * mac = esp_bt_dev_get_address();
  snprintf(name, sizeof(name), "Jumble %2.2x%2.2x", mac[4], mac[5]);
  esp_bt_dev_set_device_name(name);
  Serial.printf("Bluetooth active. Device name is %s.", name);
}


void bt_loop() 
{
  // Do nothing (for now).
}

