#include "battery.hpp"

int vref = 1100;
bool charging = false;
float vbus = -1.0f;
float vbatt = -1.0f;

void setupADC()
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
  {
    vref = adc_chars.vref;
  }
}

void setupBattery()
{
  pinMode(CHARGE_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

float getBattVoltage()
{
    return vbatt;
}

float getVBusVoltage()
{
    return vbus;
}

uint8_t calcPercentage(float volts)
{
  float percentage = (volts - BATTERY_MIN_V) * 100 / (BATTERY_MAX_V - BATTERY_MIN_V);
  if (percentage > 100)
  {
    percentage = 100;
  }
  if (percentage < 0)
  {
    percentage = 0;
  }
  return (uint8_t) percentage;
}

void updateBatteryChargeStatus()
{
  uint16_t v;
  
  v = analogRead(BATT_ADC_PIN);
  vbatt = ((float) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);

  v = analogRead(VBUS_PIN);
  vbus = ((float) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);

  charging = !digitalRead(CHARGE_PIN);

  printf("vbatt: %3.3f, vbus: %3.3f, charge: %d", vbatt, vbus, isCharging());
  digitalWrite(LED_PIN, isCharging());
}

bool isCharging()
{
  return charging;
}
