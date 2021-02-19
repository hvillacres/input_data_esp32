from pyArduinoPlot import *

maxSample = 100
sampleTime = 100 #tiempo de muestreo, este s tambien se encuentra en el arduino (dispositivo)
sizeData = 6
legendLabel = ['ax','ay','az','gx','gy','gz']
xLimit = [(0,maxSample)]*sizeData
yLimit = [(-20,20),(-20,20),(-20,20),(-300,300),(-300,300),(-300,300)] #limites de la señal a medir
style = ['r-','g-','b-','r-','g-','b-'] #colores de las graficas
anim = []

port = 'COM3' #Puerto en el cual se encuentra el dispositivo
baudRate = 115200

arduino = realTimePlot(port,baudRate,maxSample,sizeData)
arduino.readSerialStart()

for i in range(sizeData):
     fig, ax = makeFigure(xLimit[i], yLimit[i],'Data'+str(i+1))
     lines = ax.plot([],[],style[i],label=legendLabel[i])[0]
     legendValue = ax.text(0.50,0.90,'',transform=ax.transAxes)
     anim.append(animation.FuncAnimation(fig,arduino.getSerialData,fargs = (lines,legendValue,legendLabel[i],i),interval = sampleTime))
     plt.legend(loc="upper left")
     plt.grid()

plt.show()

arduino.close()
