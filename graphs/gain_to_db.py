import matplotlib.pyplot as plt
import numpy as np

"""
Converting gain to dB is often done in compressors and when
displaying volume in dB in GUIs.
In audio, high precision is necessary, making log2_mineiro
the only viable option. It offers high precision and is
significantly faster than using std::log10.
In graphics, errors may only cause a pixel to be off by a
fraction, making log1_ankerl32 an excellent choice.

The following table compares the speed and error margin of
different log10 approxiamtions used in the following formula:
    log10(x) * 20

SEC      xSPEED ERROR   NAME
0.055813                pass
0.178238 1      0       stl
0.098199 2.88   awful   log1_njuffa
0.082548 4.57   bad     newton
0.076831 5.82   awful   log1_njuffa_faster
0.074206 6.65   0.0005  log2_mineiro
0.067856 10.16  0.36    log2_mineiro_faster
0.067668 10.32  0.5     log1_ekmett_ub
0.067658 10.33  0.5     log1_ekmett_lb
0.065508 12.62  0.34    log1_ankerl32
0.061177 22.82  bad     jcook

- pass = no processing
- stl = log10(x)
- xSpeed of the log10 function calculated: (stl - pass) / (other_sec - pass)
- Error is in dB
"""

stl = [-84,-81,-78,-75,-72,-69,-66,-63,-60,-57,-54,-51,-48,-45,-42.000003814697,-39,-36,-33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,5.999999523163,9,12,]
# jcook = [-19.997476577759,-19.996435165405,-19.994966506958,-19.992887496948,-19.9899559021,-19.985813140869,-19.979961395264,-19.971702575684,-19.960039138794,-19.943578720093,-19.920349121094,-19.887582778931,-19.841388702393,-19.776321411133,-19.684772491455,-19.556171417236,-19.37593460083,-19.124118804932,-18.773862838745,-18.289663314819,-17.62596321106,-16.726726531982,-15.527370452881,-13.960817337036,-11.969599723816,-9.524361610413,-6.645577430725,-3.419947385788,0,3.419947385788,6.645576953888,9.524361610413,11.969599723816,]
# newton = [-7.19820022583,-7.197948455811,-7.197592258453,-7.197088718414,-7.196377754211,-7.195373058319,-7.193953990936,-7.191950798035,-7.189118862152,-7.185120105743,-7.179470539093,-7.171492099762,-7.160220623016,-7.144300460815,-7.12181186676,-7.090046882629,-7.045177459717,-6.981796741486,-6.892270088196,-6.765809059143,-6.587178707123,-6.334857940674,-5.978443622589,-5.474996566772,-4.763857364655,-3.759346961975,-2.340438604355,-0.336176872253,2.494918346405,6.493947029114,12.142723083496,9.977328300476,17.063131332397,]
# log1_njuffa = [2998.547119140625,3001.54736328125,3004.54736328125,3007.546875,3010.547119140625,3013.54736328125,3016.54736328125,3019.547119140625,3022.54736328125,3025.547119140625,3028.54736328125,3031.54736328125,3034.547119140625,3037.54736328125,3040.547119140625,3043.547119140625,3046.54736328125,3049.54736328125,3052.546875,3055.547119140625,3058.54736328125,3061.54736328125,3064.547119140625,3067.54736328125,3070.547119140625,3073.54736328125,3076.54736328125,-3,0,3,6,9,12,]
# log1_njuffa_faster = [2998.547119140625,3001.54736328125,3004.54736328125,3007.546875,3010.547119140625,3013.547119140625,3016.54736328125,3019.547119140625,3022.54736328125,3025.547119140625,3028.54736328125,3031.546875,3034.547119140625,3037.54736328125,3040.547119140625,3043.547119140625,3046.54736328125,3049.547119140625,3052.546875,3055.547119140625,3058.54736328125,3061.54736328125,3064.547119140625,3067.546875,3070.547119140625,3073.54736328125,3076.54736328125,-3.000080823898,0,2.999919652939,6,8.999919891357,12,]
log1_ankerl32 = [-83.736038208008,-81.168457031250,-77.730186462402,-75.168693542480,-71.724296569824,-69.168869018555,-65.718368530273,-63.168998718262,-59.712406158447,-57.16907119751,-53.706405639648,-51.169097900391,-47.700382232666,-45.169082641602,-41.694313049316,-39.169010162354,-35.688213348389,-33.168891906738,-29.682081222534,-27.168727874756,-23.675910949707,-21.168514251709,-17.669708251953,-15.168251037598,-11.663473129272,-9.167939186096,-5.657201766968,-3.167581081390,0.349102765322,2.832826614380,6.341178894043,8.833281517029,12.333322525024,]
log1_ekmett_ub = [-83.566940307617,-80.999366760254,-77.561080932617,-74.999588012695,-71.555191040039,-68.999771118164,-65.549270629883,-62.999897003174,-59.543308258057,-56.999969482422,-53.537307739258,-51.000003814697,-47.531276702881,-44.999980926514,-41.525215148926,-38.999912261963,-35.519115447998,-32.999794006348,-29.512979507446,-26.999626159668,-23.506813049316,-20.999412536621,-17.500612258911,-14.999151229858,-11.494373321533,-8.998841285706,-5.488103866577,-2.998482227325,0.518201351166,3.001924991608,6.510277271271,9.002379417419,12.502422332764,]
log1_ekmett_lb = [-84.085151672363,-81.517562866211,-78.079284667969,-75.517799377441,-72.073394775391,-69.517967224121,-66.067474365234,-63.518096923828,-60.061508178711,-57.518177032471,-54.055511474609,-51.518203735352,-48.04948425293,-45.518184661865,-42.04341506958,-39.518112182617,-36.037315368652,-33.517997741699,-30.031183242798,-27.517833709717,-24.025012969971,-21.517616271973,-18.018814086914,-15.517353057861,-12.01257610321,-9.517042160034,-6.006305217743,-3.516684055328,-0.000000717711,2.483722925186,5.992074966431,8.484177589417,11.984219551086,]
log2_mineiro = [-84.000480651855,-80.999946594238,-78.000450134277,-74.999961853027,-72.000457763672,-68.999946594238,-66.000411987305,-62.999916076660,-60.000328063965,-56.999923706055,-54.000282287598,-50.999980926514,-48.000267028809,-44.999942779541,-42.000289916992,-38.999946594238,-36.000259399414,-32.999923706055,-30.000194549561,-26.999950408936,-24.000158309937,-20.999950408936,-18.000080108643,-14.999923706055,-12.000110626221,-8.999944686890,-6.000020503998,-2.999936342239,0.000038146973,3.000022172928,5.999965190887,9.000085830688,11.999979019165,]
log2_mineiro_faster = [-83.740081787109,-81.172485351562,-77.734222412109,-75.172729492188,-71.728363037109,-69.172897338867,-65.722427368164,-63.172988891602,-59.716415405273,-57.173080444336,-53.710403442383,-51.173171997070,-47.704391479492,-45.173110961914,-41.698379516602,-39.173049926758,-35.692291259766,-33.172912597656,-29.686126708984,-27.172775268555,-23.679962158203,-21.172561645508,-17.673721313477,-15.172271728516,-11.667556762695,-9.171981811523,-5.661239624023,-3.171615600586,0.345077514648,2.828750610352,6.337127685547,8.829269409180,12.329330444336,]

