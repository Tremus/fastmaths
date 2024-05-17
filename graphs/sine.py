import matplotlib.pyplot as plt
import numpy as np
import math

"""
Sin functions can be used in audio synthesis, trigonometry, filter
coefficients, LFOs and saturation algorithms.

In audio synthesis, errors will result in aliasing, making high precision is
necessary. If the radians can be controlled to be within -/+ π, then the pade &
sin_approx functions are great choices. If not, the mineiro_full function may
be okay here, although the aliasing in your use case should be analysed before
deciding to use it.

High precision is also needed in generating filter coefficients, so again the
pade & sin_approx functions may be good choices, however it should be tested
how much they effect the cutoff.
For displaying a magnitude graph of a filters cutoff, you may be able to get
away with the very fast functions. The inputs to sin functions are often well
below π which is where these approximations are most accurate. The func
mineiro_faster is likely a great choice, and perhaps bluemangoo, however their
errors should be plotted before using them.

In trigonometry for graphics, the fast bluemangoo algorithm yields fantastic
speed. It is bad after +/- π, however most right angle triangle trig only uses
angles less than 90deg. The accuracy of wildmagic0 is also great for right
angle triangles.

LFO's can also take advantage of the fast bluemangoo algorithm.
Often a sine LFO is implmented as:
    sin(-pi + phase_0_to1 * 2pi);
This is so the phase can easily be cycled by using:
    phase_0_to1 += inc;
    phase_0_to1 -= (int)phase_0_to1;
The sin function easily remains within +/- π bounds, making it a great choice.

In saturation, error will result in different tones, so you should experiment
with the sound of different funcs that suit your needs. For cycling funcs,
mineiro_full_faster should work great.

The following table compares the speed and error margin of different sin
approxiamtions used in the following formula:
    sin(radians)

SEC      xSPEED ERROR     NAME
0.055519                  pass
0.117328        0         stl
0.139088 0.73   0.0000005 njuffa              -- cycles well. extremely accurate
0.089681 1.8    0.000005  mineiro_full        -- cycles well. very accurate
0.084139 2.15   0.000000  sin_approx          -- bad close to +/- 2π. extremely accurate before
0.080947 2.43   0.00035   mineiro_full_faster -- cycles well. fairly accurate
0.080815 2.44   0.000025  wildmagic1          -- bad after +/- π
0.078929 2.64   0.000023  mineiro             -- really bad after +/- π. very accurate before
0.078534 2.68   0.043     juha_fmod           -- cycles well. slightly innacurate
0.077564 2.8    0.000001  pade                -- really bad close to +/- 2π. extremely accurate before +/- 2π
0.071909 3.77   0.0007    slaru               -- bad after +/- π
0.069258 4.49   0.00063   mineiro_faster      -- bad after +/- π
0.067741 5.05   0.000025  wildmagic0          -- bad after +/- π/2
0.067358 5.22   trash     bhaskara_radians    -- do not use
0.067136 5.32   0.0007    lanceputnam_gamma   -- bad after +/- π
0.063552 7.69   0.043     juha                -- bad after +/- π
0.060302 12.92  0.043     bluemangoo          -- bad after +/- π

- pass = no processing
- stl = sin(x)
- xSpeed of the sin function calculated: (stl - pass) / (other_sec - pass)
- Error is is taken at the worst point within +/- π, excexpt for the full sine
  function, where is error is taken anywhere that looks bad
"""

