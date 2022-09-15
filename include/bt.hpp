#ifndef __BT_HPP
#define __BT_HPP

#include "BluetoothSerial.h"

extern BluetoothSerial SerialBT;

void bt_init(void);
void bt_loop(void);

#endif /* __BT_HPP */
