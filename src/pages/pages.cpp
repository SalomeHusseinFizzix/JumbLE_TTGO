#include <Arduino.h>
#include <EasyButton.h>
#include "pages.hpp"
#include "bt.hpp"

int8_t page = 0;
EasyButton tp_button(TP_PIN_PIN, 80, true, false);
uint32_t time_out = millis();
uint32_t max_time_out = 15000;
bool handlingAction = false;
bool initialLoad = true;

void initButton()
{
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button.begin();
  tp_button.onPressedFor(1000, handleAction);
  tp_button.onPressed(increasePage);
  page = 0;
  showPage();
}

void handleUi()
{

  if (SerialBT.connected() || isCharging() || handlingAction)
  {
    tp_button.read();
    if (!handlingAction)
    {
      showPage();
    }
  }
  else if ((millis() - time_out) > max_time_out)
  {
//    tftSleep(false);
//    deactivateWifi();

    handleSleep(false);
  }
}

void increasePage()
{
  time_out = millis();
  page++;
  initialLoad = true;
}

void showPage()
{
  switch (page)
  {
  case 0:
    max_time_out = 30000;
    pageClock(initialLoad);
    break;
  case 1:
    max_time_out = 30000;
    pageRtc(initialLoad);
    break;
  case 2:
    max_time_out = 30000;
    pageBattery(initialLoad);
    break;
  case 3:
#ifndef IMU_SKIP
    max_time_out = 60000;
    pageBearing(initialLoad);
    break;
#else
    page++;
#endif
  case 4:
#ifndef IMU_SKIP
    max_time_out = 30000;
    pageTemperature(initialLoad);
    break;
#else
    page++;
#endif
  case 5:
    max_time_out = 30000;
    pageOta(initialLoad);
    break;
  case 6:
    handleSleep();
    break;
  }
  initialLoad = false;
}

void handleAction()
{
  handlingAction = true;
  time_out = millis();
  switch (page)
  {
  case 0:
    actionClock();
    break;
  case 1:
    actionClock();
    break;
  case 2:
    waitOta();
    break;
  case 3:
#ifndef IMU_SKIP
    actionBearing();
#endif
    break;
  case 5:
    waitOta();
    page = 0;
    break;
  }
  time_out = millis();
  handlingAction = false;
  initialLoad = true;
}
