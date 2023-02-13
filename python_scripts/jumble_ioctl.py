import logging
import msvcrt
import os.path
import serial
import sys
import time
from datetime import datetime as dt

getch = msvcrt.getch

logging.basicConfig(level=logging.DEBUG)

if len(sys.argv) > 1:
    port = sys.argv[1]
else:
    port = 'COM6'

if len(sys.argv) > 1:
    port = sys.argv[1]

lilygo_data = serial.Serial(port, 115200, timeout=1)

if not lilygo_data.isOpen():
    lilygo_data.open()

logging.info('Opened %s.' % (port))

deadline = time.time()
INTERVAL = 10

while True:
    if time.time() > deadline:
        deadline += INTERVAL
        print('Sending vibrate command')
        lilygo_data.write(b"v1\n")
        print('Done')

    b = lilygo_data.readline().strip()
    print(b)

