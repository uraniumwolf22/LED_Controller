import matplotlib.pyplot as plt
import math
from random import randint
from time import sleep

def compand(rgb, inverse=False):
    result = []
    for c in rgb:
        c = c / 255
        if inverse: c = (((c+0.055) / 1.055) ** 2.4) if (c > 0.04045) else (c / 12.92)
        else: c = ((1.055 * (c ** (1/2.4)))-0.055) if (c > 0.0031308) else (c * 12.92)
        result.append(c * 255)
    return result

list = [[500,214],[465,407],[431,601],[397,794],[362,988],[328,1181],[294,1375],[260,1568],[225,1762],[191,1955],[157,2149],[123,2342],[216,2485],[310,2627],[404,2770],[497,2912],[436,2763],[376,2614],[315,2465],[254,2316],[193,2167],[288,2297],[330,2143],[373,1990],[416,1837],[459,1684],[501,1531],[542,1683],[584,1834],[625,1986],[666,2137],[707,2288],[802,2173],[744,2314],[687,2455],[629,2596],[572,2737],[514,2879],[604,2744],[694,2609],[784,2474],[874,2340],[838,2133],[803,1926],[768,1719],[733,1512],[698,1305],[663,1098],[628,891],[593,684],[558,477],[522,270]]

colors = []
color1 = compand([255, 255, 0], inverse=True)
color2 = compand([255, 0, 255], inverse=True)
for i in list:
    min = 0
    max = 3000
    factor = i[1] / max # This is our gradient! Super simple in this scenario.

    result = [(color1[i] * (1-factor))+(color2[i] * factor) for i in range(3)]
    for i in range(3):
        #print(color1[i])
        #print(factor)
        #print(color2[i])
        #print(factor)
        #print("RESULT:")
        result[i] = color1[i] * (1 - factor) + color2[i] * factor
        #print(result[i])
        #print("--------------")
    colors.append(compand(result))

    # Graphing the points
x, y = [], []
for i in list:
    x.append(math.trunc(i[0] * 1000) / 1000)
    y.append(math.trunc(i[1] * 1000) / 1000)

for i in range(len(list)):
    plt.plot(x[i], y[i], linestyle='-', marker='o', c=tuple([i/255 for i in colors[i]]))

plt.show()