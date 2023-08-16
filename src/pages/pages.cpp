#include <Arduino.h>
#include <EasyButton.h>
#include "pages.hpp"
#include "bt.hpp"

int8_t page = 0;
EasyButton tp_button(TP_PIN_PIN, 80, true, false);
uint32_t time_out = millis();
uint32_t max_time_out = 3600*1000;
bool handlingAction = false;
bool initialLoad = true;
bool vibe_request;

void initButton()
{
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button.begin();
  tp_button.onPressedFor(4000, handleAction);
  //this is my proposed way for the button press to work, but I've yet to test it
  tp_button.onPressed(activateVibe);
  page = 0;
  showPage();
}

void handleUi()
{

  tp_button.read();
  if (!handlingAction)
  {
    showPage();
  }

  if (SerialBT.connected() || isCharging() || handlingAction)
  {
  }
  else if ((millis() - time_out) > max_time_out)
  {
//    tftSleep(false);
//    deactivateWifi();

    handleSleep(false);
  }
}

void activateVibe()
{
  
  time_out = millis();
  vibe_request!= vibe_request;
  initialLoad = true;
}

void showPage()
{
  switch (page)
  {

  // max_time_out = 30000;

  // case 0:
  //   pageClock(initialLoad);
  //   break;
  // case 1:
  //   pageRtc(initialLoad);
    // break;
  case 0:
    pageBattery(initialLoad);
    break;
  case 1:
    pageOta(initialLoad);
    break;
  default:
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
  // case 0:
  //   actionClock();
  //   break;
  // case 1:
  //   actionClock();
  //   break;
  case 0:
    waitOta();
    break;
  case 1:
    waitOta();
    page = 0;
    break;
  }
  time_out = millis();
  handlingAction = false;
  initialLoad = true;
}
