#include "pages/page-battery.hpp"

void pageBattery(bool initialLoad)
{
  if (initialLoad)
  {
    deactivateWifi();
    float voltage = getBattVoltage();
    drawBattery(voltage, calcPercentage(voltage), isCharging());
  }
}
