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
#include "drv2605.h"

int16_t *accelbuff;

Adafruit_DRV2605 drv;
bool vibrate = false;

void scanI2Cdevice(void)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0) {
            SerialBT.print("I2C device found at address 0x");
            if (addr < 16)
                SerialBT.print("0");
            SerialBT.print(addr, HEX);
            SerialBT.println(" !");
            nDevices++;
        } else if (err == 4) {
            SerialBT.print("Unknon error at address 0x");
            if (addr < 16)
                SerialBT.print("0");
            SerialBT.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        SerialBT.println("No I2C devices found\n");
    else
        SerialBT.println("Done\n");
}

void setup() {

  Serial.begin(115200);
  Serial.printf("Begin setup\n");
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

  scanI2Cdevice();

  if (!drv.begin()) {
    SerialBT.println("Could not find DRV2605");
  } else {
    drv.begin(&Wire1);
    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);
  }
  Serial.printf("Finish setup\n");
}


void loop() 
{
  accelbuff=getAccel();

  if (SerialBT.connected())
  {
    SerialBT.printf("%6.6d, %6.6d, %6.6d\n\r", accelbuff[0], accelbuff[1],accelbuff[2]);
  }
  handleUi();
  updateBatteryChargeStatus();
  bt_loop();
  if (vibrate == true)
  {
    Serial.printf("Vibrate\n");
    drv.setWaveform(0, 7);  // play effect
    drv.setWaveform(1, 0);   // end waveform
    drv.go();
    vibrate = false;
  }
}
