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
#include "Sparkfun_DRV2605L.h"

int16_t *accelbuff;

SFE_HMD_DRV2605L drv;
bool vibrate = true;
bool drv_initialised = false;

void scanI2Cdevice(void)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire1.beginTransmission(addr);
        err = Wire1.endTransmission();
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
  Wire1.begin(DRV_SDA_PIN, DRV_SCL_PIN);
  Wire1.setClock(400000);
  initClock();
  tftInit();
  bt_init();
  // deactivateWifi();
  setupADC();
#ifndef IMU_SKIP
  initMPU();
#else
  mpuDeepSleep();
#endif
  initButton();
  setupBattery();

  // enable drv2605
  digitalWrite(14, HIGH); // Enable high

  scanI2Cdevice();


  Serial.printf("Finish setup\n");
}


void loop() 
{
  // enable drv2605
  digitalWrite(14, HIGH); // Enable high

  accelbuff=getAccel();

  if (SerialBT.connected())
  {
//    SerialBT.printf("%6.6d, %6.6d, %6.6d\n\r", accelbuff[0], accelbuff[1],accelbuff[2]);
  }

  handleUi();
  updateBatteryChargeStatus();
  bt_loop();

  if (true && vibrate == true)
  {
      drv_initialised = true;

      if (drv_initialised == false) {
            
            digitalWrite(14, HIGH); // Enable high

            if (!drv.begin()) {
              SerialBT.print("Could not find DRV2605\n\r");
        } else {
              drv.begin();
              SerialBT.print("==== Initialised DRV2605\n\r");
              drv.MotorSelect(0x0A);
              drv.Library(7); //change to 6 for LRA motors 

              drv_initialised = true;
            }
      }

      if (drv_initialised)
      {
        SerialBT.printf("Vibrate\n");
        drv.Mode(0); // This takes the device out of sleep mode
        drv.Waveform(1, 16);  
        drv.Waveform(2, 0);  
        drv.Waveform(3, 16);  
        drv.Waveform(4, 0);  
        drv.go();
        vibrate = false;
      }
    }
}
