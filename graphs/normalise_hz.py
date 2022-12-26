import matplotlib.pyplot as plt
import numpy as np

"""
Normalising Hz is usually done in graphical contexts.
Any margin of error will result in a pixel being offset
left or right by a pixel (or less!)
All the fastest log functions work really great here,
as their margin of error is very forgivable.
The two fastest ankerl and ekmett use the same calcs
they only differ in accuracy at different ranges.

The following table compares the speed and error margin of
different log approxiamtions used in the following formula:
    log(Hz / 20) / log(2) / 10
    log(Hz * 0.05) / (LN2 * 0.1)
    log(Hz * 0.05) * 0.14426950408889633

SEC      xSPEED ERROR  NAME
0.055610               pass
0.176747 1      0      stl
0.093552 3.19   bad    njuffa
0.081992 4.5    bad    logNPlusOne
0.075357 6.13   bad    njuffa_faster
0.073915 6.61   0.000  mineiro
0.067568 10.13  0.005  mineiro_faster
0.064840 13.12  0.006  ankerl32      - more accurate than ekmett around 0.5, less around 0 & 1
0.064824 13.14  0.01   ekmett_lb     - very precise around 0 & 1, worst around 0.5

- pass = no processing
- stl = log
- xSpeed of the log function calculated: (stl_sec - pass) / (other_sec - pass)
"""

class Styles:
    idx = 0
    types = ['solid', 'dotted', 'dashed', 'dashdot']

    def get(self):
        s = self.types[self.idx]
        self.idx = (self.idx + 1) & 3
        return s

linestyles = Styles()

fig, ax = plt.subplots()

x_labels = [       20,   50,
            100,   200,  500,
            1000,  2000, 5000,
            10000, 20000]
x_ticks = [i for i in range(len(x_labels))]

plt.xlabel('Frequency in Hz')
plt.xticks(ticks=x_ticks, labels=x_labels)

stl = [0.000000000000,0.132192820311,0.232192829251,0.332192838192,0.464385658503,0.564385652542,0.664385676384,0.796578466892,0.896578490734,0.996578514576,]
logNPlusOne = [0.100000001490,0.180733785033,0.258435070515,0.345058858395,0.460027545691,0.533334672451,0.586227059364,0.626697838306,0.642166435719,0.650339961052,]
njuffa = [0.000000000000,0.132192820311,0.232192829251,0.332192838192,0.464385658503,0.564385652542,0.664385676384,0.796578466892,0.896578490734,0.996578514576,]
njuffa_faster = [0.000000000000,0.132191956043,0.232191964984,0.332191973925,0.464386820793,0.564386844635,0.664386808872,0.796578526497,0.896578550339,0.996578574181,]
ankerl32 = [0.005798471160,0.130798473954,0.230798497796,0.330798506737,0.462048500776,0.562048494816,0.662048459053,0.801110982895,0.901111006737,1.001111030579,]
ekmett_lb = [-0.000000011921,0.125000000000,0.225000008941,0.325000047684,0.456250011921,0.556250035763,0.656250059605,0.795312523842,0.895312547684,0.995312571526,]
mineiro = [-0.000000238419,0.132186457515,0.232186481357,0.332186460495,0.464381873608,0.564381897449,0.664381921291,0.796571612358,0.896571695805,0.996571660042,]
mineiro_faster = [0.005731287878,0.130731031299,0.230730831623,0.330730646849,0.461981207132,0.561981022358,0.661980807781,0.801044046879,0.901043832302,1.001043677330,]


x = [i for i in range(len(stl))]

ax.plot(stl, label='stl', linestyle=linestyles.get())
# ax.plot(logNPlusOne, label='logNPlusOne', linestyle=linestyles.get())
# ax.plot(njuffa, label='njuffa', linestyle=linestyles.get())
# ax.plot(njuffa_faster, label='njuffa_faster', linestyle=linestyles.get())
ax.plot(ankerl32, label='ankerl32', linestyle=linestyles.get())
ax.plot(ekmett_lb, label='ekmett_lb', linestyle=linestyles.get())
ax.plot(mineiro, label='mineiro', linestyle=linestyles.get())
ax.plot(mineiro_faster, label='mineiro_faster', linestyle=linestyles.get())

ax.legend()
plt.show()
