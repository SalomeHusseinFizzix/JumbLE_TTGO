import serial
import sys
import time
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime as dt

port = 'COM4'
if len(sys.argv) > 1:
    port = sys.argv[1]
    
lilygo_data = serial.Serial(port, 115200, timeout=1)
time.sleep(2)
if not lilygo_data.isOpen():
    lilygo_data.open()
print('com4 is open', lilygo_data.isOpen())
time.sleep(1)
x_data =[]
y_data=[]
z_data=[]

col_names = ['x_acc', 'y_acc', 'z_acc']
fn = f'data/IMUdata_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.csv'
fcsv = open(fn, 'w+')
fcsv.write(','.join(col_names) + '\n')

while True:
    b = lilygo_data.readline().strip()
    dataPacket=str(b,'utf-8')
    accel=dataPacket.split(',')
    #kludgy try-catch to ignore the print statement regarding bluetooth enabling
    if len(accel) == 3:
        fcsv.write(dataPacket + '\n')

fn.close()
lilygo_data.close()


