import logging
import msvcrt
import os.path
import serial
import sys
import time
from datetime import datetime as dt

getch = msvcrt.getch

logging.basicConfig(level=logging.DEBUG)

port = 'COM6'

if len(sys.argv) > 1:
    port = sys.argv[1]

lilygo_data = serial.Serial(port, 115200, timeout=1)
#time.sleep(2)

if not lilygo_data.isOpen():
    lilygo_data.open()

logging.info('Opened %s.' % (port))

#time.sleep(1)

col_names = ['time', 'marker', 'data_1', 'data_2', 'data_3']
fn = os.path.join('data', f'IMUdata_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.csv')
fcsv = open(fn, 'w+')

logging.info('Logging data to %s.' % (fn))

fcsv.write(','.join(col_names) + '\n')

class Marker:
    AccelData = 1
    Keyboard = 2

t0 = time.time()
nsamples = 0
print_ts = 0

while True:
        b = lilygo_data.readline().strip()
        t = time.time()
        dataPacket=str(b, 'utf-8')
        accel=dataPacket.split(',')

        if msvcrt.kbhit():
            c = str(getch(), 'utf-8')
            fields = [t, Marker.Keyboard, c]
            print()
            logging.info('Key "%s" logged @%fs.' % (c, t-t0))
            fcsv.write(','.join(str(f) for f in fields) + '\n')

        if len(accel) == 3:
            fields = [t, Marker.AccelData] + accel
            fcsv.write(','.join(str(f) for f in fields) + '\n')
            nsamples += 1

            if t >= print_ts + 0.1:
                print('\r %3.1f: %d' % (t-t0, nsamples), end='')
                print_ts = t

fcsv.close()

lilygo_data.close()

