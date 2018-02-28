#!/usr/bin/python3
import serial
import time
import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize
from scipy.interpolate import interp1d


#The following line is for serial over GPIO
port = '/dev/ttyACM0'

ard = serial.Serial(port,115200,timeout=5)
time.sleep(2) # wait for Arduino
i = 0
N = 500 #int(5/dt)
accel, time_arr = np.empty((N, 3)), np.empty(N)

def map_accel(rawg):
    a = 340
    alpha = 63
    return alpha**-1*(rawg - a)


m = interp1d([-2,2],[0,128])
t0 = time.time()
while i < N:
    # Serial write section

    if i != 0:
        print(accel[i-1])
        try:
            poses = [int(m(accel[i-1, 0])), int(m(accel[i-1, 1]))]
        except ValueError:
            print('paskaa')
    else: poses = [64, 64]
    print(poses)
    send = ''
    for pos in poses:
        send += '{0:03d}:'.format(pos)
    ard.write(send[:-1].encode('utf-8'))

    # Serial read section
    msg = ard.readline() #ard.inWaiting()) # read everything in the input buffer
    l = msg.decode("utf-8")

    print(l)

    try:
        d = np.array([float(val) for val in l.split(' ')])
    except ValueError as e:
        print(e)
        d = np.zeros(3)

    #print(np.sqrt(d[2]**2 + d[3]**2 + d[4]**2))
    accel[i,:] = map_accel(d)

    time_arr[i] = time.time() - t0
    time.sleep(.1)
    i += 1
else:
    print ("Exiting")


'''
def g(x, *args):
    alpha, a = x, 340
    return (np.sqrt(np.sum(alpha**-2*np.square(accel[0, -3:] - a))) - 1)**2

x0 = 60.
res = scipy.optimize.minimize(g, x0)

print(res)
alpha = res['x']

'''


print(time_arr[-1]/N)
print(np.linalg.norm(accel, axis = 1))


ylabs = ["gX", "gY", "gZ"]
for col, ylab in zip(range(3), ylabs):
    dat = accel[:,col]
    plt.plot(time_arr, dat)
    plt.title('Avg time per call = {}'.format(time_arr[-1]/N))
    plt.xlabel('Time [s]')
    plt.ylabel(ylab)
    plt.show()