stl = [-0.000000349691,0.382683277130,0.707106769085,0.923879563808,1.000000000000,0.923879325390,0.707107007504,0.382683604956,0.000000023850,-0.382683575153,-0.707107007504,-0.923879683018,-1.000000000000,-0.923879623413,-0.707106769085,-0.382683306932,-0.000000174846,0.382683426142,0.707106888294,0.923879444599,1.000000000000,0.923879444599,0.707106709480,0.382683426142,0.000000087423,-0.382683485746,-0.707106769085,-0.923879563808,-1.000000000000,-0.923879504204,-0.707106828690,-0.382683455944,0.000000000000,0.382683455944,0.707106828690,0.923879504204,1.000000000000,0.923879563808,0.707106769085,0.382683485746,-0.000000087423,-0.382683426142,-0.707106709480,-0.923879444599,-1.000000000000,-0.923879444599,-0.707106888294,-0.382683426142,0.000000174846,0.382683306932,0.707106769085,0.923879623413,1.000000000000,0.923879683018,0.707107007504,0.382683575153,-0.000000023850,-0.382683604956,-0.707107007504,-0.923879325390,-1.000000000000,-0.923879563808,-0.707106769085,-0.382683277130,0.000000349691,]
# bhaskara_radians = [-3.047618865967,-3.005593538284,-2.961039066315,-2.913781404495,-2.863636255264,-2.810408830643,-2.753894090652,-2.693877458572,-2.630136966705,-2.562443733215,-2.490566015244,-2.414271593094,-2.333333015442,-2.247535705566,-2.156682014465,-2.060606002808,-1.959183573723,-1.852348923683,-1.740112900734,-1.622585535049,-1.500000000000,-1.372742176056,-1.241379261017,-1.106690645218,-0.969696998596,-0.831683158875,-0.694214880466,-0.559139788151,-0.428571432829,-0.304849833250,-0.190476179123,-0.088019557297,0.000000000000,0.071246817708,0.123711340129,0.155844151974,0.166666656733,0.155844166875,0.123711347580,0.071246840060,0.000000000000,-0.088019534945,-0.190476149321,-0.304849773645,-0.428571403027,-0.559139847755,-0.694214761257,-0.831683158875,-0.969696998596,-1.106690645218,-1.241379261017,-1.372742176056,-1.500000000000,-1.622585177422,-1.740112781525,-1.852348804474,-1.959183573723,-2.060606002808,-2.156682014465,-2.247535467148,-2.333333015442,-2.414271593094,-2.490566015244,-2.562443733215,-2.630136966705,]
pade = [49.701240539551,42.227882385254,35.307811737061,28.972780227661,23.250602722168,18.163597106934,13.726802825928,9.946066856384,6.816237926483,4.319363117218,2.423375606537,1.081289649010,0.231323838234,-0.201873153448,-0.304691821337,-0.170606121421,0.105037495494,0.431226164103,0.727860629559,0.932004094124,1.002875804901,0.924785315990,0.707355439663,0.382741451263,0.000011251694,-0.382682055235,-0.707106769085,-0.923879683018,-1.000000000000,-0.923879504204,-0.707106828690,-0.382683426142,0.000000000000,0.382683426142,0.707106828690,0.923879504204,1.000000000000,0.923879683018,0.707106769085,0.382682055235,-0.000011251694,-0.382741451263,-0.707355439663,-0.924785315990,-1.002875804901,-0.932004094124,-0.727860629559,-0.431226164103,-0.105037495494,0.170606121421,0.304691821337,0.201873153448,-0.231323838234,-1.081289649010,-2.423375606537,-4.319363117218,-6.816237926483,-9.946066856384,-13.726802825928,-18.163597106934,-23.250602722168,-28.972780227661,-35.307811737061,-42.227882385254,-49.701240539551,]
sin_approx = [-9491.151367187500,-5948.825683593750,-3662.681152343750,-2212.158447265625,-1308.597167968750,-756.874328613281,-427.246826171875,-234.958602905273,-125.702659606934,-65.402648925781,-33.163314819336,-16.494836807251,-8.144445419312,-4.040431499481,-1.990154743195,-0.876610636711,-0.175704747438,0.325832486153,0.690734624863,0.919813096523,0.999170243740,0.923751175404,0.707094013691,0.382682859898,0.000000087423,-0.382683515549,-0.707106828690,-0.923879563808,-1.000000000000,-0.923879444599,-0.707106769085,-0.382683426142,0.000000000000,0.382683426142,0.707106769085,0.923879444599,1.000000000000,0.923879563808,0.707106828690,0.382683515549,-0.000000087423,-0.382682859898,-0.707094013691,-0.923751175404,-0.999170243740,-0.919813096523,-0.690734624863,-0.325832486153,0.175704747438,0.876610636711,1.990154743195,4.040431499481,8.144445419312,16.494836807251,33.163314819336,65.402648925781,125.702659606934,234.958602905273,427.246826171875,756.874328613281,1308.597167968750,2212.158447265625,3662.681152343750,5948.825683593750,9491.151367187500,]
slaru = [555.599975585938,481.344604492188,414.820159912109,355.467926025391,302.749877929688,256.149200439453,215.170257568359,179.338333129883,148.199951171875,121.322708129883,98.295272827148,78.727409362793,62.249992370605,48.514930725098,37.195297241211,27.985248565674,20.599998474121,14.775873184204,10.270307540894,6.861813545227,4.349997997284,2.555565595627,1.320312500000,0.507128417492,0.000000000000,-0.382129132748,-0.707812786102,-0.924316287041,-1.000000000000,-0.924316465855,-0.707812488079,-0.382128894329,0.000000000000,0.382128894329,0.707812488079,0.924316465855,1.000000000000,0.924316287041,0.707812786102,0.382129132748,0.000000000000,-0.507128417492,-1.320312500000,-2.555565595627,-4.349997997284,-6.861813545227,-10.270307540894,-14.775873184204,-20.599998474121,-27.985248565674,-37.195297241211,-48.514930725098,-62.249992370605,-78.727409362793,-98.295272827148,-121.322708129883,-148.199951171875,-179.338333129883,-215.170257568359,-256.149200439453,-302.749877929688,-355.467926025391,-414.820159912109,-481.344604492188,-555.599975585938,]
juha = [48.000000000000,44.562500000000,41.249992370605,38.062492370605,34.999996185303,32.062496185303,29.250000000000,26.562500000000,24.000000000000,21.562496185303,19.249996185303,17.062496185303,14.999996185303,13.062500000000,11.250000000000,9.562497138977,8.000000000000,6.562498569489,5.249999046326,4.062500000000,3.000000000000,2.062499284744,1.249999284744,0.562500000000,-0.000000000000,-0.437500178814,-0.750000000000,-0.937500059605,-1.000000000000,-0.937500000000,-0.750000000000,-0.437500000000,0.000000000000,0.437500000000,0.750000000000,0.937500000000,1.000000000000,0.937500059605,0.750000000000,0.437500178814,0.000000000000,-0.562500000000,-1.249999284744,-2.062499284744,-3.000000000000,-4.062500000000,-5.249999046326,-6.562498569489,-8.000000000000,-9.562497138977,-11.250000000000,-13.062500000000,-14.999996185303,-17.062496185303,-19.249996185303,-21.562496185303,-24.000000000000,-26.562500000000,-29.250000000000,-32.062496185303,-34.999996185303,-38.062492370605,-41.249992370605,-44.562500000000,-48.000000000000,]
juha_fmod = [-0.000000000000,0.437500178814,0.750000238419,0.937500178814,1.000000000000,0.937499761581,0.750000119209,0.437499880791,0.000000238419,-0.437500357628,-0.750000357628,-0.937500238419,-1.000000000000,-0.937500000000,-0.749999880791,-0.437499701977,-0.000000000000,0.437500178814,0.750000238419,0.937500000000,0.999999940395,0.937499940395,0.749999880791,0.437499880791,0.000000000000,-0.437500178814,-0.750000000000,-0.937500059605,-1.000000000000,-0.937500000000,-0.750000000000,-0.437500000000,0.000000000000,0.437500000000,0.750000000000,0.937500000000,1.000000000000,0.937500059605,0.750000000000,0.437500178814,-0.000000000000,-0.437499880791,-0.749999880791,-0.937499940395,-0.999999940395,-0.937500000000,-0.750000238419,-0.437500178814,0.000000000000,0.437499701977,0.749999880791,0.937500000000,1.000000000000,0.937500238419,0.750000357628,0.437500357628,-0.000000238419,-0.437499880791,-0.750000119209,-0.937499761581,-1.000000000000,-0.937500178814,-0.750000238419,-0.437500178814,0.000000000000,]
mineiro = [39333496.000000000000,25176090.000000000000,15832175.000000000000,9767151.000000000000,5901057.500000000000,3484804.750000000000,2006961.000000000000,1124276.375000000000,610730.000000000000,320544.312500000000,161842.203125000000,78189.367187500000,35907.546875000000,15544.102539062500,6273.893066406250,2326.371826171875,776.074707031250,225.732910156250,54.493064880371,10.192822456360,1.644728183746,0.726024925709,0.637736618519,0.375405341387,0.000000374052,-0.382703781128,-0.707092881203,-0.923892557621,-0.999986410141,-0.923892736435,-0.707092881203,-0.382704019547,0.000000000000,0.382704019547,0.707092881203,0.923892736435,0.999986410141,0.923892557621,0.707092881203,0.382703781128,-0.000000374052,-0.375405341387,-0.637736618519,-0.726024925709,-1.644728183746,-10.192822456360,-54.493064880371,-225.732910156250,-776.074707031250,-2326.371826171875,-6273.893066406250,-15544.102539062500,-35907.546875000000,-78189.367187500000,-161842.203125000000,-320544.312500000000,-610730.000000000000,-1124276.375000000000,-2006961.000000000000,-3484804.750000000000,-5901057.500000000000,-9767151.000000000000,-15832175.000000000000,-25176090.000000000000,-39333496.000000000000,]
mineiro_faster = [551.252075195312,477.601257324219,411.616882324219,352.744476318359,300.450805664062,254.223342895508,213.570983886719,178.022674560547,147.128952026367,120.461128234863,97.611335754395,78.192657470703,61.839088439941,48.205581665039,36.967922210693,27.822870254517,20.488098144531,14.702142715454,10.224518775940,6.835619449615,4.336756706238,2.550164222717,1.318983316422,0.507271289825,0.000000370183,-0.296944439411,-0.456762313843,-0.531738638878,-0.553245127201,-0.531738698483,-0.456762313843,-0.296944588423,0.000000000000,0.382344365120,0.707733035088,0.923880457878,0.999415159225,0.923880338669,0.707733035088,0.382344126701,-0.000000370183,-0.366100251675,-0.621842265129,-0.652198016644,-0.321224749088,0.527935028076,2.073050498962,4.512807369232,8.066810607910,12.975555419922,19.500494003296,27.923954010010,38.549186706543,51.700386047363,67.722618103027,86.981880187988,109.865089416504,136.780120849609,168.155639648438,204.441162109375,246.107696533203,293.646362304688,347.569641113281,408.410858154297,476.724395751953,]
mineiro_full = [0.000000000000,0.382703542709,0.707092881203,0.923892736435,0.999986529350,0.923892378807,0.707093000412,0.382704108953,0.000000000000,-0.382704108953,-0.707093000412,-0.923892855644,-0.999986767769,-0.923892736435,-0.707092881203,-0.382703542709,0.000000000000,0.382703781128,0.707092881203,0.923892557621,0.999986529350,0.923892557621,0.707092761993,0.382703930140,0.000000000000,-0.382704108953,-0.707093000412,-0.923892736435,-0.999986410141,-0.923892557621,-0.707092881203,-0.382703542709,-0.000000374052,0.382703542709,0.707092881203,0.923892557621,0.999986410141,0.923892736435,0.707093000412,0.382704108953,0.000000000000,-0.382703930140,-0.707092761993,-0.923892557621,-0.999986529350,-0.923892557621,-0.707092881203,-0.382703781128,0.000000000000,0.382703542709,0.707092881203,0.923892736435,0.999986767769,0.923892855644,0.707093000412,0.382704108953,0.000000000000,-0.382704108953,-0.707093000412,-0.923892378807,-0.999986529350,-0.923892736435,-0.707092881203,-0.382703542709,0.000000000000,]
mineiro_full_faster = [0.000000000000,0.382343888283,0.707733035088,0.923880457878,0.999415278435,0.923880100250,0.707733213902,0.382344454527,0.000000000000,-0.382344454527,-0.707733213902,-0.923880636692,-0.999415516853,-0.923880457878,-0.707733035088,-0.382343888283,0.000000000000,0.382344126701,0.707733035088,0.923880338669,0.999415278435,0.923880338669,0.707732915878,0.382344275713,0.000000000000,-0.382344454527,-0.707733213902,-0.923880457878,-0.999415159225,-0.923880338669,-0.707733035088,-0.382343888283,-0.000000370183,0.382343888283,0.707733035088,0.923880338669,0.999415159225,0.923880457878,0.707733213902,0.382344454527,0.000000000000,-0.382344275713,-0.707732915878,-0.923880338669,-0.999415278435,-0.923880338669,-0.707733035088,-0.382344126701,0.000000000000,0.382343888283,0.707733035088,0.923880457878,0.999415516853,0.923880636692,0.707733213902,0.382344454527,0.000000000000,-0.382344454527,-0.707733213902,-0.923880100250,-0.999415278435,-0.923880457878,-0.707733035088,-0.382343888283,0.000000000000,]

