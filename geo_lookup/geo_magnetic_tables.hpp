/****************************************************************************
 *
 *   Copyright (C) 2020 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <stdint.h>

static constexpr float SAMPLING_RES = 10;
static constexpr float SAMPLING_MIN_LAT = -90;
static constexpr float SAMPLING_MAX_LAT = 90;
static constexpr float SAMPLING_MIN_LON = -180;
static constexpr float SAMPLING_MAX_LON = 180;

static constexpr int LAT_DIM = 19;
static constexpr int LON_DIM = 37;


// Magnetic declination data in radians * 10^-4
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2020.5,
static constexpr const int16_t declination_table[19][37] {
//    LONGITUDE:   -180,  -170,  -160,  -150,  -140,  -130,  -120,  -110,  -100,   -90,   -80,   -70,   -60,   -50,   -40,   -30,   -20,   -10,     0,    10,    20,    30,    40,    50,    60,    70,    80,    90,   100,   110,   120,   130,   140,   150,   160,   170,   180,
/* LAT: -90 */ {  26038, 24293, 22548, 20802, 19057, 17312, 15566, 13821, 12076, 10330,  8585,  6840,  5094,  3349,  1604,  -141, -1887, -3632, -5377, -7123, -8868,-10613,-12358,-14104,-15849,-17594,-19340,-21085,-22831,-24576,-26321,-28067,-29812, 31274, 29529, 27784, 26038, },
/* LAT: -80 */ {  22610, 20472, 18526, 16747, 15104, 13563, 12098, 10685,  9308,  7957,  6624,  5305,  3996,  2691,  1384,    66, -1273, -2641, -4047, -5495, -6985, -8517,-10091,-11706,-13367,-15081,-16863,-18732,-20710,-22825,-25096,-27528,-30098, 30090, 27465, 24950, 22610, },
/* LAT: -70 */ {  14960, 13566, 12446, 11490, 10628,  9802,  8964,  8081,  7131,  6113,  5043,  3948,  2860,  1801,   776,  -235, -1269, -2368, -3560, -4849, -6221, -7643, -9085,-10522,-11945,-13358,-14783,-16262,-17875,-19769,-22257,-26008, 30909, 24230, 19632, 16834, 14960, },
/* LAT: -60 */ {   8353,  8122,  7854,  7592,  7350,  7106,  6806,  6381,  5773,  4960,  3962,  2851,  1726,   688,  -206,  -973, -1704, -2518, -3504, -4681, -5995, -7353, -8668, -9878,-10945,-11848,-12568,-13066,-13237,-12787,-10708, -3671,  4684,  7512,  8324,  8472,  8353, },
/* LAT: -50 */ {   5433,  5476,  5429,  5350,  5285,  5256,  5229,  5110,  4775,  4122,  3121,  1854,   508,  -689, -1585, -2173, -2582, -3016, -3674, -4652, -5865, -7126, -8266, -9175, -9788,-10046, -9872, -9134, -7642, -5295, -2399,   345,  2453,  3875,  4749,  5225,  5433, },
/* LAT: -40 */ {   3916,  4012,  4027,  3993,  3941,  3914,  3925,  3920,  3755,  3233,  2227,   796,  -772, -2108, -3003, -3484, -3692, -3763, -3912, -4428, -5354, -6411, -7303, -7862, -7996, -7649, -6786, -5414, -3696, -1981,  -512,   712,  1752,  2610,  3259,  3686,  3916, },
/* LAT: -30 */ {   2951,  3039,  3078,  3075,  3026,  2955,  2901,  2871,  2749,  2286,  1273,  -228, -1840, -3119, -3892, -4266, -4372, -4194, -3767, -3493, -3786, -4505, -5216, -5595, -5501, -4943, -4000, -2790, -1562,  -600,    87,   690,  1308,  1900,  2401,  2756,  2951, },
/* LAT: -20 */ {   2309,  2357,  2385,  2400,  2368,  2282,  2182,  2110,  1967,  1485,   455, -1015, -2501, -3592, -4162, -4311, -4137, -3611, -2762, -1931, -1596, -1910, -2556, -3046, -3110, -2772, -2147, -1324,  -529,   -23,   250,   555,   989,  1454,  1864,  2162,  2309, },
/* LAT: -10 */ {   1915,  1912,  1902,  1911,  1894,  1820,  1720,  1636,  1454,   915,  -125, -1491, -2777, -3639, -3949, -3758, -3206, -2437, -1590,  -818,  -323,  -321,  -771, -1284, -1515, -1440, -1132,  -628,  -117,   139,   200,   365,   727,  1148,  1527,  1803,  1915, },
/* LAT:   0 */ {   1703,  1672,  1628,  1631,  1630,  1572,  1481,  1377,  1127,   514,  -514, -1738, -2810, -3432, -3472, -3004, -2248, -1463,  -801,  -255,   169,   304,    46,  -370,  -637,  -699,  -601,  -334,   -34,    68,    20,   114,   446,   870,  1275,  1582,  1703, },
/* LAT:  10 */ {   1573,  1583,  1550,  1577,  1612,  1574,  1467,  1291,   919,   205,  -811, -1893, -2741, -3114, -2945, -2361, -1589,  -869,  -341,    47,   377,   540,   394,    74,  -168,  -277,  -295,  -207,   -94,  -125,  -253,  -222,    70,   508,   974,  1370,  1573, },
/* LAT:  20 */ {   1401,  1551,  1618,  1718,  1811,  1800,  1658,  1366,   826,   -31, -1076, -2041, -2662, -2801, -2497, -1908, -1199,  -543,   -71,   247,   509,   664,   585,   344,   140,    23,   -59,  -113,  -184,  -366,  -595,  -652,  -426,     8,   538,  1048,  1401, },
/* LAT:  30 */ {   1117,  1485,  1748,  1975,  2139,  2156,  1977,  1563,   838,  -194, -1322, -2220, -2661, -2621, -2237, -1664, -1010,  -387,    86,   400,   636,   790,   780,   634,   481,   359,   213,    17,  -255,  -622,  -988, -1150,  -999,  -582,   -11,   596,  1117, },
/* LAT:  40 */ {    780,  1368,  1862,  2253,  2503,  2549,  2337,  1808,   897,  -339, -1598, -2491, -2835, -2697, -2259, -1670, -1017,  -383,   135,   508,   785,   992,  1095,  1089,  1013,   868,   613,   221,  -303,  -904, -1428, -1679, -1572, -1160,  -561,   116,   780, },
/* LAT:  50 */ {    513,  1264,  1943,  2495,  2858,  2961,  2730,  2074,   924,  -594, -2046, -2988, -3299, -3115, -2628, -1982, -1271,  -568,    55,   568,   994,  1358,  1648,  1826,  1851,  1674,  1248,   563,  -314, -1216, -1905, -2205, -2089, -1648, -1005,  -261,   513, },
/* LAT:  60 */ {    350,  1210,  2015,  2704,  3199,  3401,  3175,  2346,   799, -1207, -2963, -3963, -4219, -3957, -3380, -2630, -1798,  -951,  -136,   622,  1322,  1960,  2512,  2918,  3092,  2926,  2317,  1232,  -163, -1490, -2379, -2704, -2544, -2042, -1333,  -514,   350, },
/* LAT:  70 */ {    179,  1125,  2026,  2823,  3428,  3706,  3424,  2223,  -136, -2963, -4946, -5753, -5735, -5229, -4440, -3491, -2454, -1378,  -294,   774,  1806,  2779,  3655,  4373,  4837,  4894,  4326,  2908,   759, -1348, -2668, -3126, -2966, -2423, -1654,  -765,   179, },
/* LAT:  80 */ {   -315,   616,  1479,  2178,  2558,  2327,   956, -2054, -5605, -7731, -8372, -8144, -7444, -6477, -5349, -4123, -2837, -1515,  -178,  1159,  2479,  3766,  4997,  6136,  7133,  7895,  8247,  7843,  6058,  2569,  -886, -2637, -3052, -2754, -2091, -1243,  -315, },
/* LAT:  90 */ { -30598,-28852,-27107,-25361,-23616,-21870,-20125,-18380,-16634,-14889,-13144,-11399, -9653, -7908, -6163, -4418, -2673,  -928,   818,  2563,  4308,  6053,  7799,  9544, 11289, 13034, 14780, 16525, 18271, 20016, 21761, 23507, 25252, 26998, 28743, 30489,-30598, },
};

