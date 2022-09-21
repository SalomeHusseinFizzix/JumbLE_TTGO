import logging
import serial
import sys
import time
from datetime import datetime as dt
import pygame as pg

port = 'COM6'

if len(sys.argv) > 1:
    port = sys.argv[1]
    
lilygo_data = serial.Serial(port, 115200, timeout=1)
time.sleep(2)
if not lilygo_data.isOpen():
    lilygo_data.open()
print('%s is open' % (port), lilygo_data.isOpen())

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

logging.info('Loging to %s.' % (fn))

fcsv.write(','.join(col_names) + '\n')

class Marker:
    AccelData = 1
    Keyboard = 2

pg.init()

while True:
        b = lilygo_data.readline().strip()
        t = time.time()
        dataPacket=str(b,'utf-8')
        accel=dataPacket.split(',')

#        for ev in pg.event.get():
#            if ev.type == pg.KEYDOWN:
#                print('key;)')

#        pg.event.pump()
#        for event in pg.event.get():
#            print('e')

        keys = pg.key.get_pressed()
        
        if True in keys:
#            name = pg.key.name()
            print('k')
            fields = [t, Marker.Keyboard, 'a']
            fcsv.write(','.join(fields) + '\n')

        if len(accel) == 3:
#            print('.')
            fields = [t, Marker.AccelData] + accel
            fcsv.write(','.join(str(f) for f in fields) + '\n')

#    except Exception as e:
#        print(f"KeyboardInterrupt or {e}")
#        break


fcsv.close()

lilygo_data.close()

