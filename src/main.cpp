#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "wristband-tft.hpp"
#include "wristband-ota.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"
#include "bt.hpp"
#include "BluetoothSerial.h"

int16_t *accelbuff;

void setup() {

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  initClock();
  tftInit();
  bt_init();
  deactivateWifi();
  setupADC();
#ifndef IMU_SKIP
  initMPU();
#else
  mpuDeepSleep();
#endif
  initButton();
  setupBattery();
  }

void loop() 
{
  accelbuff=getAccel();

  if (SerialBT.connected())
  {
    SerialBT.printf("%6.6d, %6.6d, %6.6d\n", accelbuff[0], accelbuff[1],accelbuff[2]);
  }

  handleUi();
  updateBatteryChargeStatus();
  bt_loop();
}
