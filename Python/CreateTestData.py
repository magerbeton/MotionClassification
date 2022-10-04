import math
from matplotlib import pyplot
import pandas as pd


Scale = 0.2
dataLen = 60
OffsetY = 5


DataList1 = []
DataList2 = []

f = open("export.txt","a")
def MakeFunc(DatList, offset):
    for i in range(dataLen):
        #print(i)
        DatList.append(math.sin(i*Scale+offset))
        f.write(str(math.sin(i*Scale+offset)))
        f.write(",")
    
MakeFunc(DataList1,0)
MakeFunc(DataList2,2)

pyplot.plot(DataList2)
pyplot.plot(DataList1)
pyplot.show()

print(DataList1)

f.close()