// Magnetic inclination data in radians * 10^-4
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2020.5,
static constexpr const int16_t inclination_table[19][37] {
//    LONGITUDE:   -180,  -170,  -160,  -150,  -140,  -130,  -120,  -110,  -100,   -90,   -80,   -70,   -60,   -50,   -40,   -30,   -20,   -10,     0,    10,    20,    30,    40,    50,    60,    70,    80,    90,   100,   110,   120,   130,   140,   150,   160,   170,   180,
/* LAT: -90 */ { -12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584,-12584, },
/* LAT: -80 */ { -13673,-13539,-13379,-13198,-13004,-12802,-12597,-12396,-12204,-12027,-11869,-11735,-11624,-11537,-11474,-11433,-11414,-11417,-11444,-11498,-11581,-11695,-11839,-12011,-12209,-12425,-12654,-12887,-13115,-13328,-13516,-13667,-13772,-13825,-13823,-13770,-13673, },
/* LAT: -70 */ { -14124,-13805,-13486,-13163,-12830,-12485,-12128,-11769,-11424,-11113,-10857,-10671,-10557,-10503,-10491,-10497,-10507,-10520,-10545,-10602,-10709,-10881,-11124,-11435,-11805,-12220,-12667,-13130,-13596,-14049,-14469,-14818,-15014,-14966,-14736,-14439,-14124, },
/* LAT: -60 */ { -13529,-13177,-12840,-12508,-12165,-11792,-11376,-10920,-10449,-10015, -9679, -9496, -9487, -9620, -9826,-10025,-10161,-10215,-10211,-10200,-10245,-10397,-10678,-11078,-11570,-12122,-12707,-13302,-13891,-14452,-14948,-15240,-15074,-14695,-14291,-13900,-13529, },
/* LAT: -50 */ { -12500,-12161,-11832,-11511,-11188,-10841,-10441, -9968, -9436, -8909, -8509, -8370, -8557, -9010, -9576,-10098,-10474,-10657,-10646,-10505,-10363,-10357,-10562,-10964,-11497,-12086,-12673,-13215,-13665,-13966,-14076,-14004,-13802,-13520,-13193,-12848,-12500, },
/* LAT: -40 */ { -11240,-10896,-10553,-10210, -9870, -9530, -9168, -8740, -8216, -7645, -7206, -7141, -7578, -8389, -9313,-10153,-10828,-11285,-11453,-11312,-10981,-10698,-10669,-10931,-11382,-11885,-12337,-12668,-12834,-12841,-12749,-12610,-12428,-12195,-11909,-11584,-11240, },
/* LAT: -30 */ {  -9600, -9230, -8857, -8470, -8077, -7699, -7338, -6942, -6426, -5811, -5342, -5390, -6123, -7308, -8562, -9671,-10600,-11332,-11763,-11789,-11441,-10935,-10583,-10562,-10801,-11122,-11386,-11510,-11454,-11278,-11100,-10958,-10806,-10596,-10315, -9972, -9600, },
/* LAT: -20 */ {  -7368, -6942, -6535, -6112, -5667, -5233, -4836, -4414, -3842, -3152, -2682, -2896, -3958, -5547, -7181, -8574, -9665,-10466,-10936,-11004,-10667,-10066, -9500, -9239, -9275, -9436, -9583, -9610, -9443, -9170, -8968, -8869, -8753, -8539, -8222, -7815, -7368, },
/* LAT: -10 */ {  -4409, -3894, -3456, -3025, -2569, -2116, -1694, -1225,  -586,   128,   517,   141, -1135, -3025, -5009, -6653, -7789, -8455, -8747, -8710, -8327, -7661, -6996, -6636, -6587, -6679, -6802, -6832, -6649, -6350, -6184, -6174, -6114, -5888, -5505, -4986, -4409, },
/* LAT:   0 */ {   -898,  -300,   148,   543,   959,  1377,  1776,  2236,  2839,  3433,  3663,  3210,  1962,    82, -1968, -3658, -4713, -5177, -5259, -5107, -4690, -3995, -3290, -2906, -2841, -2915, -3044, -3122, -2994, -2754, -2686, -2803, -2840, -2642, -2219, -1599,  -898, },
/* LAT:  10 */ {   2569,  3173,  3591,  3925,  4277,  4646,  5006,  5410,  5880,  6267,  6325,  5868,  4822,  3281,  1588,   181,  -665,  -945,  -874,  -652,  -258,   364,   996,  1341,  1403,  1350,  1245,  1153,  1198,  1307,  1246,  1005,   838,   923,  1273,  1865,  2569, },
/* LAT:  20 */ {   5421,  5933,  6302,  6592,  6904,  7249,  7599,  7960,  8308,  8523,  8452,  8011,  7197,  6114,  4985,  4057,  3501,  3357,  3490,  3724,  4044,  4499,  4957,  5215,  5268,  5242,  5188,  5133,  5126,  5115,  4953,  4640,  4360,  4279,  4450,  4865,  5421, },
/* LAT:  30 */ {   7570,  7936,  8248,  8528,  8836,  9186,  9548,  9899, 10187, 10312, 10179,  9771,  9152,  8443,  7778,  7255,  6948,  6890,  7020,  7224,  7464,  7757,  8043,  8217,  8271,  8277,  8274,  8266,  8248,  8173,  7961,  7620,  7275,  7060,  7043,  7233,  7570, },
/* LAT:  40 */ {   9265,  9485,  9739, 10024, 10351, 10715, 11087, 11433, 11693, 11784, 11642, 11288, 10817, 10341,  9940,  9650,  9492,  9478,  9580,  9735,  9905, 10085, 10255, 10380, 10456, 10511, 10562, 10595, 10583, 10479, 10245,  9902,  9540,  9257,  9114,  9125,  9265, },
/* LAT:  50 */ {  10800, 10923, 11126, 11397, 11722, 12079, 12438, 12759, 12986, 13051, 12915, 12618, 12252, 11903, 11624, 11432, 11333, 11323, 11383, 11481, 11591, 11704, 11818, 11933, 12049, 12168, 12279, 12352, 12348, 12230, 11990, 11665, 11328, 11043, 10853, 10773, 10800, },
/* LAT:  60 */ {  12317, 12393, 12547, 12769, 13042, 13346, 13652, 13921, 14100, 14128, 13991, 13740, 13448, 13174, 12951, 12791, 12696, 12660, 12672, 12716, 12781, 12864, 12967, 13097, 13254, 13428, 13592, 13704, 13715, 13601, 13380, 13102, 12823, 12585, 12414, 12324, 12317, },
/* LAT:  70 */ {  13759, 13806, 13905, 14051, 14235, 14442, 14653, 14837, 14941, 14916, 14773, 14567, 14347, 14141, 13965, 13829, 13733, 13676, 13655, 13665, 13706, 13775, 13876, 14010, 14175, 14361, 14545, 14688, 14739, 14672, 14513, 14315, 14120, 13955, 13835, 13769, 13759, },
/* LAT:  80 */ {  15006, 15021, 15062, 15125, 15205, 15294, 15376, 15420, 15394, 15306, 15188, 15060, 14935, 14820, 14719, 14636, 14573, 14532, 14512, 14515, 14541, 14589, 14659, 14751, 14861, 14987, 15121, 15252, 15359, 15406, 15373, 15291, 15199, 15117, 15054, 15017, 15006, },
/* LAT:  90 */ {  15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, 15388, },
};

