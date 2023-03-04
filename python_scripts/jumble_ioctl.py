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
#setting up logging file for imu data
col_names = ['time', 'marker', 'acc_1', 'acc_2', 'acc_3','gyr_1','gyr_2','gyr_3']
fn = os.path.join('data', f'IMUdata_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.csv')
fcsv = open(fn, 'w+')

logging.info('Logging data to %s.' % (fn))

fcsv.write(','.join(col_names) + '\n')
class Marker:
    LogData   = 0
    IMUData = 1
    Keyboard  = 2

t0 = time.time()
nsamples = 0
print_ts = 0
deadline = time.time()
#frequency of vibe
INTERVAL = 0.3

while True:
    if time.time() > deadline:
        deadline += INTERVAL
        print('Sending vibrate command')
        lilygo_data.write(b"v1\n")
        print('Done')

    b = lilygo_data.readline().strip()
    t = time.time()
    dataPacket = str(b, 'utf-8')
    imu = dataPacket.split(',')
    if msvcrt.kbhit():
        c = str(getch(), 'utf-8')
        fields = [t, Marker.Keyboard, c]
        print()
        logging.info('Key "%s" logged @%fs.' % (c, t - t0))
        fcsv.write(','.join(str(f) for f in fields) + '\n')

    if len(imu) == 6:
        fail = False
        for i in range(len(imu)):
            try:
                imu[i] = int(imu[i])
            except:
                fail = True
        if not fail:
            fields = [t, Marker.IMUData] + imu
            fcsv.write(','.join(str(f) for f in fields) + '\n')
            nsamples += 1

            if t >= print_ts + 0.1:
                print('\r %3.1f: %d' % (t - t0, nsamples), end='')
                print_ts = t
            continue

    # Treat anything else as log data
    fields = [t, Marker.LogData, '"%s"' % (dataPacket)]
    fcsv.write(','.join(str(f) for f in fields) + '\n')
    print()
    logging.info(dataPacket)

fcsv.close()
lilygo_data.close()