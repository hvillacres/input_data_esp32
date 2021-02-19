"""▬▬▬▬▬▬▬▬▬ SÍGUEME ROBOTICOSS ▬▬▬▬▬▬▬▬▬▬
📰 Sitio web: https://roboticoss.com/ 
📱Instagram: https://bit.ly/3k7izQD 
📱Facebook: https://bit.ly/37d84ro 
📹 YouTube: https://bit.ly/2Hda3ks 
👷 LinkedIn: http://bit.ly/32oxHzw
"""
from threading import Thread
import serial
import time
import collections
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import copy
import sys
import numpy as np
 
class realTimePlot:
    def __init__(self, serialPort, serialBaud=9600, plotLength=100,numPlots=1):
        self.rawData = []
        self.collectData = []
        self.isCollect = False
        self.port = serialPort
        self.baud = serialBaud
        self.plotMaxLength = plotLength
        self.numPlots = numPlots
        self.data = []
        for i in range(numPlots):
            self.data.append(collections.deque([0] * plotLength, maxlen=plotLength))
            self.rawData.append(0)
            
        self.isRun = True
        self.isReceiving = False
        self.thread = None
        print('Trying to connect to: ' + str(serialPort))
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud)
            print('Connected to ' + str(serialPort))
        except:
            print("Failed to connect with " + str(serialPort))
            sys.exit(1)
            
    def readSerialStart(self):
        if self.thread == None:
            self.thread = Thread(target=self.backgroundThread)
            self.thread.start()
            while self.isReceiving != True:
                print("Starting receive data")
                time.sleep(0.1)
 
    def getSerialData(self,frame,lines,lineValueText,lineLabel, pltNumber):
        self.data[pltNumber].append(self.rawData[pltNumber])
        lines.set_data(range(self.plotMaxLength), self.data[pltNumber])
        lineValueText.set_text(lineLabel + ' = ' + str(self.rawData[pltNumber]))
 
    def backgroundThread(self):
        time.sleep(1.0)
        self.serialConnection.reset_input_buffer()
        while (self.isRun):
             for i in range(self.numPlots):
                  self.rawData[i] = float(self.serialConnection.readline().strip())  #Leer sensor / Read sensor
             if self.isCollect:
                privateData = copy.deepcopy(self.rawData)
                self.collectData.append(privateData)
             self.isReceiving = True
             
    def startCollectData(self):
         self.isCollect = True
         print("Collecting data")
         
    def stopCollectData(self):
         self.isCollect = False
         print("Collected data")
         
    def deleteData(self):
         del self.collectData[:]
         print("Deleted data")
         
    def saveData(self,Filename = 'Dataset'):
        print("Saving data")
        with open(Filename+'.npy', 'wb') as f:
            np.save(f,self.collectData)
        print("Saved data")
                   
    def close(self):
        self.isRun = False
        self.thread.join()
        self.serialConnection.close()
        print('Disconnected...')

def makeFigure(xLimit, yLimit, title="Data"):
    xmin, xmax = xLimit
    ymin, ymax = yLimit
    fig = plt.figure()
    ax = plt.axes(xlim=(xmin, xmax), ylim=(int(ymin - (ymax - ymin) / 10), int(ymax + (ymax - ymin) / 10)))
    ax.set_title(title)
    ax.set_xlabel("Samples")
    ax.set_ylabel("Output")
    return fig, ax
 
 

 
 
