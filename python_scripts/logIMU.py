import pandas as pd
import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from datetime import datetime as dt

lilygo_data =serial.Serial('COM4', 115200, timeout=1)
time.sleep(2)
if not lilygo_data.isOpen():
    lilygo_data.open()
print('com4 is open', lilygo_data.isOpen())
time.sleep(1)
x_data =[]
y_data=[]
z_data=[]
for i in np.arange(1,1200,1):
    b = lilygo_data.readline()
    dataPacket=str(b,'utf-8')
    accel=dataPacket.split(',')
    #kludgy try-catch to ignore the print statement regarding bluetooth enabling
    try:
        x_data.append(float(accel[0]))
        y_data.append(float(accel[1]))
        z_data.append(float(accel[2]))
    except:
        continue


lilygo_data.close()

imu_df=pd.DataFrame({'x_acc':x_data, 'y_acc':y_data, 'z_acc':z_data})
imu_df.to_csv(f'data/IMUdata_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.csv')

plt.figure(figsize=(10,5))
plt.plot(x_data)
plt.plot(y_data)
plt.plot(z_data)
plt.xlabel('Samples')
plt.ylabel('IMU Reading')
plt.title(f'IMU Reading vs. Time, {dt.now().strftime("%D %H:%M:%S")}')
plt.show()
plt.savefig(f'figures/IMUplot_{dt.now().strftime("%d%m%Y")}_{dt.now().strftime("%H%M%S")}.png')