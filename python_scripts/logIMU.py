import serial
import sys
import time
from datetime import datetime as dt
from pynput import keyboard
import pygame as pg
port = 'COM6'
# if len(sys.argv) > 1:
#     port = sys.argv[1]
    
lilygo_data = serial.Serial(port, 115200, timeout=1)
time.sleep(2)
if not lilygo_data.isOpen():
    lilygo_data.open()
print('com6 is open', lilygo_data.isOpen())
time.sleep(1)

## functions from pynput documentation example, creates a Listener object that
## needs care regarding its blocking
# def on_press(key):
#     try:
#         print('alphanumeric key {0} pressed'.format(
#             key.char))
#     except AttributeError:
#         print('special key {0} pressed'.format(
#             key))
#
# def on_release(key):
#     print('{0} released'.format(
#         key))
#     if key == keyboard.Key.esc:
#         # Stop listener
#         return False

col_names = ['x_acc', 'y_acc', 'z_acc', 'label']
fn = f'data/IMUdata_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.csv'
fcsv = open(fn, 'w+')
fcsv.write(','.join(col_names) + '\n')


while True:
    try:
        b = lilygo_data.readline().strip()
        dataPacket=str(b,'utf-8')
        accel=dataPacket.split(',')
        print(accel)

        if pg.key.get_focused == True:
            name = pg.key.name()
            print(name)
        else:
            name = 'nan'
        if len(accel) == 3:
            fcsv.write(f'{dataPacket}, {name}' + '\n')
    except Exception as e:
        print(f"KeyboardInterrupt or {e}")
        break


fcsv.close()

lilygo_data.close()

