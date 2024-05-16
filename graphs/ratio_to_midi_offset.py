import matplotlib.pyplot as plt
import numpy as np

"""
Paul Mineiro's log2 function is the clear winner in both
speed and accuracy. The margin of error when converting
Hz to midi is at worst 0.15ct.
Any of the faster functions are only faster by about
~7%, while also increasing the margin of error to 30ct

The following table compares the speed and error margin of
different log2 approxiamtions used in the following formula:
    log2(ratio) * 12

SEC      xSPEED ERROR  NAME
0.055522               pass
0.175968 1      0      stl
0.075569 5.99   0.06   lgeoffroy_accurate
0.073967 6.52   0.07   lgeoffroy
0.072461 7.09   0.0015 mineiro
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

x_labels = [0.125, 0.25, 0.5,
        1,  2,  3,  4,  5,  6,  7,  8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19,
        21, 22, 23, 24, 25, 26, 27, 28, 29,
        31, 32, 33, 34, 35, 36, 37, 38, 39,
        41, 42, 43, 44, 45, 46, 47, 48.0]
x_ticks = [i for i in range(len(x_labels))]

plt.xlabel('Ratio')
plt.xticks(ticks=x_ticks, labels=x_labels)
plt.ylabel('midi offset')

stl = [-36,-24,-12,0,12,19.019550323486,24,27.863136291504,31.019550323486,33.688259124756,36,38.039100646973,39.863136291504,41.513179779053,43.019550323486,44.405277252197,45.688259124756,46.882686614990,48,49.049552917480,50.039100646973,50.975131988525,52.707809448242,53.513179779053,54.282745361328,55.019546508789,55.726272583008,56.405281066895,57.058650970459,57.688259124756,58.295776367188,59.450355529785,60,60.532730102539,61.049552917480,61.551395416260,62.039100646973,62.513442993164,62.975131988525,63.424827575684,64.290626525879,64.707809448242,65.115173339844,65.513183593750,65.902236938477,66.282745361328,66.655067443848,67.019546508789,]
lgeoffroy = [-35.934555053711,-23.934556961060,-11.934556961060,0.065443038940,12.065443038940,19.028011322021,24.065443038940,27.798599243164,31.028011322021,33.753677368164,36.065444946289,37.994987487793,39.798599243164,41.476272583008,43.028011322021,44.453811645508,45.753677368164,46.927612304688,48.065444946289,49.045955657959,49.994991302490,50.912532806396,52.653179168701,53.476272583008,54.267883300781,55.028011322021,55.756656646729,56.453811645508,57.119487762451,57.753677368164,58.356388092041,59.467353820801,60.065444946289,60.559638977051,61.045955657959,61.524410247803,61.994991302490,62.457698822021,62.912532806396,63.359504699707,64.229820251465,64.653182983398,65.068664550781,65.476272583008,65.876014709473,66.267883300781,66.651885986328,67.028015136719,]
lgeoffroy_accurate = [-35.940723419189,-23.940723419189,-11.940722465515,0.059277534485,12.059277534485,19.034545898438,24.059276580811,27.805545806885,31.034545898438,33.746273040771,36.059276580811,37.99707031250,39.805545806885,41.484703063965,43.034545898438,44.455066680908,45.746273040771,46.908157348633,48.059280395508,49.044342041016,49.997074127197,50.917476654053,52.661293029785,53.484706878662,54.275791168213,55.034545898438,55.760971069336,56.455066680908,57.116832733154,57.746269226074,58.343376159668,59.440605163574,60.059280395508,60.555850982666,61.044342041016,61.524749755859,61.997074127197,62.461315155029,62.917476654053,63.365554809570,64.237457275391,64.661293029785,65.077041625977,65.484710693359,65.884292602539,66.275787353516,66.659210205078,67.034545898438,]
mineiro = [-36.000030517578,-24.000028610229,-12.000028610229,-0.000028610229,11.999971389771,19.019420623779,23.999969482422,27.862375259399,31.019420623779,33.686668395996,35.999969482422,38.037422180176,39.862373352051,41.513160705566,43.019420623779,44.404411315918,45.686668395996,46.881126403809,47.999969482422,49.048072814941,50.037422180176,50.973827362061,52.707515716553,53.513160705566,54.282775878906,55.019416809082,55.725822448730,56.404415130615,57.057373046875,57.686668395996,58.294063568115,59.449333190918,59.999965667725,60.531768798828,61.048072814941,61.549705505371,62.037422180176,62.511901855469,62.973827362061,63.423786163330,64.290107727051,64.707519531250,65.115043640137,65.513160705566,65.902275085449,66.282775878906,66.655044555664,67.019416809082,]
mineiro_faster = [-35.312347412109,-23.312347412109,-11.312347412109,0.687652587891,12.687652587891,18.687652587891,24.687652587891,27.687652587891,30.687652587891,33.687652587891,36.687652587891,38.187652587891,39.687652587891,41.187652587891,42.687652587891,44.187652587891,45.687652587891,47.187652587891,48.687652587891,49.437652587891,50.187652587891,50.937652587891,52.437652587891,53.187652587891,53.937652587891,54.687652587891,55.437652587891,56.187652587891,56.937652587891,57.687652587891,58.437652587891,59.937652587891,60.687652587891,61.062652587891,61.437652587891,61.812652587891,62.187652587891,62.562652587891,62.937652587891,63.312652587891,64.062652587891,64.437652587891,64.812652587891,65.187652587891,65.562652587891,65.937652587891,66.312652587891,66.687652587891,]
desoras = [-36,-24,-12,0,12,18,24,27,30,33,36,37.50,39,40.50,42,43.50,45,46.50,48,48.750,49.50,50.250,51.750,52.50,53.250,54,54.750,55.50,56.250,57,57.750,59.250,60,60.3750,60.750,61.1250,61.50,61.8750,62.250,62.6250,63.3750,63.750,64.1250,64.50,64.8750,65.250,65.6250,66,]
log1_ankerl32 = [-35.304180145264,-23.304183959961,-11.304183959961,0.695816457272,12.695816040039,18.695816040039,24.695817947388,27.695817947388,30.695817947388,33.695816040039,36.695819854736,38.195816040039,39.695816040039,41.195816040039,42.695816040039,44.195816040039,45.695816040039,47.195816040039,48.695816040039,49.445816040039,50.195816040039,50.945816040039,52.445816040039,53.195816040039,53.945816040039,54.695816040039,55.445816040039,56.195816040039,56.945816040039,57.695816040039,58.445816040039,59.945816040039,60.695816040039,61.070816040039,61.445816040039,61.820816040039,62.195816040039,62.570816040039,62.945816040039,63.320816040039,64.070816040039,64.445816040039,64.820816040039,65.195816040039,65.570816040039,65.945816040039,66.320816040039,66.695816040039,]
log1_ekmett_lb = [-36,-24,-12.000001907349,-0.000001430511,11.999998092651,17.999996185303,23.999996185303,27,30,33,36,37.50,39.000003814697,40.499996185303,42,43.50,44.999996185303,46.499996185303,48,48.750,49.50,50.250,51.750,52.50,53.250,54,54.750,55.50,56.250,57,57.750,59.250,60,60.3750,60.750,61.1250,61.50,61.8750,62.250,62.6250,63.3750,63.750,64.1250,64.50,64.8750,65.250,65.6250,66,]

x = [i for i in range(len(stl))]

ax.plot(stl, label='stl', linestyle=linestyles.get())
# ax.plot(lgeoffroy_accurate, label='lgeoffroy_accurate', linestyle=linestyles.get())
# ax.plot(lgeoffroy, label='lgeoffroy', linestyle=linestyles.get())
ax.plot(mineiro, label='mineiro', linestyle=linestyles.get())
# ax.plot(log1_ankerl32, label='log1_ankerl32', linestyle=linestyles.get())
# ax.plot(mineiro_faster, label='mineiro_faster', linestyle=linestyles.get())
# ax.plot(log1_ekmett_lb, label='log1_ekmett_lb', linestyle=linestyles.get())
# ax.plot(desoras, label='desoras', linestyle=linestyles.get())


ax.legend()

# plt.plot(x, y)  # Plot the chart
plt.show()  # display
