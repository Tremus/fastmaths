import matplotlib.pyplot as plt
import numpy as np

"""
Denormalising Hz is often done in graphical contexts.
Errors will result in a pixel being offset a few places,
or just a fraction.
All the fastest pow functions will offset any on screen
value by a few pixels. On small displays this won't be
noticeable, but on large ones it could be.
The standard mineiro function is the only viable fast option
due to its accuracy. Unfortunately the speed improvements are
only minor.
The mineiro_faster can be a good option if the display is
small so as to minimise the effects of the errors.

The following table compares the speed and error margin of
different pow(2, x) approxiamtions used in the following formula:
    20 * pow(2, normalised * 10)

    (alternate) 5 * pow(2, 10 * normalised + 2)


SEC      xSPEED ERROR  NAME
0.055522               pass
0.081256 1      0      stl32
0.091158 0.72          ekmett_fast_precise
0.090583 0.73          ekmett_fast_better_precise
0.077771 1.15   0.15   mineiro            -- amazing at all ranges
0.063001 3.43   bad    ekmett_fast_ub
0.062901 3.47   bad    ekmett_fast
0.062841 3.5    1000   ekmett_fast_lb     -- error of 1000 close to 20kHz, 0.8Hz close to 20. Will result in a few pixels offset
0.061775 4.09   14     mineiro_faster     -- error of 750 close to 20kHz, 0.5Hz close to 20Hz. Will result in a couple pixels offset

- pass = no processing
- stl = std::pow(2, x)
- xSpeed of the pow2 function calculated: (stl_sec - pass) / (other_sec - pass)
- Error is in Hz
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

x_labels = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
x_ticks = [i for i in range(len(x_labels))]

plt.xlabel('Normalised Position')
plt.xticks(ticks=x_ticks, labels=x_labels)
plt.ylabel('Hz')

stl32 = [20.0,40.0,80.0,160.0,320.0,640.0,1280.0,2560.0,5120.0,10240.0,20480.0,]
ekmett_fast = [19.420166015625,40.0,84.6386718750,178.55468750,375.66406250,788.43750,1651.093750,3450.66406250,7198.2031250,14990.156250,31167.81250,]
ekmett_fast_lb = [19.139251708984,38.278503417969,76.557006835938,153.114013671875,306.228027343750,612.45605468750,1224.9121093750,2449.824218750,4899.64843750,9799.2968750,19598.593750,]
ekmett_fast_ub = [20.0,43.442993164062,93.771972656250,201.31347656250,430.17089843750,915.42968750,1941.035156250,4102.38281250,8645.468750,18172.343750,38107.50,]
ekmett_fast_precise = [20.0,40.0,80.0,160.0,320.0,640.0,1280.0,2560.0,5120.0,10240.0,20480.0,]
ekmett_fast_better_precise = [20.0,40.0,80.0,160.0,320.0,640.0,1280.0,2560.0,5120.0,10240.0,20480.0,]
mineiro = [20.0,40.0,80.0,160.0,320.0,640.0,1280.0,2559.980468750,5119.96093750,10239.9218750,20479.843750,]
mineiro_faster = [19.426956176758,38.853912353516,77.707519531250,155.41503906250,310.8300781250,621.660156250,1243.32031250,2486.6406250,4973.281250,9946.56250,19893.1250,]

x = [i for i in range(len(stl32))]

ax.plot(stl32, label='stl32', linestyle=linestyles.get())
# ax.plot(ekmett_fast, label='ekmett_fast', linestyle=linestyles.get())
# ax.plot(ekmett_fast_lb, label='ekmett_fast_lb', linestyle=linestyles.get())
# ax.plot(ekmett_fast_ub, label='ekmett_fast_ub', linestyle=linestyles.get())
# ax.plot(ekmett_fast_precise, label='ekmett_fast_precise', linestyle=linestyles.get())
# ax.plot(ekmett_fast_better_precise, label='ekmett_fast_better_precise', linestyle=linestyles.get())
ax.plot(mineiro, label='mineiro', linestyle=linestyles.get())
ax.plot(mineiro_faster, label='mineiro_faster', linestyle=linestyles.get())

ax.legend()
plt.show()
