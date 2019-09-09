#include "L1Trigger/CSCTriggerPrimitives/interface/CSCUpgradeMotherboardLUT.h"

CSCMotherboardLUTME11::CSCMotherboardLUTME11() {
  // Keep in mind that ME1A is considered an extension of ME1B
  // This means that ME1A half-strips start at 128 and end at 223
  lut_wg_vs_hs_me1a = {{128, 223}, {128, 223}, {128, 223}, {128, 223}, {128, 223}, {128, 223}, {128, 223}, {128, 223},
                       {128, 223}, {128, 223}, {128, 223}, {128, 223}, {128, 205}, {128, 189}, {128, 167}, {128, 150},
                       {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                       {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                       {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                       {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1}};
  // When the half-strips are triple-ganged, (Run-1)
  // ME1A half-strips go from 128 to 159
  lut_wg_vs_hs_me1ag = {{128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159},
                        {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 159}, {128, 150},
                        {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                        {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                        {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},
                        {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1},   {-1, -1}};
  // ME1B half-strips start at 0 and end at 127
  lut_wg_vs_hs_me1b = {{-1, -1}, {-1, -1}, {-1, -1},   {-1, -1},  {-1, -1},  {-1, -1},  {-1, -1}, {-1, -1},
                       {-1, -1}, {-1, -1}, {100, 127}, {73, 127}, {47, 127}, {22, 127}, {0, 127}, {0, 127},
                       {0, 127}, {0, 127}, {0, 127},   {0, 127},  {0, 127},  {0, 127},  {0, 127}, {0, 127},
                       {0, 127}, {0, 127}, {0, 127},   {0, 127},  {0, 127},  {0, 127},  {0, 127}, {0, 127},
                       {0, 127}, {0, 127}, {0, 127},   {0, 127},  {0, 127},  {0, 127},  {0, 127}, {0, 127},
                       {0, 127}, {0, 127}, {0, 127},   {0, 127},  {0, 105},  {0, 93},   {0, 78},  {0, 63}};
}

bool CSCMotherboardLUTME11::doesALCTCrossCLCT(const CSCALCTDigi &a,
                                              const CSCCLCTDigi &c,
                                              int theEndcap,
                                              bool gangedME1a) const {
  if (!c.isValid() || !a.isValid())
    return false;
  int key_hs = c.getKeyStrip();
  int key_wg = a.getKeyWG();
  return doesWiregroupCrossStrip(key_wg, key_hs, theEndcap, gangedME1a);
}

bool CSCMotherboardLUTME11::doesWiregroupCrossStrip(int key_wg, int key_hs, int theEndcap, bool gangedME1a) const {
  // ME1/a half-strip starts at 128
  if (key_hs > CSCConstants::MAX_HALF_STRIP_ME1B) {
    if (!gangedME1a) {
      // wrap around ME11 HS number for -z endcap
      if (theEndcap == 2) {
        // first subtract 128
        key_hs -= 1 + CSCConstants::MAX_HALF_STRIP_ME1B;
        // flip the HS
        key_hs = CSCConstants::MAX_HALF_STRIP_ME1A_UNGANGED - key_hs;
        // then add 128 again
        key_hs += 1 + CSCConstants::MAX_HALF_STRIP_ME1B;
      }
      if (key_hs >= lut_wg_vs_hs_me1a[key_wg][0] && key_hs <= lut_wg_vs_hs_me1a[key_wg][1])
        return true;
      return false;
    } else {
      // wrap around ME11 HS number for -z endcap
      if (theEndcap == 2) {
        // first subtract 128
        key_hs -= 1 + CSCConstants::MAX_HALF_STRIP_ME1B;
        // flip the HS
        key_hs = CSCConstants::MAX_HALF_STRIP_ME1A_GANGED - key_hs;
        // then add 128 again
        key_hs += 1 + CSCConstants::MAX_HALF_STRIP_ME1B;
      }
      if (key_hs >= lut_wg_vs_hs_me1ag[key_wg][0] && key_hs <= lut_wg_vs_hs_me1ag[key_wg][1])
        return true;
      return false;
    }
  }
  // ME1/b half-strip ends at 127
  if (key_hs <= CSCConstants::MAX_HALF_STRIP_ME1B) {
    if (theEndcap == 2)
      key_hs = CSCConstants::MAX_HALF_STRIP_ME1B - key_hs;
    if (key_hs >= lut_wg_vs_hs_me1b[key_wg][0] && key_hs <= lut_wg_vs_hs_me1b[key_wg][1])
      return true;
  }
  return false;
}

CSCGEMMotherboardLUT::CSCGEMMotherboardLUT()
    : lut_wg_eta_odd(0),
      lut_wg_eta_even(0),
      lut_pt_vs_dphi_gemcsc(0)

      ,
      gem_roll_eta_limits_odd_l1(0),
      gem_roll_eta_limits_odd_l2(0),
      gem_roll_eta_limits_even_l1(0),
      gem_roll_eta_limits_even_l2(0)

      ,
      csc_wg_to_gem_roll_odd_l1(0),
      csc_wg_to_gem_roll_odd_l2(0),
      csc_wg_to_gem_roll_even_l1(0),
      csc_wg_to_gem_roll_even_l2(0) {}

std::vector<std::pair<int, int> > CSCGEMMotherboardLUT::get_csc_wg_to_gem_roll(Parity par, int layer) const {
  if (par == Parity::Even) {
    return layer == 1 ? csc_wg_to_gem_roll_even_l1 : csc_wg_to_gem_roll_even_l2;
  } else {
    return layer == 1 ? csc_wg_to_gem_roll_odd_l1 : csc_wg_to_gem_roll_odd_l2;
  }
}

std::vector<int> CSCGEMMotherboardLUT::get_gem_roll_to_csc_wg(Parity par, int layer) const {
  if (par == Parity::Even) {
    return layer == 1 ? gem_roll_to_csc_wg_even_l1 : gem_roll_to_csc_wg_even_l2;
  } else {
    return layer == 1 ? gem_roll_to_csc_wg_odd_l1 : gem_roll_to_csc_wg_odd_l2;
  }
}

std::vector<int> CSCGEMMotherboardLUTME11::get_gem_pad_to_csc_hs(Parity par, enum CSCPart p) const {
  if (p == CSCPart::ME1A) {
    return par == Parity::Even ? gem_pad_to_csc_hs_me1a_even : gem_pad_to_csc_hs_me1a_odd;
  } else {
    return par == Parity::Even ? gem_pad_to_csc_hs_me1b_even : gem_pad_to_csc_hs_me1b_odd;
  }
}

std::vector<int> CSCGEMMotherboardLUTME21::get_gem_pad_to_csc_hs(Parity par, enum CSCPart p) const {
  return par == Parity::Even ? gem_pad_to_csc_hs_even : gem_pad_to_csc_hs_odd;
}

std::vector<std::pair<int, int> > CSCGEMMotherboardLUTME21::get_csc_hs_to_gem_pad(Parity par, enum CSCPart p) const {
  return par == Parity::Even ? csc_hs_to_gem_pad_even : csc_hs_to_gem_pad_odd;
}

std::vector<std::pair<int, int> > CSCGEMMotherboardLUTME11::get_csc_hs_to_gem_pad(Parity par, enum CSCPart p) const {
  if (p == CSCPart::ME1A) {
    return par == Parity::Even ? csc_hs_to_gem_pad_me1a_even : csc_hs_to_gem_pad_me1a_odd;
  } else {
    return par == Parity::Even ? csc_hs_to_gem_pad_me1b_even : csc_hs_to_gem_pad_me1b_odd;
  }
}

CSCGEMMotherboardLUT::~CSCGEMMotherboardLUT() {}

CSCGEMMotherboardLUTME11::CSCGEMMotherboardLUTME11() : CSCGEMMotherboardLUT() {
  lut_wg_eta_odd = {{2.4466, 2.45344},  {2.33403, 2.43746}, {2.28122, 2.38447}, {2.23122, 2.33427}, {2.18376, 2.2866},
                    {2.1386, 2.24124},  {2.09556, 2.19796}, {2.05444, 2.15662}, {2.01511, 2.11704}, {1.97741, 2.07909},
                    {1.94124, 2.04266}, {1.90649, 2.00764}, {1.87305, 1.97392}, {1.84084, 1.94143}, {1.80978, 1.91008},
                    {1.77981, 1.87981}, {1.75086, 1.85055}, {1.72286, 1.82225}, {1.69577, 1.79484}, {1.66954, 1.76828},
                    {1.64412, 1.74253}, {1.61946, 1.71754}, {1.60584, 1.69328}, {1.60814, 1.6697}};

  lut_wg_eta_even = {{2.3981, 2.40492},  {2.28578, 2.38883}, {2.23311, 2.33595}, {2.18324, 2.28587}, {2.13592, 2.23831},
                     {2.09091, 2.19306}, {2.048, 2.14991},   {2.00704, 2.10868}, {1.96785, 2.06923}, {1.93031, 2.03141},
                     {1.8943, 1.9951},   {1.8597, 1.96021},  {1.82642, 1.92663}, {1.79438, 1.89427}, {1.76349, 1.86306},
                     {1.73369, 1.83293}, {1.70491, 1.80382}, {1.67709, 1.77566}, {1.65018, 1.7484},  {1.62413, 1.72199},
                     {1.59889, 1.69639}, {1.57443, 1.67155}, {1.56088, 1.64745}, {1.5631, 1.62403}};

  /*
    98% acceptance cuts of the GEM-CSC bending angle in ME1b
    for various pT thresholds and for even/odd chambers
   */
  lut_pt_vs_dphi_gemcsc = {{3, 0.03971647, 0.01710244},
                           {5, 0.02123785, 0.00928431},
                           {7, 0.01475524, 0.00650928},
                           {10, 0.01023299, 0.00458796},
                           {15, 0.00689220, 0.00331313},
                           {20, 0.00535176, 0.00276152},
                           {30, 0.00389050, 0.00224959},
                           {40, 0.00329539, 0.00204670}};

  gem_roll_eta_limits_odd_l1 = {{1.61082, 1.67865},
                                {1.67887, 1.7528},
                                {1.75303, 1.82091},
                                {1.82116, 1.89486},
                                {1.89513, 1.96311},
                                {1.9634, 2.037},
                                {2.03732, 2.10527},
                                {2.10562, 2.17903}};

  gem_roll_eta_limits_odd_l2 = {{1.61705, 1.68494},
                                {1.68515, 1.75914},
                                {1.75938, 1.8273},
                                {1.82756, 1.9013},
                                {1.90158, 1.96959},
                                {1.96988, 2.04352},
                                {2.04384, 2.11181},
                                {2.11216, 2.1856}};

  gem_roll_eta_limits_even_l1 = {{1.55079, 1.62477},
                                 {1.62497, 1.70641},
                                 {1.70663, 1.78089},
                                 {1.78113, 1.86249},
                                 {1.86275, 1.9371},
                                 {1.93739, 2.01855},
                                 {2.01887, 2.09324},
                                 {2.09358, 2.17456}};

  gem_roll_eta_limits_even_l2 = {{1.55698, 1.63103},
                                 {1.63123, 1.71275},
                                 {1.71297, 1.78728},
                                 {1.78752, 1.86894},
                                 {1.86921, 1.94359},
                                 {1.94388, 2.02509},
                                 {2.02541, 2.09981},
                                 {2.10015, 2.18115}};

  csc_wg_to_gem_roll_odd_l1 = {{-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99},
                               {-99, -99}, {8, -99},   {8, -99},   {8, -99},   {7, -99},   {7, -99},   {7, 8},
                               {7, 8},     {6, 8},     {6, 7},     {6, 7},     {6, 7},     {5, 7},     {5, 6},
                               {5, 6},     {4, 6},     {4, 6},     {4, 5},     {4, 5},     {4, 5},     {3, 5},
                               {3, 5},     {3, 4},     {3, 4},     {2, 4},     {2, 4},     {2, 4},     {2, 3},
                               {2, 3},     {2, 3},     {1, 3},     {1, 3},     {1, 2},     {1, 2},     {1, 2},
                               {1, 2},     {1, 2},     {1, 2},     {1, 1},     {1, 1},     {1, 1}};

  csc_wg_to_gem_roll_odd_l2 = {{-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99},
                               {-99, -99}, {8, -99},   {8, -99},   {8, -99},   {7, -99},   {7, -99},   {7, 8},
                               {7, 8},     {6, 8},     {6, 7},     {6, 7},     {6, 7},     {5, 7},     {5, 6},
                               {5, 6},     {4, 6},     {4, 6},     {4, 5},     {4, 5},     {4, 5},     {3, 5},
                               {3, 5},     {3, 4},     {3, 4},     {2, 4},     {2, 4},     {2, 4},     {2, 3},
                               {2, 3},     {2, 3},     {1, 3},     {1, 3},     {1, 2},     {1, 2},     {1, 2},
                               {1, 2},     {1, 2},     {1, 2},     {1, 1},     {1, 1},     {1, 1}};

  csc_wg_to_gem_roll_even_l1 = {
      {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {8, -99}, {8, -99}, {8, -99}, {8, -99},
      {7, -99},   {7, 8},     {7, 8},     {6, 8},     {6, 8},     {6, 7},     {6, 7},   {5, 7},   {5, 6},   {5, 6},
      {5, 6},     {5, 6},     {4, 6},     {4, 5},     {4, 5},     {4, 5},     {4, 5},   {3, 5},   {3, 4},   {3, 4},
      {3, 4},     {2, 4},     {2, 4},     {2, 3},     {2, 3},     {2, 3},     {2, 3},   {1, 3},   {1, 3},   {1, 2},
      {1, 2},     {1, 2},     {1, 2},     {1, 2},     {1, 2},     {1, 1},     {1, 1},   {1, 1}};

  csc_wg_to_gem_roll_even_l2 = {
      {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {-99, -99}, {8, -99}, {8, -99}, {8, -99}, {8, -99},
      {7, -99},   {7, 8},     {7, 8},     {6, 8},     {6, 8},     {6, 7},     {6, 7},   {5, 7},   {5, 6},   {5, 6},
      {5, 6},     {5, 6},     {4, 6},     {4, 5},     {4, 5},     {4, 5},     {4, 5},   {3, 5},   {3, 4},   {3, 4},
      {3, 4},     {2, 4},     {2, 4},     {2, 3},     {2, 3},     {2, 3},     {2, 3},   {1, 3},   {1, 3},   {1, 2},
      {1, 2},     {1, 2},     {1, 2},     {1, 2},     {1, 2},     {1, 1},     {1, 1},   {1, 1}};

  gem_roll_to_csc_wg_odd_l1 = {};
  gem_roll_to_csc_wg_odd_l2 = {};
  gem_roll_to_csc_wg_even_l1 = {};
  gem_roll_to_csc_wg_even_l2 = {};

  gem_pad_to_csc_hs_me1a_odd = {
      93, 92, 92, 92, 91, 91, 90, 90, 89, 89, 88, 88, 87, 87, 86, 86, 85, 85, 84, 84, 83, 83, 83, 82, 82, 81, 81, 80,
      80, 79, 79, 78, 78, 77, 77, 76, 76, 75, 75, 74, 74, 73, 73, 73, 72, 72, 71, 71, 70, 70, 69, 69, 68, 68, 67, 67,
      66, 66, 65, 65, 64, 64, 63, 63, 63, 62, 62, 61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 56, 56, 55, 55, 54, 54, 53,
      53, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44, 43, 43, 43, 42, 42, 41, 41, 40,
      40, 39, 39, 38, 38, 37, 37, 36, 36, 35, 35, 34, 34, 33, 33, 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27,
      26, 26, 25, 25, 24, 24, 23, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13,
      13, 13, 12, 12, 11, 11, 10, 10, 9,  9,  8,  8,  7,  7,  6,  6,  5,  5,  4,  4,  4,  3,  3,  2};

  gem_pad_to_csc_hs_me1b_odd = {
      123, 123, 122, 121, 121, 120, 119, 119, 118, 118, 117, 116, 116, 115, 114, 114, 113, 113, 112, 111, 111, 110,
      110, 109, 108, 108, 107, 106, 106, 105, 105, 104, 103, 103, 102, 101, 101, 100, 100, 99,  98,  98,  97,  96,
      96,  95,  95,  94,  93,  93,  92,  91,  91,  90,  90,  89,  88,  88,  87,  86,  86,  85,  85,  84,  83,  83,
      82,  81,  81,  80,  80,  79,  78,  78,  77,  76,  76,  75,  75,  74,  73,  73,  72,  71,  71,  70,  70,  69,
      68,  68,  67,  66,  66,  65,  65,  64,  63,  63,  62,  61,  61,  60,  60,  59,  58,  58,  57,  56,  56,  55,
      55,  54,  53,  53,  52,  51,  51,  50,  50,  49,  48,  48,  47,  46,  46,  45,  45,  44,  43,  43,  42,  41,
      41,  40,  40,  39,  38,  38,  37,  36,  36,  35,  35,  34,  33,  33,  32,  31,  31,  30,  30,  29,  28,  28,
      27,  26,  26,  25,  25,  24,  23,  23,  22,  21,  21,  20,  20,  19,  18,  18,  17,  16,  16,  15,  15,  14,
      13,  13,  12,  11,  11,  10,  10,  9,   8,   8,   7,   7,   6,   5,   5,   4};

  gem_pad_to_csc_hs_me1a_even = {
      2,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9,  10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15,
      15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28,
      29, 29, 30, 30, 31, 31, 32, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42,
      42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 52, 53, 53, 54, 54, 55,
      55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68,
      69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 81, 82,
      82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 91, 92, 92, 93};

  gem_pad_to_csc_hs_me1b_even = {
      4,   4,   5,   6,   6,   7,   7,   8,   9,   9,   10,  11,  11,  12,  12,  13,  14,  14,  15,  16,  16,  17,
      17,  18,  19,  19,  20,  20,  21,  22,  22,  23,  24,  24,  25,  25,  26,  27,  27,  28,  29,  29,  30,  30,
      31,  32,  32,  33,  34,  34,  35,  35,  36,  37,  37,  38,  39,  39,  40,  40,  41,  42,  42,  43,  44,  44,
      45,  45,  46,  47,  47,  48,  49,  49,  50,  50,  51,  52,  52,  53,  54,  54,  55,  55,  56,  57,  57,  58,
      59,  59,  60,  60,  61,  62,  62,  63,  64,  64,  65,  65,  66,  67,  67,  68,  69,  69,  70,  70,  71,  72,
      72,  73,  74,  74,  75,  75,  76,  77,  77,  78,  79,  79,  80,  80,  81,  82,  82,  83,  84,  84,  85,  85,
      86,  87,  87,  88,  89,  89,  90,  90,  91,  92,  92,  93,  94,  94,  95,  95,  96,  97,  97,  98,  99,  99,
      100, 100, 101, 102, 102, 103, 104, 104, 105, 105, 106, 107, 107, 108, 109, 109, 110, 110, 111, 112, 112, 113,
      114, 114, 115, 115, 116, 117, 117, 118, 119, 119, 120, 120, 121, 122, 122, 123};

  csc_hs_to_gem_pad_me1a_odd = {
      {192, 192}, {192, 192}, {192, 192}, {190, 191}, {188, 188}, {185, 186}, {183, 184}, {181, 182}, {179, 180},
      {177, 178}, {175, 176}, {173, 174}, {171, 172}, {169, 169}, {166, 167}, {164, 165}, {162, 163}, {160, 161},
      {158, 159}, {156, 157}, {154, 155}, {152, 153}, {150, 151}, {148, 148}, {145, 146}, {143, 144}, {141, 142},
      {139, 140}, {137, 138}, {135, 136}, {133, 134}, {131, 132}, {129, 130}, {127, 127}, {124, 125}, {122, 123},
      {120, 121}, {118, 119}, {116, 117}, {114, 115}, {112, 113}, {110, 111}, {108, 109}, {106, 106}, {103, 104},
      {101, 102}, {99, 100},  {97, 98},   {95, 96},   {93, 94},   {91, 92},   {89, 90},   {87, 88},   {85, 85},
      {82, 83},   {80, 81},   {78, 79},   {76, 77},   {74, 75},   {72, 73},   {70, 71},   {68, 69},   {66, 67},
      {64, 64},   {61, 62},   {59, 60},   {57, 58},   {55, 56},   {53, 54},   {51, 52},   {49, 50},   {47, 48},
      {45, 46},   {43, 43},   {40, 41},   {38, 39},   {36, 37},   {34, 35},   {32, 33},   {30, 31},   {28, 29},
      {26, 27},   {24, 25},   {22, 22},   {19, 20},   {17, 18},   {15, 16},   {13, 14},   {11, 12},   {9, 10},
      {7, 8},     {5, 6},     {3, 3},     {1, 1},     {1, 1},     {1, 1}};

  csc_hs_to_gem_pad_me1a_even = {
      {1, 1},     {1, 1},     {1, 1},     {2, 3},     {4, 5},     {7, 7},     {9, 10},    {11, 12},   {13, 14},
      {15, 16},   {17, 18},   {19, 20},   {21, 22},   {23, 24},   {26, 26},   {28, 29},   {30, 31},   {32, 33},
      {34, 35},   {36, 37},   {38, 39},   {40, 41},   {42, 43},   {44, 45},   {47, 47},   {49, 50},   {51, 52},
      {53, 54},   {55, 56},   {57, 58},   {59, 60},   {61, 62},   {63, 64},   {65, 66},   {68, 68},   {70, 71},
      {72, 73},   {74, 75},   {76, 77},   {78, 79},   {80, 81},   {82, 83},   {84, 85},   {86, 87},   {88, 89},
      {91, 91},   {93, 94},   {95, 96},   {97, 98},   {99, 100},  {101, 102}, {103, 104}, {105, 106}, {107, 108},
      {109, 110}, {112, 112}, {114, 115}, {116, 117}, {118, 119}, {120, 121}, {122, 123}, {124, 125}, {126, 127},
      {128, 129}, {130, 131}, {133, 133}, {135, 136}, {137, 138}, {139, 140}, {141, 142}, {143, 144}, {145, 146},
      {147, 148}, {149, 150}, {151, 152}, {154, 154}, {156, 157}, {158, 159}, {160, 161}, {162, 163}, {164, 165},
      {166, 167}, {168, 169}, {170, 171}, {172, 173}, {175, 175}, {177, 178}, {179, 180}, {181, 182}, {183, 184},
      {185, 186}, {187, 188}, {189, 190}, {191, 192}, {192, 192}, {192, 192}};

  csc_hs_to_gem_pad_me1b_odd = {
      {192, 192}, {192, 192}, {192, 192}, {192, 192}, {192, 192}, {190, 191}, {189, 189}, {187, 188}, {185, 186},
      {184, 184}, {182, 183}, {180, 181}, {179, 179}, {177, 178}, {176, 176}, {174, 175}, {172, 173}, {171, 171},
      {169, 170}, {168, 168}, {166, 167}, {164, 165}, {163, 163}, {161, 162}, {160, 160}, {158, 159}, {156, 157},
      {155, 155}, {153, 154}, {152, 152}, {150, 151}, {148, 149}, {147, 147}, {145, 146}, {144, 144}, {142, 143},
      {140, 141}, {139, 139}, {137, 138}, {136, 136}, {134, 135}, {132, 133}, {131, 131}, {129, 130}, {128, 128},
      {126, 127}, {124, 125}, {123, 123}, {121, 122}, {120, 120}, {118, 119}, {116, 117}, {115, 115}, {113, 114},
      {112, 112}, {110, 111}, {108, 109}, {107, 107}, {105, 106}, {104, 104}, {102, 103}, {100, 101}, {99, 99},
      {97, 98},   {96, 96},   {94, 95},   {92, 93},   {91, 91},   {89, 90},   {88, 88},   {86, 87},   {84, 85},
      {83, 83},   {81, 82},   {80, 80},   {78, 79},   {76, 77},   {75, 75},   {73, 74},   {72, 72},   {70, 71},
      {68, 69},   {67, 67},   {65, 66},   {64, 64},   {62, 63},   {60, 61},   {59, 59},   {57, 58},   {56, 56},
      {54, 55},   {52, 53},   {51, 51},   {49, 50},   {48, 48},   {46, 47},   {44, 45},   {43, 43},   {41, 42},
      {40, 40},   {38, 39},   {36, 37},   {35, 35},   {33, 34},   {32, 32},   {30, 31},   {28, 29},   {27, 27},
      {25, 26},   {24, 24},   {22, 23},   {20, 21},   {19, 19},   {17, 18},   {15, 16},   {14, 14},   {12, 13},
      {11, 11},   {9, 10},    {7, 8},     {6, 6},     {4, 5},     {3, 3},     {1, 2},     {1, 1},     {1, 1},
      {1, 1},     {1, 1}};

  csc_hs_to_gem_pad_me1b_even = {
      {1, 1},     {1, 1},     {1, 1},     {1, 1},     {1, 2},     {3, 4},     {5, 5},     {6, 7},     {8, 8},
      {9, 10},    {11, 12},   {13, 13},   {14, 15},   {16, 16},   {17, 18},   {19, 20},   {21, 21},   {22, 23},
      {24, 24},   {25, 26},   {27, 28},   {29, 29},   {30, 31},   {32, 32},   {33, 34},   {35, 36},   {37, 37},
      {38, 39},   {40, 40},   {41, 42},   {43, 44},   {45, 45},   {46, 47},   {48, 48},   {49, 50},   {51, 52},
      {53, 53},   {54, 55},   {56, 56},   {57, 58},   {59, 60},   {61, 61},   {62, 63},   {64, 64},   {65, 66},
      {67, 68},   {69, 69},   {70, 71},   {72, 72},   {73, 74},   {75, 76},   {77, 77},   {78, 79},   {80, 80},
      {81, 82},   {83, 84},   {85, 85},   {86, 87},   {88, 88},   {89, 90},   {91, 92},   {93, 93},   {94, 95},
      {96, 96},   {97, 98},   {99, 100},  {101, 101}, {102, 103}, {104, 104}, {105, 106}, {107, 108}, {109, 109},
      {110, 111}, {112, 112}, {113, 114}, {115, 116}, {117, 117}, {118, 119}, {120, 120}, {121, 122}, {123, 124},
      {125, 125}, {126, 127}, {128, 128}, {129, 130}, {131, 132}, {133, 133}, {134, 135}, {136, 136}, {137, 138},
      {139, 140}, {141, 141}, {142, 143}, {144, 144}, {145, 146}, {147, 148}, {149, 149}, {150, 151}, {152, 152},
      {153, 154}, {155, 156}, {157, 157}, {158, 159}, {160, 160}, {161, 162}, {163, 164}, {165, 165}, {166, 167},
      {168, 169}, {170, 170}, {171, 172}, {173, 173}, {174, 175}, {176, 177}, {178, 178}, {179, 180}, {181, 181},
      {182, 183}, {184, 185}, {186, 186}, {187, 188}, {189, 189}, {190, 191}, {192, 192}, {192, 192}, {192, 192},
      {192, 192}, {192, 192}};
}

CSCGEMMotherboardLUTME11::~CSCGEMMotherboardLUTME11() {}

CSCGEMMotherboardLUTME21::CSCGEMMotherboardLUTME21() : CSCGEMMotherboardLUT() {
  lut_wg_eta_odd = {
      {-2.43106, -2.43106}, {-2.41441, -2.41441}, {-2.39399, -2.39399}, {-2.374, -2.374},     {-2.35442, -2.35442},
      {-2.33524, -2.33524}, {-2.31644, -2.31644}, {-2.29801, -2.29801}, {-2.27993, -2.27993}, {-2.26219, -2.26219},
      {-2.24478, -2.24478}, {-2.22768, -2.22768}, {-2.2109, -2.2109},   {-2.19441, -2.19441}, {-2.17821, -2.17821},
      {-2.16228, -2.16228}, {-2.14663, -2.14663}, {-2.13123, -2.13123}, {-2.11609, -2.11609}, {-2.1012, -2.1012},
      {-2.08654, -2.08654}, {-2.07211, -2.07211}, {-2.06002, -2.06002}, {-2.04392, -2.04392}, {-2.03015, -2.03015},
      {-2.01659, -2.01659}, {-2.00322, -2.00322}, {-1.99005, -1.99005}, {-1.97707, -1.97707}, {-1.96428, -1.96428},
      {-1.95166, -1.95166}, {-1.93922, -1.93922}, {-1.92696, -1.92696}, {-1.91306, -1.91306}, {-1.89878, -1.89878},
      {-1.88474, -1.88474}, {-1.87091, -1.87091}, {-1.8573, -1.8573},   {-1.8439, -1.8439},   {-1.8307, -1.8307},
      {-1.81931, -1.81931}, {-1.80489, -1.80489}, {-1.79227, -1.79227}, {-1.77984, -1.77984}, {-1.76758, -1.76758},
      {-1.7555, -1.7555},   {-1.74359, -1.74359}, {-1.73184, -1.73184}, {-1.72026, -1.72026}, {-1.70883, -1.70883},
      {-1.69756, -1.69756}, {-1.68644, -1.68644}, {-1.67546, -1.67546}, {-1.66463, -1.66463}, {-1.65394, -1.65394},
      {-1.64339, -1.64339}};

  lut_wg_eta_even = {
      {-2.40118, -2.40118}, {-2.38455, -2.38455}, {-2.36416, -2.36416}, {-2.3442, -2.3442},   {-2.32465, -2.32465},
      {-2.3055, -2.3055},   {-2.28673, -2.28673}, {-2.26833, -2.26833}, {-2.25028, -2.25028}, {-2.23257, -2.23257},
      {-2.21519, -2.21519}, {-2.19813, -2.19813}, {-2.18138, -2.18138}, {-2.16492, -2.16492}, {-2.14875, -2.14875},
      {-2.13286, -2.13286}, {-2.11723, -2.11723}, {-2.10187, -2.10187}, {-2.08676, -2.08676}, {-2.0719, -2.0719},
      {-2.05727, -2.05727}, {-2.04288, -2.04288}, {-2.03082, -2.03082}, {-2.01476, -2.01476}, {-2.00102, -2.00102},
      {-1.98749, -1.98749}, {-1.97416, -1.97416}, {-1.96102, -1.96102}, {-1.94808, -1.94808}, {-1.93532, -1.93532},
      {-1.92274, -1.92274}, {-1.91033, -1.91033}, {-1.8981, -1.8981},   {-1.88424, -1.88424}, {-1.87001, -1.87001},
      {-1.85601, -1.85601}, {-1.84222, -1.84222}, {-1.82866, -1.82866}, {-1.8153, -1.8153},   {-1.80215, -1.80215},
      {-1.7908, -1.7908},   {-1.77643, -1.77643}, {-1.76385, -1.76385}, {-1.75146, -1.75146}, {-1.73925, -1.73925},
      {-1.72722, -1.72722}, {-1.71535, -1.71535}, {-1.70365, -1.70365}, {-1.69211, -1.69211}, {-1.68073, -1.68073},
      {-1.66951, -1.66951}, {-1.65843, -1.65843}, {-1.64751, -1.64751}, {-1.63672, -1.63672}, {-1.62608, -1.62608},
      {-1.61558, -1.61558}};

  /*
    98% acceptance cuts of the GEM-CSC bending angle in ME21
    for various pT thresholds and for even/odd chambers
   */
  lut_pt_vs_dphi_gemcsc = {{3, 0.01832829, 0.01003643},
                           {5, 0.01095490, 0.00631625},
                           {7, 0.00786026, 0.00501017},
                           {10, 0.00596349, 0.00414560},
                           {15, 0.00462411, 0.00365550},
                           {20, 0.00435298, 0.00361550},
                           {30, 0.00465160, 0.00335700},
                           {40, 0.00372145, 0.00366262}};

  // roll 1 through 8
  gem_roll_eta_limits_odd_l1 = {{1.64351, 1.70857},
                                {1.70864, 1.77922},
                                {1.79143, 1.86953},
                                {1.8696, 1.95538},
                                {1.97034, 2.06691},
                                {2.06701, 2.17505},
                                {2.19413, 2.31912},
                                {2.31924, 2.46333}};

  gem_roll_eta_limits_odd_l2 = {{1.64764, 1.71913},
                                {1.71919, 1.79737},
                                {1.80979, 1.89713},
                                {1.8972, 1.99417},
                                {2.00973, 2.10042},
                                {2.10052, 2.20119},
                                {2.22072, 2.33613},
                                {2.33625, 2.46772}};

  gem_roll_eta_limits_even_l1 = {{1.6407, 1.70574},
                                 {1.70581, 1.77636},
                                 {1.78857, 1.86665},
                                 {1.86672, 1.95247},
                                 {1.96743, 2.06399},
                                 {2.06408, 2.1721},
                                 {2.19118, 2.31615},
                                 {2.31627, 2.46036}};

  gem_roll_eta_limits_even_l2 = {{1.64485, 1.71631},
                                 {1.71637, 1.79453},
                                 {1.80694, 1.89425},
                                 {1.89433, 1.99127},
                                 {2.00683, 2.0975},
                                 {2.0976, 2.19825},
                                 {2.21778, 2.33317},
                                 {2.3333, 2.46475}};

  csc_wg_to_gem_roll_odd_l1 = {
          {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {7, 7}, {7, 7}, {7, 7},
          {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
          {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6},
          {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5},
          {5, 5}, {5, 5}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
          {4, 4}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3},
          {3, 3}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1},
          {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};

  csc_wg_to_gem_roll_odd_l2 = {
          {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
          {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {6, 6}, {6, 6}, {6, 6}, {6, 6},
          {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5},
          {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
          {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {3, 3}, {3, 3}, {3, 3},
          {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {2, 2}, {2, 2},
          {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1},
          {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};

  csc_wg_to_gem_roll_even_l1 = {
          {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
          {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {6, 6}, {6, 6}, {6, 6}, {6, 6},
          {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {5, 5}, {5, 5}, {5, 5}, {5, 5},
          {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {4, 4}, {4, 4},
          {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {3, 3}, {3, 3},
          {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {2, 2}, {2, 2}, {2, 2},
          {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1},
          {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};

  csc_wg_to_gem_roll_even_l2 = {
          {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {8, 8}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
          {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6},
          {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5},
          {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {5, 5}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4},
          {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3},
          {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2},
          {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1},
          {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};

  gem_pad_to_csc_hs_odd = {
      157, 157, 156, 156, 156, 155, 155, 154, 154, 154, 153, 153, 152, 152, 152, 151, 151, 150, 150, 150, 149, 149, 148,
      148, 148, 147, 147, 146, 146, 146, 145, 145, 144, 144, 144, 143, 143, 142, 142, 142, 141, 141, 140, 140, 140, 139,
      139, 138, 138, 138, 137, 137, 136, 136, 135, 135, 135, 134, 134, 133, 133, 133, 132, 132, 131, 131, 131, 130, 130,
      129, 129, 129, 128, 128, 127, 127, 127, 126, 126, 125, 125, 125, 124, 124, 123, 123, 122, 122, 122, 121, 121, 120,
      120, 120, 119, 119, 118, 118, 118, 117, 117, 116, 116, 116, 115, 115, 114, 114, 113, 113, 113, 112, 112, 111, 111,
      111, 110, 110, 109, 109, 109, 108, 108, 107, 107, 107, 106, 106, 105, 105, 104, 104, 104, 103, 103, 102, 102, 102,
      101, 101, 100, 100, 100, 99,  99,  98,  98,  97,  97,  97,  96,  96,  95,  95,  95,  94,  94,  93,  93,  93,  92,
      92,  91,  91,  90,  90,  90,  89,  89,  88,  88,  88,  87,  87,  86,  86,  86,  85,  85,  84,  84,  83,  83,  83,
      82,  82,  81,  81,  81,  80,  80,  79,  79,  79,  78,  78,  77,  77,  76,  76,  76,  75,  75,  74,  74,  74,  73,
      73,  72,  72,  72,  71,  71,  70,  70,  69,  69,  69,  68,  68,  67,  67,  67,  66,  66,  65,  65,  65,  64,  64,
      63,  63,  62,  62,  62,  61,  61,  60,  60,  60,  59,  59,  58,  58,  58,  57,  57,  56,  56,  55,  55,  55,  54,
      54,  53,  53,  53,  52,  52,  51,  51,  51,  50,  50,  49,  49,  48,  48,  48,  47,  47,  46,  46,  46,  45,  45,
      44,  44,  44,  43,  43,  42,  42,  42,  41,  41,  40,  40,  39,  39,  39,  38,  38,  37,  37,  37,  36,  36,  35,
      35,  35,  34,  34,  33,  33,  33,  32,  32,  31,  31,  31,  30,  30,  29,  29,  28,  28,  28,  27,  27,  26,  26,
      26,  25,  25,  24,  24,  24,  23,  23,  22,  22,  22,  21,  21,  20,  20,  20,  19,  19,  18,  18,  18,  17,  17,
      16,  16,  16,  15,  15,  14,  14,  14,  13,  13,  12,  12,  11,  11,  11,  10,  10,  9,   9,   9,   8,   8,   7,
      7,   7,   6,   6,   5,   5,   5,   4,   4,   3,   3,   3,   2,   2,   1,   1};

  gem_pad_to_csc_hs_even = {
      1,   1,   2,   2,   2,   3,   3,   4,   4,   4,   5,   5,   6,   6,   6,   7,   7,   8,   8,   8,   9,   9,   10,
      10,  10,  11,  11,  12,  12,  12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  17,  18,  18,  19,  19,
      19,  20,  20,  21,  21,  21,  22,  22,  23,  23,  23,  24,  24,  25,  25,  25,  26,  26,  27,  27,  27,  28,  28,
      29,  29,  29,  30,  30,  31,  31,  32,  32,  32,  33,  33,  34,  34,  34,  35,  35,  36,  36,  36,  37,  37,  38,
      38,  38,  39,  39,  40,  40,  40,  41,  41,  42,  42,  43,  43,  43,  44,  44,  45,  45,  45,  46,  46,  47,  47,
      47,  48,  48,  49,  49,  50,  50,  50,  51,  51,  52,  52,  52,  53,  53,  54,  54,  54,  55,  55,  56,  56,  56,
      57,  57,  58,  58,  59,  59,  59,  60,  60,  61,  61,  61,  62,  62,  63,  63,  63,  64,  64,  65,  65,  66,  66,
      66,  67,  67,  68,  68,  68,  69,  69,  70,  70,  70,  71,  71,  72,  72,  73,  73,  73,  74,  74,  75,  75,  75,
      76,  76,  77,  77,  77,  78,  78,  79,  79,  80,  80,  80,  81,  81,  82,  82,  82,  83,  83,  84,  84,  84,  85,
      85,  86,  86,  87,  87,  87,  88,  88,  89,  89,  89,  90,  90,  91,  91,  92,  92,  92,  93,  93,  94,  94,  94,
      95,  95,  96,  96,  96,  97,  97,  98,  98,  98,  99,  99,  100, 100, 101, 101, 101, 102, 102, 103, 103, 103, 104,
      104, 105, 105, 105, 106, 106, 107, 107, 108, 108, 108, 109, 109, 110, 110, 110, 111, 111, 112, 112, 112, 113, 113,
      114, 114, 115, 115, 115, 116, 116, 117, 117, 117, 118, 118, 119, 119, 119, 120, 120, 121, 121, 121, 122, 122, 123,
      123, 124, 124, 124, 125, 125, 126, 126, 126, 127, 127, 128, 128, 128, 129, 129, 130, 130, 130, 131, 131, 132, 132,
      132, 133, 133, 134, 134, 134, 135, 135, 136, 136, 137, 137, 137, 138, 138, 139, 139, 139, 140, 140, 141, 141, 141,
      142, 142, 143, 143, 143, 144, 144, 145, 145, 145, 146, 146, 147, 147, 147, 148, 148, 149, 149, 149, 150, 150, 151,
      151, 151, 152, 152, 153, 153, 153, 154, 154, 155, 155, 155, 156, 156, 157, 157};

  csc_hs_to_gem_pad_odd = {
      {384, 384}, {384, 384}, {381, 382}, {378, 380}, {376, 377}, {373, 375}, {371, 372}, {368, 370}, {366, 367},
      {363, 365}, {361, 362}, {358, 360}, {356, 357}, {354, 355}, {351, 353}, {349, 350}, {346, 348}, {344, 345},
      {341, 343}, {339, 340}, {336, 338}, {334, 335}, {331, 333}, {329, 330}, {326, 328}, {324, 325}, {321, 323},
      {319, 320}, {316, 318}, {314, 315}, {312, 313}, {309, 311}, {307, 308}, {304, 306}, {302, 303}, {299, 301},
      {297, 298}, {294, 296}, {292, 293}, {289, 291}, {287, 288}, {285, 286}, {282, 284}, {280, 281}, {277, 279},
      {275, 276}, {272, 274}, {270, 271}, {267, 269}, {265, 266}, {263, 264}, {260, 262}, {258, 259}, {255, 257},
      {253, 254}, {250, 252}, {248, 249}, {246, 247}, {243, 245}, {241, 242}, {238, 240}, {236, 237}, {233, 235},
      {231, 232}, {229, 230}, {226, 228}, {224, 225}, {221, 223}, {219, 220}, {216, 218}, {214, 215}, {212, 213},
      {209, 211}, {207, 208}, {204, 206}, {202, 203}, {199, 201}, {197, 198}, {195, 196}, {192, 194}, {190, 191},
      {187, 189}, {185, 186}, {182, 184}, {180, 181}, {178, 179}, {175, 177}, {173, 174}, {170, 172}, {168, 169},
      {165, 167}, {163, 164}, {161, 162}, {158, 160}, {156, 157}, {153, 155}, {151, 152}, {148, 150}, {146, 147},
      {144, 145}, {141, 143}, {139, 140}, {136, 138}, {134, 135}, {131, 133}, {129, 130}, {127, 128}, {124, 126},
      {122, 123}, {119, 121}, {117, 118}, {114, 116}, {112, 113}, {109, 111}, {107, 108}, {105, 106}, {102, 104},
      {100, 101}, {97, 99},   {95, 96},   {92, 94},   {90, 91},   {87, 89},   {85, 86},   {83, 84},   {80, 82},
      {78, 79},   {75, 77},   {73, 74},   {70, 72},   {68, 69},   {65, 67},   {63, 64},   {60, 62},   {58, 59},
      {55, 57},   {53, 54},   {51, 52},   {48, 50},   {46, 47},   {43, 45},   {41, 42},   {38, 40},   {36, 37},
      {33, 35},   {31, 32},   {28, 30},   {26, 27},   {23, 25},   {21, 22},   {18, 20},   {16, 17},   {13, 15},
      {11, 12},   {8, 10},    {6, 7},     {3, 5},     {1, 2},     {1, 1},     {1, 1}};

  csc_hs_to_gem_pad_even = {
      {1, 2},     {1, 2},     {3, 5},     {6, 7},     {8, 10},    {11, 12},   {13, 15},   {16, 17},   {18, 20},
      {21, 22},   {23, 25},   {26, 27},   {28, 30},   {31, 32},   {33, 35},   {36, 37},   {38, 39},   {40, 42},
      {43, 44},   {45, 47},   {48, 49},   {50, 52},   {53, 54},   {55, 57},   {58, 59},   {60, 62},   {63, 64},
      {65, 67},   {68, 69},   {70, 72},   {73, 74},   {75, 76},   {77, 79},   {80, 81},   {82, 84},   {85, 86},
      {87, 89},   {90, 91},   {92, 94},   {95, 96},   {97, 99},   {100, 101}, {102, 103}, {104, 106}, {107, 108},
      {109, 111}, {112, 113}, {114, 116}, {117, 118}, {119, 120}, {121, 123}, {124, 125}, {126, 128}, {129, 130},
      {131, 133}, {134, 135}, {136, 138}, {139, 140}, {141, 142}, {143, 145}, {146, 147}, {148, 150}, {151, 152},
      {153, 155}, {156, 157}, {158, 159}, {160, 162}, {163, 164}, {165, 167}, {168, 169}, {170, 172}, {173, 174},
      {175, 176}, {177, 179}, {180, 181}, {182, 184}, {185, 186}, {187, 189}, {190, 191}, {192, 193}, {194, 196},
      {197, 198}, {199, 201}, {202, 203}, {204, 206}, {207, 208}, {209, 210}, {211, 213}, {214, 215}, {216, 218},
      {219, 220}, {221, 222}, {223, 225}, {226, 227}, {228, 230}, {231, 232}, {233, 235}, {236, 237}, {238, 240},
      {241, 242}, {243, 244}, {245, 247}, {248, 249}, {250, 252}, {253, 254}, {255, 257}, {258, 259}, {260, 261},
      {262, 264}, {265, 266}, {267, 269}, {270, 271}, {272, 274}, {275, 276}, {277, 278}, {279, 281}, {282, 283},
      {284, 286}, {287, 288}, {289, 291}, {292, 293}, {294, 296}, {297, 298}, {299, 300}, {301, 303}, {304, 305},
      {306, 308}, {309, 310}, {311, 313}, {314, 315}, {316, 318}, {319, 320}, {321, 323}, {324, 325}, {326, 328},
      {329, 330}, {331, 332}, {333, 335}, {336, 337}, {338, 340}, {341, 342}, {343, 345}, {346, 347}, {348, 350},
      {351, 352}, {353, 355}, {356, 357}, {358, 360}, {361, 362}, {363, 365}, {366, 367}, {368, 370}, {371, 372},
      {373, 375}, {376, 377}, {378, 380}, {381, 382}, {383, 384}, {384, 384}, {384, 384}};
}

CSCGEMMotherboardLUTME21::~CSCGEMMotherboardLUTME21() {}