njuffa = [0.000000000021,0.382683575153,0.707107245922,0.923879802227,1.000000000000,0.923879086971,0.707106530666,0.382683575153,-0.000000000015,-0.382683575153,-0.707107245922,-0.923879802227,-1.000000000000,-0.923879444599,-0.707106530666,-0.382683128119,0.000000000010,0.382683575153,0.707106888294,0.923879444599,1.000000000000,0.923879444599,0.707106649876,0.382683366537,-0.000000000005,-0.382683575153,-0.707106828690,-0.923879563808,-1.000000000000,-0.923879504204,-0.707106709480,-0.382683455944,0.000000000000,0.382683455944,0.707106709480,0.923879504204,1.000000000000,0.923879563808,0.707106828690,0.382683575153,0.000000000005,-0.382683366537,-0.707106649876,-0.923879444599,-1.000000000000,-0.923879444599,-0.707106888294,-0.382683575153,-0.000000000010,0.382683128119,0.707106530666,0.923879444599,1.000000000000,0.923879802227,0.707107245922,0.382683575153,0.000000000015,-0.382683575153,-0.707106530666,-0.923879086971,-1.000000000000,-0.923879802227,-0.707107245922,-0.382683575153,-0.000000000021,]

wildmagic0 = [-2067.758056640625,-1747.263671875000,-1467.265136718750,-1223.856079101562,-1013.384094238281,-832.445068359375,-677.873840332031,-546.734375000000,-436.313568115234,-344.111114501953,-267.831420898438,-205.375015258789,-154.830047607422,-114.463699340820,-82.713821411133,-58.180229187012,-39.616367340088,-25.920513153076,-16.127502441406,-9.400059700012,-5.020290374756,-2.381196260452,-0.978110074997,-0.400172024965,-0.321810156107,-0.494206398726,-0.736769616604,-0.928607463837,-0.999997675419,-0.923859357834,-0.707225620747,-0.382714301348,0.000000000000,0.382714301348,0.707225620747,0.923859357834,0.999997675419,0.928607463837,0.736769616604,0.494206398726,0.321810156107,0.400172024965,0.978110074997,2.381196260452,5.020290374756,9.400059700012,16.127502441406,25.920513153076,39.616367340088,58.180229187012,82.713821411133,114.463699340820,154.830047607422,205.375015258789,267.831420898438,344.111114501953,436.313568115234,546.734375000000,677.873840332031,832.445068359375,1013.384094238281,1223.856079101562,1467.265136718750,1747.263671875000,2067.758056640625,]
wildmagic1 = [15507.542968750000,10557.169921875000,7086.625488281250,4685.791503906250,3048.538574218750,1949.030029296875,1222.754638671875,751.491271972656,451.545806884766,264.608795166016,150.761459350586,83.189079284668,44.245231628418,22.570520401001,11.022744178772,5.221198558807,2.547279357910,1.476758241653,1.147341966629,1.088062047958,1.055972695351,0.941009759903,0.711697220802,0.383723884821,0.000189126062,-0.382658183575,-0.707104682922,-0.923879444599,-0.999999880791,-0.923879563808,-0.707106769085,-0.382683426142,0.000000000000,0.382683426142,0.707106769085,0.923879563808,0.999999880791,0.923879444599,0.707104682922,0.382658183575,-0.000189126062,-0.383723884821,-0.711697220802,-0.941009759903,-1.055972695351,-1.088062047958,-1.147341966629,-1.476758241653,-2.547279357910,-5.221198558807,-11.022744178772,-22.570520401001,-44.245231628418,-83.189079284668,-150.761459350586,-264.608795166016,-451.545806884766,-751.491271972656,-1222.754638671875,-1949.030029296875,-3048.538574218750,-4685.791503906250,-7086.625488281250,-10557.169921875000,-15507.542968750000,]