// Magnetic strength data in milli-Gauss * 10
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2020.5,
static constexpr const int16_t strength_table[19][37] {
//    LONGITUDE:  -180, -170, -160, -150, -140, -130, -120, -110, -100,  -90,  -80,  -70,  -60,  -50,  -40,  -30,  -20,  -10,    0,   10,   20,   30,   40,   50,   60,   70,   80,   90,  100,  110,  120,  130,  140,  150,  160,  170,  180,
/* LAT: -90 */ {  5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, 5463, },
/* LAT: -80 */ {  6070, 6007, 5930, 5839, 5737, 5625, 5508, 5386, 5264, 5144, 5030, 4924, 4830, 4750, 4686, 4641, 4615, 4612, 4633, 4678, 4747, 4840, 4954, 5084, 5227, 5377, 5526, 5669, 5800, 5914, 6007, 6077, 6122, 6142, 6139, 6114, 6070, },
/* LAT: -70 */ {  6314, 6183, 6035, 5872, 5695, 5504, 5299, 5084, 4865, 4649, 4446, 4262, 4103, 3972, 3867, 3789, 3741, 3728, 3754, 3829, 3954, 4132, 4358, 4624, 4918, 5224, 5528, 5812, 6062, 6267, 6419, 6516, 6559, 6553, 6505, 6423, 6314, },
/* LAT: -60 */ {  6198, 6009, 5810, 5603, 5386, 5153, 4897, 4618, 4326, 4035, 3767, 3538, 3356, 3220, 3121, 3047, 2996, 2976, 3000, 3088, 3253, 3500, 3823, 4206, 4627, 5060, 5481, 5866, 6192, 6442, 6607, 6688, 6692, 6631, 6520, 6372, 6198, },
/* LAT: -50 */ {  5853, 5626, 5396, 5168, 4937, 4693, 4424, 4121, 3792, 3459, 3155, 2913, 2749, 2656, 2607, 2574, 2542, 2517, 2522, 2591, 2758, 3042, 3431, 3898, 4403, 4909, 5386, 5806, 6147, 6390, 6531, 6574, 6534, 6426, 6266, 6071, 5853, },
/* LAT: -40 */ {  5400, 5156, 4914, 4676, 4443, 4207, 3954, 3670, 3356, 3030, 2729, 2504, 2383, 2355, 2375, 2398, 2406, 2397, 2387, 2413, 2536, 2800, 3208, 3716, 4262, 4788, 5258, 5650, 5944, 6133, 6224, 6228, 6161, 6032, 5853, 5637, 5400, },
/* LAT: -30 */ {  4883, 4644, 4408, 4175, 3950, 3732, 3514, 3283, 3028, 2753, 2494, 2309, 2234, 2256, 2323, 2396, 2464, 2518, 2545, 2558, 2619, 2805, 3155, 3635, 4164, 4664, 5088, 5410, 5616, 5718, 5745, 5719, 5641, 5510, 5332, 5117, 4883, },
/* LAT: -20 */ {  4323, 4113, 3907, 3704, 3508, 3326, 3158, 2996, 2820, 2623, 2432, 2295, 2249, 2287, 2374, 2485, 2615, 2746, 2842, 2882, 2905, 2991, 3218, 3592, 4039, 4467, 4822, 5063, 5173, 5182, 5152, 5105, 5023, 4896, 4730, 4534, 4323, },
/* LAT: -10 */ {  3791, 3633, 3483, 3337, 3202, 3082, 2980, 2890, 2795, 2683, 2561, 2460, 2408, 2426, 2507, 2634, 2790, 2952, 3082, 3150, 3166, 3187, 3301, 3544, 3865, 4187, 4456, 4625, 4665, 4614, 4546, 4481, 4392, 4267, 4119, 3957, 3791, },
/* LAT:   0 */ {  3413, 3322, 3240, 3168, 3113, 3076, 3051, 3035, 3015, 2969, 2891, 2795, 2710, 2672, 2707, 2806, 2938, 3075, 3194, 3272, 3306, 3326, 3395, 3546, 3750, 3963, 4145, 4255, 4264, 4199, 4111, 4018, 3905, 3774, 3641, 3518, 3413, },
/* LAT:  10 */ {  3283, 3254, 3235, 3233, 3259, 3307, 3365, 3422, 3461, 3453, 3385, 3270, 3140, 3039, 3006, 3042, 3121, 3219, 3320, 3406, 3471, 3532, 3617, 3731, 3863, 3999, 4119, 4192, 4197, 4138, 4030, 3887, 3726, 3567, 3433, 3338, 3283, },
/* LAT:  20 */ {  3400, 3405, 3434, 3490, 3584, 3707, 3839, 3959, 4043, 4057, 3984, 3842, 3672, 3527, 3444, 3427, 3459, 3529, 3624, 3720, 3810, 3907, 4016, 4126, 4232, 4341, 4442, 4509, 4523, 4467, 4332, 4132, 3907, 3698, 3536, 3437, 3400, },
/* LAT:  30 */ {  3724, 3734, 3792, 3895, 4040, 4214, 4392, 4550, 4660, 4688, 4616, 4459, 4266, 4096, 3985, 3934, 3933, 3981, 4065, 4161, 4257, 4360, 4476, 4593, 4712, 4835, 4954, 5041, 5070, 5016, 4863, 4626, 4353, 4100, 3901, 3775, 3724, },
/* LAT:  40 */ {  4223, 4226, 4296, 4423, 4593, 4783, 4968, 5126, 5231, 5257, 5188, 5037, 4846, 4667, 4534, 4454, 4424, 4442, 4500, 4577, 4661, 4756, 4870, 5004, 5154, 5314, 5464, 5575, 5618, 5568, 5415, 5179, 4905, 4645, 4435, 4292, 4223, },
/* LAT:  50 */ {  4831, 4830, 4892, 5007, 5156, 5317, 5467, 5588, 5660, 5668, 5603, 5473, 5307, 5140, 4999, 4898, 4840, 4825, 4847, 4895, 4962, 5051, 5169, 5318, 5493, 5677, 5844, 5964, 6013, 5972, 5844, 5648, 5421, 5204, 5024, 4897, 4831, },
/* LAT:  60 */ {  5391, 5385, 5419, 5487, 5576, 5671, 5757, 5821, 5852, 5841, 5784, 5687, 5564, 5433, 5312, 5213, 5145, 5109, 5105, 5131, 5186, 5271, 5387, 5533, 5700, 5869, 6018, 6125, 6172, 6152, 6070, 5943, 5795, 5650, 5528, 5440, 5391, },
/* LAT:  70 */ {  5726, 5710, 5712, 5727, 5751, 5778, 5801, 5813, 5811, 5789, 5747, 5688, 5615, 5537, 5462, 5396, 5347, 5318, 5312, 5331, 5374, 5443, 5534, 5642, 5760, 5875, 5976, 6050, 6091, 6095, 6066, 6012, 5945, 5874, 5810, 5759, 5726, },
/* LAT:  80 */ {  5788, 5773, 5759, 5749, 5739, 5730, 5720, 5708, 5692, 5672, 5649, 5623, 5594, 5566, 5540, 5519, 5504, 5498, 5502, 5517, 5542, 5577, 5619, 5667, 5717, 5766, 5809, 5844, 5868, 5882, 5885, 5878, 5865, 5847, 5827, 5807, 5788, },
/* LAT:  90 */ {  5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, 5677, },
};