x_labels = [
    -84, -81, -78, -75,
    -72, -69, -66, -63,
    -60, -57, -54, -51,
    -48, -45, -42, -39,
    -36, -33, -30, -27,
    -24, -21, -18, -15,
    -12, -9, -6, -3,
    0, 3, 6, 9,
    12
]

class Styles:
    idx = 0
    types = ['solid', 'dotted', 'dashed', 'dashdot']

    def get(self):
        s = self.types[self.idx]
        self.idx = (self.idx + 1) & 3
        return s

linestyles = Styles()

fig, ax = plt.subplots()

x_ticks = [i for i in range(len(x_labels))]
x = [i for i in range(len(stl))]

plt.xlabel('dB')
plt.xticks(ticks=x_ticks, labels=x_labels)
plt.ylabel('dB')

ax.plot(stl, label='stl', linestyle=linestyles.get())
# ax.plot(jcook, label='jcook', linestyle=linestyles.get())
# ax.plot(newton, label='newton', linestyle=linestyles.get())
# ax.plot(log1_njuffa, label='log1_njuffa', linestyle=linestyles.get())
# ax.plot(log1_njuffa_faster, label='log1_njuffa_faster', linestyle=linestyles.get())
ax.plot(log1_ankerl32, label='log1_ankerl32', linestyle=linestyles.get())
ax.plot(log1_ekmett_ub, label='log1_ekmett_ub', linestyle=linestyles.get())
ax.plot(log1_ekmett_lb, label='log1_ekmett_lb', linestyle=linestyles.get())
ax.plot(log2_mineiro, label='log2_mineiro', linestyle=linestyles.get())
ax.plot(log2_mineiro_faster, label='log2_mineiro_faster', linestyle=linestyles.get())

ax.legend()
plt.show()