bluemangoo = [48.000000000000,44.562500000000,41.249992370605,38.062492370605,34.999996185303,32.062496185303,29.250000000000,26.562500000000,24.000000000000,21.562496185303,19.249996185303,17.062496185303,14.999996185303,13.062500000000,11.250000000000,9.562497138977,8.000000000000,6.562498569489,5.249999046326,4.062500000000,3.000000000000,2.062499284744,1.249999284744,0.562500000000,0.000000000000,-0.437500178814,-0.750000000000,-0.937500059605,-1.000000000000,-0.937500000000,-0.750000000000,-0.437500000000,0.000000000000,0.437500000000,0.750000000000,0.937500000000,1.000000000000,0.937500059605,0.750000000000,0.437500178814,0.000000000000,-0.562500000000,-1.249999284744,-2.062499284744,-3.000000000000,-4.062500000000,-5.249999046326,-6.562498569489,-8.000000000000,-9.562497138977,-11.250000000000,-13.062500000000,-14.999996185303,-17.062496185303,-19.249996185303,-21.562496185303,-24.000000000000,-26.562500000000,-29.250000000000,-32.062496185303,-34.999996185303,-38.062492370605,-41.249992370605,-44.562500000000,-48.000000000000,]
lanceputnam_gamma = [-481.199981689453,-412.272735595703,-350.882690429688,-296.471069335938,-248.499938964844,-206.452392578125,-169.832794189453,-138.166503906250,-111.000000000000,-87.900840759277,-68.457778930664,-52.280540466309,-38.999977111816,-28.268066406250,-19.757810592651,-13.163369178772,-8.199999809265,-4.604000568390,-2.132811069489,-0.564941406250,0.299999982119,0.641308665276,0.617187321186,0.364746093750,-0.000000000000,-0.382129073143,-0.707812488079,-0.924316465855,-1.000000000000,-0.924316406250,-0.707812488079,-0.382128894329,0.000000000000,0.382128894329,0.707812488079,0.924316406250,1.000000000000,0.924316465855,0.707812488079,0.382129073143,0.000000000000,-0.364746093750,-0.617187321186,-0.641308665276,-0.299999982119,0.564941406250,2.132811069489,4.604000568390,8.199999809265,13.163369178772,19.757810592651,28.268066406250,38.999977111816,52.280540466309,68.457778930664,87.900840759277,111.000000000000,138.166503906250,169.832794189453,206.452392578125,248.499938964844,296.471069335938,350.882690429688,412.272735595703,481.199981689453,]

