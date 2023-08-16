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
#include "battery.hpp"

int16_t *accelbuff;
int16_t *gyrobuff;

SFE_HMD_DRV2605L drv;
bool vibrate = true;
bool drv_initialised = false;
extern bool vibe_request;
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
            SerialBT.print("Unknown error at address 0x");
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
  
  bt_init();
  tftInit();
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
  gyrobuff=getGyro();

  if (SerialBT.connected())
  {
    SerialBT.printf("%6.6d, %6.6d, %6.6d, %6.6d, %6.6d, %6.6d\n\r", accelbuff[0], accelbuff[1],accelbuff[2], 
      gyrobuff[0], gyrobuff[1],gyrobuff[2]);
  }

//  Serial.printf("%6.6d, %6.6d, %6.6d, %6.6d, %6.6d, %6.6d\n\r", accelbuff[0], accelbuff[1],accelbuff[2], 
//      gyrobuff[0], gyrobuff[1],gyrobuff[2]);

  handleUi();
  updateBatteryChargeStatus();
  if (! isCharging());
    bt_loop();

  if (sqrt(accelbuff[0]*accelbuff[0]+ accelbuff[1]*accelbuff[1]+accelbuff[2]*accelbuff[2])>17500)
    vibrate = true;

  if (vibe_request== true)
    vibrate = true;
  
  // vibrate = false;
  if (vibrate == true)
  {
    // swa rfc - Tidy up the digital writes above this, once we know there's no need for a delay after EN.
    digitalWrite(14, HIGH); // Enable high
    drv.begin();
    SerialBT.print("==== Initialised DRV2605\n\r");
    drv.MotorSelect(0x0A);
    drv.Library(7); //change to 6 for LRA motors 

    SerialBT.printf("Vibrate\n");
    drv.Mode(0); // This takes the device out of sleep mode

    for (int i = 0; i < 100; i++)
    {
      drv.Waveform(1, 16);  
      drv.Waveform(2, 0);  
      drv.Waveform(3, 16);  
      drv.Waveform(4, 0);  
      drv.go();
    }
    vibrate = false;
  }
}
