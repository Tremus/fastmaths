import matplotlib.pyplot as plt
import numpy as np

"""
Paul Mineiro's log2 function is the clear winner in both
speed and accuracy. The margin of error when converting
Hz to midi is at worst 0.15ct.
Any of the faster functions are only faster by about
~7%, while also increasing the margin of error to 30ct

In applications where accuracy is not citical, the
hz_to_midi algorithm could be modified to reduce the
error caused by the log2 function.
eg 1. if there is a linear a negative offset, the
variable 69 could be incrased by a fraction to 69.3
eg 2. if there is in exponential offset, the
multiplication by 12 could be increased or decreased
by a small fraction.
The De Soras algorithm seems best suited for this.

The following table compares the speed and error margin of
different log2 approxiamtions used in the following formula:
    69 + log2(Hz / 440) * 12

SEC      xSPEED ERROR  NAME
0.055522               pass
0.175968 1      0      stl
0.095747 2.99   bad    log1_njuffa
0.078736 5.18   bad    newton
0.077620 5.44   bad    log1_njuffa_faster
0.076514 5.73   0.0015 log1_mineiro
0.075569 5.99   0.04   lgeoffroy_accurate
0.073967 6.52   0.075  lgeoffroy
0.072461 7.09   0.0015 mineiro
0.070592 7.97   0.3    log1_mineiro_faster
0.068189 9.48   0.3    log1_ankerl32
0.067995 9.63   0.3    mineiro_faster
0.067818 9.77   1.0    log1_ekmett_lb
0.066792 10.68  1.0    desoras
0.064067 14.04  bad    jcook


- pass = no processing
- stl = std::log2
- xSpeed of the log2 function calculated: (stl_sec - pass) / (other_sec - pass)
- Error is in semitones. I've recorded approximately the worst error
  between 1Hz (~-36midi) & 20Khz (~135midi) not the average error
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

x_labels = [1,     2,    5,
            10,    20,   50,
            100,   200,  500,
            1000,  2000, 5000,
            10000, 20000]
x_ticks = [i for i in range(len(x_labels))]

plt.xlabel('Frequency in Hz')
plt.xticks(ticks=x_ticks, labels=x_labels)
plt.ylabel('midi note number')

stl = [-36.3763122559,-24.3763122559,-8.51318359375,3.48681640625,15.4868202209,31.3499603271,43.3499603271,55.3499603271,71.2130966187,83.2130966187,95.2130966187,111.07623291,123.07623291,135.07623291,]
# log1_njuffa = [6107.62402344,6119.62402344,6135.48730469,6147.48730469,6159.48730469,6175.34960938,6187.34960938,6199.34960938,71.2130966187,83.2130966187,95.2130966187,111.07623291,123.07623291,135.07623291,]
# newton = [51.7406425476,51.7936248779,51.9525756836,52.2174835205,52.7473106384,54.3367881775,56.9859199524,62.2841796875,78.1789550781,84.9971389771,98.6334991455,126.017326355,188.243408203,324.415100098,]
# log1_njuffa_faster = [6107.62402344,6119.62402344,6135.48632812,6147.48632812,6159.48632812,6175.34960938,6187.34960938,6199.34960938,71.2132492065,83.2132415771,95.2132415771,111.076072693,123.076065063,135.076065063,]
log1_mineiro = [-36.3778152466,-24.3778076172,-8.51318359375,3.48681640625,15.4868164062,31.348274231,43.348274231,55.348274231,71.2114105225,83.2115020752,95.2115020752,111.076293945,123.076293945,135.076293945,]
lgeoffroy_accurate = [-36.4301376343,-24.4301376343,-8.51406860352,3.48593139648,15.4859313965,31.3960571289,43.3960571289,55.3960571289,71.1668243408,83.1668243408,95.1668243408,111.063247681,123.063247681,135.063247681,]
lgeoffroy = [-36.4340820312,-24.4340820312,-8.52168273926,3.47831726074,15.4783172607,31.4096183777,43.4096183777,55.4096183777,71.1641540527,83.1641540527,95.1641540527,111.055114746,123.055114746,135.055114746,]
mineiro = [-36.3778152466,-24.3778152466,-8.51318359375,3.48681640625,15.4868164062,31.348274231,43.348274231,55.348274231,71.2114105225,83.2115020752,95.2115020752,111.076293945,123.076293945,135.076293945,]
log1_mineiro_faster = [-36.3486099243,-24.3486404419,-8.85771179199,3.14225769043,15.1422348022,31.5059585571,43.5059318542,55.5059051514,71.3239974976,83.3241043091,95.3242111206,110.73323822,122.733215332,134.733184814,]
log1_ankerl32 = [-36.3405532837,-24.3405456543,-8.84963989258,3.15036010742,15.1503639221,31.513999939,43.513999939,55.513999939,71.3321762085,83.3321838379,95.3321838379,110.741271973,122.741271973,134.741271973,]
mineiro_faster = [-36.3487243652,-24.3487243652,-8.85781860352,3.14218139648,15.1421813965,31.505859375,43.505859375,55.505859375,71.3239746094,83.3240661621,95.3240661621,110.733123779,122.733123779,134.733123779,]
log1_ekmett_lb = [-37.0363616943,-25.0363616943,-9.54545593262,2.45454406738,14.4545440674,30.8181800842,42.8181800842,54.8181800842,70.6363601685,82.6363601685,94.6363601685,110.045455933,122.045455933,134.045455933,]
# jcook = [34.1881980896,34.3460693359,34.8154525757,35.5838394165,37.0705032349,41.1664161682,46.9815979004,55.886100769,71.2321548462,82.5996017456,91.3581237793,98.3134231567,101.022705078,102.464828491,]
desoras = [-37.036361694336,-25.036361694336,-9.545455932617,2.454544067383,14.454544067383,30.818183898926,42.818183898926,54.818183898926,70.636360168457,82.636367797852,94.636360168457,110.045455932617,122.045455932617,134.045455932617,]

x = [i for i in range(len(stl))]

ax.plot(stl, label='stl', linestyle=linestyles.get())
ax.plot(log1_mineiro, label='log1_mineiro', linestyle=linestyles.get())
ax.plot(lgeoffroy_accurate, label='lgeoffroy_accurate', linestyle=linestyles.get())
ax.plot(lgeoffroy, label='lgeoffroy', linestyle=linestyles.get())
ax.plot(mineiro, label='mineiro', linestyle=linestyles.get())
ax.plot(log1_mineiro_faster, label='log1_mineiro_faster', linestyle=linestyles.get())
ax.plot(log1_ankerl32, label='log1_ankerl32', linestyle=linestyles.get())
ax.plot(mineiro_faster, label='mineiro_faster', linestyle=linestyles.get())
ax.plot(log1_ekmett_lb, label='log1_ekmett_lb', linestyle=linestyles.get())
ax.plot(desoras, label='desoras', linestyle=linestyles.get())


ax.legend()

# plt.plot(x, y)  # Plot the chart
plt.show()  # display