x_labels = [
    '-4', '', '-3 3/4', '', '-3 1/2', '', '-3 1/4', '',
    '-3', '', '-2 3/4', '', '-2 1/2', '', '-2 1/4', '',
    '-2', '', '-1 3/4', '', '-1 1/2', '', '-1 1/4', '',
    '-π', '', '-3/4', '', '-π/2', '', '-π/4', '',
    '0', '', 'π/4', '', 'π/2', '', '3/4', '',
    'π', '', '1 1/4', '', '1 1/2', '', '1 3/4', '',
    '2', '', '2 1/4', '', '2 1/2', '', '2 3/4', '',
    '3', '', '3 1/4', '', '3 1/2', '', '3 3/4', '',
    '4',
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

# plt.xlabel('phase 0-1')
plt.xticks(ticks=x_ticks, labels=x_labels)
plt.ylabel('mag')
plt.ylim(-1, 1)
plt.xlim(left=(-1.5 * math.pi), right=(1.5 * math.pi))

ax.plot(stl, label='stl', linestyle=linestyles.get())
# ax.plot(bhaskara_radians, label='bhaskara_radians', linestyle=linestyles.get())

ax.plot(pade, label='pade', linestyle=linestyles.get())
# ax.plot(sin_approx, label='sin_approx', linestyle=linestyles.get())
ax.plot(mineiro, label='mineiro', linestyle=linestyles.get())
# ax.plot(wildmagic0, label='wildmagic0', linestyle=linestyles.get())

# bad after π
# ax.plot(slaru, label='slaru', linestyle=linestyles.get())
# ax.plot(juha, label='juha', linestyle=linestyles.get())
# ax.plot(mineiro_faster, label='mineiro_faster', linestyle=linestyles.get())
# ax.plot(wildmagic1, label='wildmagic1', linestyle=linestyles.get())

# cycles well
# ax.plot(mineiro_full, label='mineiro_full', linestyle=linestyles.get())
# ax.plot(mineiro_full_faster, label='mineiro_full_faster', linestyle=linestyles.get())
# ax.plot(njuffa, label='njuffa', linestyle=linestyles.get())
# ax.plot(juha_fmod, label='juha_fmod', linestyle=linestyles.get())

ax.plot(bluemangoo, label='bluemangoo', linestyle=linestyles.get())
ax.plot(lanceputnam_gamma, label='lanceputnam_gamma', linestyle=linestyles.get())

ax.legend()
plt.show()
