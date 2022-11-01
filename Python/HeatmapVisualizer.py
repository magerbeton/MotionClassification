import numpy as np
import seaborn
import matplotlib.pyplot as plt

f = open("Data.txt", "rt")
def ReadData():
    global f
    Arr = f.readline()

    StrArr = Arr.split(",")
    IntArr = [int(x) for x in StrArr]

    return IntArr

#60x60
Perfect = ReadData()
SpacialOffset = ReadData()
DirectionalOffset = ReadData()

#32x32
TestValue = ReadData()
ExponentialValues = ReadData()
ExponentailVals = ReadData()
ExponentailValsInverted = ReadData()

newArr = np.reshape(ExponentailVals, (32,32))

print(newArr)

seaborn.heatmap(newArr, vmin=0,vmax=200)
plt.show()

Values = list()

for i in range(newArr.shape[0]):
    if(i>0):
        Values.append(newArr[31][i])

SmallestValueRight = Values[Values.index(min(Values))]

print("Smallest value right:"+str(SmallestValueRight))        

Values = list()
for i in range(newArr.shape[0]):
    if(i>0):
        Values.append(newArr[i][31])

smallestValueBottom = Values[Values.index(min(Values))]

print("Smallest value bottom:"+str(smallestValueBottom))

if(smallestValueBottom<SmallestValueRight):
    print(smallestValueBottom)
else:
    print(SmallestValueRight)


#print(str(SmallestValueRight)+","+str(Values.index(SmallestValueRight)))