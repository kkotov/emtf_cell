#include "emtf/ts/cell/LinksAlignmentReferences.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "emtf/ts/cell/Common.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

namespace emtf {

// initial reference values
map<string, uint32_t> InputLinksAlignmentReferences::referenceValues = {
    // endcap 1, sector 1
    {"1_1_me1a_02", 15}, {"1_1_me1a_03", 15}, {"1_1_me1a_04", 15}, {"1_1_me1a_05", 15}, {"1_1_me1a_06", 15}, {"1_1_me1a_07", 15}, {"1_1_me1a_08", 15}, {"1_1_me1a_09", 15},
    {"1_1_me1b_02", 15}, {"1_1_me1b_03", 15}, {"1_1_me1b_04", 15}, {"1_1_me1b_05", 15}, {"1_1_me1b_06", 14}, {"1_1_me1b_07", 15}, {"1_1_me1b_08", 15}, {"1_1_me1b_09", 16},
    {"1_1_me2_02",  15}, {"1_1_me2_03",  15}, {"1_1_me2_04",  15}, {"1_1_me2_05",  15}, {"1_1_me2_06",  15}, {"1_1_me2_07",  15}, {"1_1_me2_08",  15}, {"1_1_me2_09",  14},
    {"1_1_me3_02",  15}, {"1_1_me3_03",  15}, {"1_1_me3_04",  15}, {"1_1_me3_05",  15}, {"1_1_me3_06",  15}, {"1_1_me3_07",  15}, {"1_1_me3_08",  15}, {"1_1_me3_09",  14},
    {"1_1_me4_02",  15}, {"1_1_me4_03",  14}, {"1_1_me4_04",  14}, {"1_1_me4_05",  14}, {"1_1_me4_06",  14}, {"1_1_me4_07",  15}, {"1_1_me4_08",  14}, {"1_1_me4_09",  14},
    {"1_1_me1n_03", 15}, {"1_1_me1n_06", 14}, {"1_1_me1n_09", 14}, {"1_1_me2n_03", 14}, {"1_1_me2n_09", 13}, {"1_1_me3n_03", 15}, {"1_1_me3n_09", 14}, {"1_1_me4n_03", 15}, {"1_1_me4n_09", 14},

    // endcap 1, sector 2
    {"1_2_me1a_02", 15}, {"1_2_me1a_03", 15}, {"1_2_me1a_04", 15}, {"1_2_me1a_05", 15}, {"1_2_me1a_06", 15}, {"1_2_me1a_07", 15}, {"1_2_me1a_08", 15}, {"1_2_me1a_09", 15},
    {"1_2_me1b_02", 16}, {"1_2_me1b_03", 16}, {"1_2_me1b_04", 16}, {"1_2_me1b_05", 16}, {"1_2_me1b_06", 15}, {"1_2_me1b_07", 16}, {"1_2_me1b_08", 16}, {"1_2_me1b_09", 17},
    {"1_2_me2_02",  15}, {"1_2_me2_03",  15}, {"1_2_me2_04",  15}, {"1_2_me2_05",  15}, {"1_2_me2_06",  15}, {"1_2_me2_07",  15}, {"1_2_me2_08",  15}, {"1_2_me2_09",  14},
    {"1_2_me3_02",  15}, {"1_2_me3_03",  15}, {"1_2_me3_04",  15}, {"1_2_me3_05",  15}, {"1_2_me3_06",  15}, {"1_2_me3_07",  15}, {"1_2_me3_08",  15}, {"1_2_me3_09",  14},
    {"1_2_me4_02",   3}, {"1_2_me4_03",   3}, {"1_2_me4_04",   3}, {"1_2_me4_05",   3}, {"1_2_me4_06",   3}, {"1_2_me4_07",   3}, {"1_2_me4_08",   3}, {"1_2_me4_09",   2},
    {"1_2_me1n_03", 15}, {"1_2_me1n_06", 14}, {"1_2_me1n_09", 14}, {"1_2_me2n_03", 15}, {"1_2_me2n_09", 14}, {"1_2_me3n_03", 15}, {"1_2_me3n_09", 14}, {"1_2_me4n_03", 14}, {"1_2_me4n_09", 14},

    // endcap 1, sector 3
    {"1_3_me1a_02", 15}, {"1_3_me1a_03", 15}, {"1_3_me1a_04", 15}, {"1_3_me1a_05", 15}, {"1_3_me1a_06", 15}, {"1_3_me1a_07", 15}, {"1_3_me1a_08", 15}, {"1_3_me1a_09", 15},
    {"1_3_me1b_02", 15}, {"1_3_me1b_03", 15}, {"1_3_me1b_04", 15}, {"1_3_me1b_05", 15}, {"1_3_me1b_06", 14}, {"1_3_me1b_07", 15}, {"1_3_me1b_08", 15}, {"1_3_me1b_09", 16},
    {"1_3_me2_02",  15}, {"1_3_me2_03",  15}, {"1_3_me2_04",  14}, {"1_3_me2_05",  14}, {"1_3_me2_06",  15}, {"1_3_me2_07",  15}, {"1_3_me2_08",  14}, {"1_3_me2_09",  13},
    {"1_3_me3_02",  15}, {"1_3_me3_03",  15}, {"1_3_me3_04",  14}, {"1_3_me3_05",  15}, {"1_3_me3_06",  14}, {"1_3_me3_07",  15}, {"1_3_me3_08",  14}, {"1_3_me3_09",  14},
    {"1_3_me4_02",   9}, {"1_3_me4_03",   9}, {"1_3_me4_04",   9}, {"1_3_me4_05",   9}, {"1_3_me4_06",   9}, {"1_3_me4_07",   9}, {"1_3_me4_08",   9}, {"1_3_me4_09",   8},
    {"1_3_me1n_03", 16}, {"1_3_me1n_06", 15}, {"1_3_me1n_09", 15}, {"1_3_me2n_03", 15}, {"1_3_me2n_09", 14}, {"1_3_me3n_03", 15}, {"1_3_me3n_09", 14}, {"1_3_me4n_03",  3}, {"1_3_me4n_09",  2},

    // endcap 1, sector 4
    {"1_4_me1a_02", 15}, {"1_4_me1a_03", 15}, {"1_4_me1a_04", 14}, {"1_4_me1a_05", 14}, {"1_4_me1a_06", 15}, {"1_4_me1a_07", 15}, {"1_4_me1a_08", 15}, {"1_4_me1a_09", 15},
    {"1_4_me1b_02", 15}, {"1_4_me1b_03", 15}, {"1_4_me1b_04", 15}, {"1_4_me1b_05", 15}, {"1_4_me1b_06", 14}, {"1_4_me1b_07", 15}, {"1_4_me1b_08", 15}, {"1_4_me1b_09", 16},
    {"1_4_me2_02",  14}, {"1_4_me2_03",  14}, {"1_4_me2_04",  14}, {"1_4_me2_05",  14}, {"1_4_me2_06",  14}, {"1_4_me2_07",  14}, {"1_4_me2_08",  14}, {"1_4_me2_09",  13},
    {"1_4_me3_02",  15}, {"1_4_me3_03",  15}, {"1_4_me3_04",  15}, {"1_4_me3_05",  15}, {"1_4_me3_06",  15}, {"1_4_me3_07",  15}, {"1_4_me3_08",  15}, {"1_4_me3_09",  14},
    {"1_4_me4_02",  15}, {"1_4_me4_03",  15}, {"1_4_me4_04",  14}, {"1_4_me4_05",  14}, {"1_4_me4_06",  14}, {"1_4_me4_07",  15}, {"1_4_me4_08",  14}, {"1_4_me4_09",  14},
    {"1_4_me1n_03", 15}, {"1_4_me1n_06", 14}, {"1_4_me1n_09", 14}, {"1_4_me2n_03", 14}, {"1_4_me2n_09", 14}, {"1_4_me3n_03", 15}, {"1_4_me3n_09", 14}, {"1_4_me4n_03",  9}, {"1_4_me4n_09",  8},

    // endcap 1, sector 5
    {"1_5_me1a_02", 42}, {"1_5_me1a_03", 41}, {"1_5_me1a_04", 41}, {"1_5_me1a_05", 41}, {"1_5_me1a_06", 41}, {"1_5_me1a_07", 41}, {"1_5_me1a_08", 41}, {"1_5_me1a_09", 41},
    {"1_5_me1b_02", 41}, {"1_5_me1b_03", 41}, {"1_5_me1b_04", 41}, {"1_5_me1b_05", 41}, {"1_5_me1b_06", 40}, {"1_5_me1b_07", 41}, {"1_5_me1b_08", 41}, {"1_5_me1b_09", 42},
    {"1_5_me2_02",  15}, {"1_5_me2_03",  15}, {"1_5_me2_04",  15}, {"1_5_me2_05",  15}, {"1_5_me2_06",  15}, {"1_5_me2_07",  15}, {"1_5_me2_08",  15}, {"1_5_me2_09",  15},
    {"1_5_me3_02",  15}, {"1_5_me3_03",  15}, {"1_5_me3_04",  15}, {"1_5_me3_05",  14}, {"1_5_me3_06",  15}, {"1_5_me3_07",  15}, {"1_5_me3_08",  14}, {"1_5_me3_09",  14},
    {"1_5_me4_02",  14}, {"1_5_me4_03",  14}, {"1_5_me4_04",  14}, {"1_5_me4_05",  14}, {"1_5_me4_06",  14}, {"1_5_me4_07",  14}, {"1_5_me4_08",  14}, {"1_5_me4_09",  13},
    {"1_5_me1n_03", 15}, {"1_5_me1n_06", 14}, {"1_5_me1n_09", 14}, {"1_5_me2n_03", 14}, {"1_5_me2n_09", 13}, {"1_5_me3n_03", 15}, {"1_5_me3n_09", 14}, {"1_5_me4n_03", 15}, {"1_5_me4n_09", 14},

    // endcap 1, sector 6
    {"1_6_me1a_02", 15}, {"1_6_me1a_03", 15}, {"1_6_me1a_04", 15}, {"1_6_me1a_05", 15}, {"1_6_me1a_06", 15}, {"1_6_me1a_07", 15}, {"1_6_me1a_08", 15}, {"1_6_me1a_09", 15},
    {"1_6_me1b_02", 15}, {"1_6_me1b_03", 15}, {"1_6_me1b_04", 15}, {"1_6_me1b_05", 15}, {"1_6_me1b_06", 14}, {"1_6_me1b_07", 15}, {"1_6_me1b_08", 15}, {"1_6_me1b_09", 16},
    {"1_6_me2_02",  15}, {"1_6_me2_03",  14}, {"1_6_me2_04",  14}, {"1_6_me2_05",  14}, {"1_6_me2_06",  14}, {"1_6_me2_07",  14}, {"1_6_me2_08",  14}, {"1_6_me2_09",  13},
    {"1_6_me3_02",  15}, {"1_6_me3_03",  15}, {"1_6_me3_04",  15}, {"1_6_me3_05",  15}, {"1_6_me3_06",  15}, {"1_6_me3_07",  15}, {"1_6_me3_08",  15}, {"1_6_me3_09",  14},
    {"1_6_me4_02",  15}, {"1_6_me4_03",  15}, {"1_6_me4_04",  15}, {"1_6_me4_05",  15}, {"1_6_me4_06",  15}, {"1_6_me4_07",  15}, {"1_6_me4_08",  15}, {"1_6_me4_09",  14},
    {"1_6_me1n_03", 41}, {"1_6_me1n_06", 40}, {"1_6_me1n_09", 40}, {"1_6_me2n_03", 15}, {"1_6_me2n_09", 15}, {"1_6_me3n_03", 15}, {"1_6_me3n_09", 14}, {"1_6_me4n_03", 14}, {"1_6_me4n_09", 13},

    // endcap 2, sector 1
    {"2_1_me1a_02", 15}, {"2_1_me1a_03", 15}, {"2_1_me1a_04", 15}, {"2_1_me1a_05", 15}, {"2_1_me1a_06", 15}, {"2_1_me1a_07", 15}, {"2_1_me1a_08", 15}, {"2_1_me1a_09", 15},
    {"2_1_me1b_02", 15}, {"2_1_me1b_03", 15}, {"2_1_me1b_04", 15}, {"2_1_me1b_05", 15}, {"2_1_me1b_06", 14}, {"2_1_me1b_07", 15}, {"2_1_me1b_08", 14}, {"2_1_me1b_09", 16},
    {"2_1_me2_02",  15}, {"2_1_me2_03",  15}, {"2_1_me2_04",  14}, {"2_1_me2_05",  15}, {"2_1_me2_06",  15}, {"2_1_me2_07",  15}, {"2_1_me2_08",  15}, {"2_1_me2_09",  14},
    {"2_1_me3_02",  15}, {"2_1_me3_03",  15}, {"2_1_me3_04",  14}, {"2_1_me3_05",  15}, {"2_1_me3_06",  14}, {"2_1_me3_07",  15}, {"2_1_me3_08",  15}, {"2_1_me3_09",  14},
    {"2_1_me4_02",  15}, {"2_1_me4_03",  15}, {"2_1_me4_04",  15}, {"2_1_me4_05",  15}, {"2_1_me4_06",  15}, {"2_1_me4_07",  15}, {"2_1_me4_08",  15}, {"2_1_me4_09",  14},
    {"2_1_me1n_03", 14}, {"2_1_me1n_06", 13}, {"2_1_me1n_09", 13}, {"2_1_me2n_03", 14}, {"2_1_me2n_09", 14}, {"2_1_me3n_03", 14}, {"2_1_me3n_09", 13}, {"2_1_me4n_03", 14}, {"2_1_me4n_09", 13},

    // endcap 2, sector 2
    {"2_2_me1a_02", 15}, {"2_2_me1a_03", 15}, {"2_2_me1a_04", 15}, {"2_2_me1a_05", 15}, {"2_2_me1a_06", 15}, {"2_2_me1a_07", 15}, {"2_2_me1a_08", 15}, {"2_2_me1a_09", 15},
    {"2_2_me1b_02", 15}, {"2_2_me1b_03", 15}, {"2_2_me1b_04", 14}, {"2_2_me1b_05", 14}, {"2_2_me1b_06", 14}, {"2_2_me1b_07", 15}, {"2_2_me1b_08", 14}, {"2_2_me1b_09", 16},
    {"2_2_me2_02",   8}, {"2_2_me2_03",   8}, {"2_2_me2_04",   8}, {"2_2_me2_05",   8}, {"2_2_me2_06",   8}, {"2_2_me2_07",   8}, {"2_2_me2_08",   8}, {"2_2_me2_09",   7},
    {"2_2_me3_02",   9}, {"2_2_me3_03",   9}, {"2_2_me3_04",   9}, {"2_2_me3_05",   9}, {"2_2_me3_06",   9}, {"2_2_me3_07",   9}, {"2_2_me3_08",   9}, {"2_2_me3_09",   8},
    {"2_2_me4_02",   9}, {"2_2_me4_03",   9}, {"2_2_me4_04",   9}, {"2_2_me4_05",   9}, {"2_2_me4_06",   9}, {"2_2_me4_07",   9}, {"2_2_me4_08",   9}, {"2_2_me4_09",   8},
    {"2_2_me1n_03", 14}, {"2_2_me1n_06", 14}, {"2_2_me1n_09", 14}, {"2_2_me2n_03", 14}, {"2_2_me2n_09", 14}, {"2_2_me3n_03", 14}, {"2_2_me3n_09", 14}, {"2_2_me4n_03", 15}, {"2_2_me4n_09", 14},

    // endcap 2, sector 3
    {"2_3_me1a_02", 15}, {"2_3_me1a_03", 15}, {"2_3_me1a_04", 15}, {"2_3_me1a_05", 15}, {"2_3_me1a_06", 15}, {"2_3_me1a_07", 15}, {"2_3_me1a_08", 15}, {"2_3_me1a_09", 15},
    {"2_3_me1b_02", 15}, {"2_3_me1b_03", 14}, {"2_3_me1b_04", 14}, {"2_3_me1b_05", 14}, {"2_3_me1b_06", 14}, {"2_3_me1b_07", 14}, {"2_3_me1b_08", 14}, {"2_3_me1b_09", 15},
    {"2_3_me2_02",   9}, {"2_3_me2_03",   9}, {"2_3_me2_04",   9}, {"2_3_me2_05",   9}, {"2_3_me2_06",   9}, {"2_3_me2_07",   9}, {"2_3_me2_08",   9}, {"2_3_me2_09",   8},
    {"2_3_me3_02",   8}, {"2_3_me3_03",   8}, {"2_3_me3_04",   8}, {"2_3_me3_05",   8}, {"2_3_me3_06",   9}, {"2_3_me3_07",   8}, {"2_3_me3_08",   8}, {"2_3_me3_09",   8},
    {"2_3_me4_02",   3}, {"2_3_me4_03",   2}, {"2_3_me4_04",   2}, {"2_3_me4_05",   2}, {"2_3_me4_06",   2}, {"2_3_me4_07",   2}, {"2_3_me4_08",   2}, {"2_3_me4_09",   1},
    {"2_3_me1n_03", 15}, {"2_3_me1n_06", 14}, {"2_3_me1n_09", 14}, {"2_3_me2n_03",  8}, {"2_3_me2n_09",  8}, {"2_3_me3n_03",  9}, {"2_3_me3n_09",  8}, {"2_3_me4n_03",  9}, {"2_3_me4n_09",  8},

    // endcap 2, sector 4
    {"2_4_me1a_02", 14}, {"2_4_me1a_03", 14}, {"2_4_me1a_04", 14}, {"2_4_me1a_05", 14}, {"2_4_me1a_06", 14}, {"2_4_me1a_07", 14}, {"2_4_me1a_08", 14}, {"2_4_me1a_09", 14},
    {"2_4_me1b_02", 14}, {"2_4_me1b_03", 14}, {"2_4_me1b_04", 14}, {"2_4_me1b_05", 14}, {"2_4_me1b_06", 13}, {"2_4_me1b_07", 14}, {"2_4_me1b_08", 14}, {"2_4_me1b_09", 15},
    {"2_4_me2_02",  15}, {"2_4_me2_03",  14}, {"2_4_me2_04",  14}, {"2_4_me2_05",  14}, {"2_4_me2_06",  14}, {"2_4_me2_07",  14}, {"2_4_me2_08",  14}, {"2_4_me2_09",  14},
    {"2_4_me3_02",  15}, {"2_4_me3_03",  14}, {"2_4_me3_04",  14}, {"2_4_me3_05",  14}, {"2_4_me3_06",  15}, {"2_4_me3_07",  14}, {"2_4_me3_08",  14}, {"2_4_me3_09",  14},
    {"2_4_me4_02",   3}, {"2_4_me4_03",   3}, {"2_4_me4_04",   3}, {"2_4_me4_05",   3}, {"2_4_me4_06",   3}, {"2_4_me4_07",   3}, {"2_4_me4_08",   3}, {"2_4_me4_09",   2},
    {"2_4_me1n_03", 14}, {"2_4_me1n_06", 13}, {"2_4_me1n_09", 13}, {"2_4_me2n_03",  9}, {"2_4_me2n_09",  8}, {"2_4_me3n_03",  8}, {"2_4_me3n_09",  7}, {"2_4_me4n_03",  2}, {"2_4_me4n_09",  2},

    // endcap 2, sector 5
    {"2_5_me1a_02", 41}, {"2_5_me1a_03", 41}, {"2_5_me1a_04", 41}, {"2_5_me1a_05", 41}, {"2_5_me1a_06", 41}, {"2_5_me1a_07", 41}, {"2_5_me1a_08", 41}, {"2_5_me1a_09", 41},
    {"2_5_me1b_02", 41}, {"2_5_me1b_03", 41}, {"2_5_me1b_04", 41}, {"2_5_me1b_05", 41}, {"2_5_me1b_06", 40}, {"2_5_me1b_07", 42}, {"2_5_me1b_08", 41}, {"2_5_me1b_09", 43},
    {"2_5_me2_02",  15}, {"2_5_me2_03",  15}, {"2_5_me2_04",  15}, {"2_5_me2_05",  15}, {"2_5_me2_06",  15}, {"2_5_me2_07",  15}, {"2_5_me2_08",  15}, {"2_5_me2_09",  14},
    {"2_5_me3_02",  15}, {"2_5_me3_03",  15}, {"2_5_me3_04",  14}, {"2_5_me3_05",  14}, {"2_5_me3_06",  15}, {"2_5_me3_07",  15}, {"2_5_me3_08",  15}, {"2_5_me3_09",  14},
    {"2_5_me4_02",  15}, {"2_5_me4_03",  15}, {"2_5_me4_04",  15}, {"2_5_me4_05",  15}, {"2_5_me4_06",  15}, {"2_5_me4_07",  15}, {"2_5_me4_08",  15}, {"2_5_me4_09",  14},
    {"2_5_me1n_03", 14}, {"2_5_me1n_06", 13}, {"2_5_me1n_09", 13}, {"2_5_me2n_03", 14}, {"2_5_me2n_09", 14}, {"2_5_me3n_03", 15}, {"2_5_me3n_09", 14}, {"2_5_me4n_03",  3}, {"2_5_me4n_09",  2},

    // endcap 2, sector 6
    {"2_6_me1a_02", 15}, {"2_6_me1a_03", 15}, {"2_6_me1a_04", 14}, {"2_6_me1a_05", 15}, {"2_6_me1a_06", 15}, {"2_6_me1a_07", 15}, {"2_6_me1a_08", 15}, {"2_6_me1a_09", 15},
    {"2_6_me1b_02", 15}, {"2_6_me1b_03", 14}, {"2_6_me1b_04", 14}, {"2_6_me1b_05", 14}, {"2_6_me1b_06", 14}, {"2_6_me1b_07", 15}, {"2_6_me1b_08", 14}, {"2_6_me1b_09", 16},
    {"2_6_me2_02",  15}, {"2_6_me2_03",  15}, {"2_6_me2_04",  15}, {"2_6_me2_05",  14}, {"2_6_me2_06",  15}, {"2_6_me2_07",  15}, {"2_6_me2_08",  15}, {"2_6_me2_09",  14},
    {"2_6_me3_02",  15}, {"2_6_me3_03",  14}, {"2_6_me3_04",  14}, {"2_6_me3_05",  14}, {"2_6_me3_06",  14}, {"2_6_me3_07",  14}, {"2_6_me3_08",  14}, {"2_6_me3_09",  13},
    {"2_6_me4_02",  14}, {"2_6_me4_03",  14}, {"2_6_me4_04",  14}, {"2_6_me4_05",  14}, {"2_6_me4_06",  14}, {"2_6_me4_07",  14}, {"2_6_me4_08",  14}, {"2_6_me4_09",  13},
    {"2_6_me1n_03", 41}, {"2_6_me1n_06", 40}, {"2_6_me1n_09", 40}, {"2_6_me2n_03", 15}, {"2_6_me2n_09", 14}, {"2_6_me3n_03", 15}, {"2_6_me3n_09", 14}, {"2_6_me4n_03", 15}, {"2_6_me4n_09", 14}
};


UpdateLinkAlignmentRefs::UpdateLinkAlignmentRefs(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("update_link_alignment", xdata::UnsignedInteger(0u));
}

swatch::action::Command::State UpdateLinkAlignmentRefs::code(const swatch::core::XParameterSet& params)
{
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    std::remove( std::string(config::alignmentReferencesDir() + "/" + processor.getStub().id).c_str() );

    const uint64_t needUpdate( params.get<xdata::UnsignedInteger>("update_link_alignment").value_ );

    Command::State commandStatus = Functionoid::kDone;

    // no need updating?
    if( !needUpdate ) return commandStatus;

    setStatusMsg("Updating link alignment references...");

    uint32_t endcap = processor.endcap();
    uint32_t sector = processor.sector();

    std::stringstream sMapId;
    sMapId << endcap << "_" << sector << "_";
    std::string mapId = sMapId.str();

    std::ifstream alignmentReferenceSummaryFile
    (
        config::alignmentReferencesDir() + "/" + processor.getStub().id + "_summary"
    );
    if( !alignmentReferenceSummaryFile )
    {
        setStatusMsg("No '" + processor.getStub().id + "_means' file found in " + config::alignmentReferencesDir());
        return Functionoid::kError;
    }

    for(auto it =processor.getInputPorts().getPorts().begin();
             it!=processor.getInputPorts().getPorts().end();
             ++it)
    {
        EmtfCscInputPort *port = dynamic_cast<EmtfCscInputPort *>(*it);

        if(port) // process only the CSC input ports
        {
            alignmentReferenceSummaryFile >> InputLinksAlignmentReferences::referenceValues[ mapId + port->getId() ];
        }
    }

    alignmentReferenceSummaryFile.close();

    setProgress(1.);

    return commandStatus;
}

SaveLinkAlignmentRefs::SaveLinkAlignmentRefs(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("save_link_alignment", xdata::UnsignedInteger(1u));
}

swatch::action::Command::State SaveLinkAlignmentRefs::code(const swatch::core::XParameterSet& params)
{
    const uint64_t needSave( params.get<xdata::UnsignedInteger>("save_link_alignment").value_ );

    Command::State commandStatus = Functionoid::kDone;

    // do nothing?
    if( !needSave ) return commandStatus;

    setStatusMsg("Processing and saving link alignment references...");

    EmtfProcessor &processor = getActionable<EmtfProcessor>();
    std::ifstream alignmentReferenceLogFile
    (
        config::alignmentReferencesDir() + "/" + processor.getStub().id
    );
    if( !alignmentReferenceLogFile )
    {
        setStatusMsg("No " + processor.getStub().id + " file found in " + config::alignmentReferencesDir() + ". Do nothing.");
        return Functionoid::kDone; //kError;
    }

    // internally, swatch keeps ports in std::deque that provides the random-access
    //  iterators, thus std::distance function offers an amortized constant time complexity
    size_t nPorts = std::distance(processor.getInputPorts().getPorts().begin(),
                                  processor.getInputPorts().getPorts().end()
    );
    size_t nCscPorts = 0;

    // read in the values a table that is a vector of columns
    std::vector<std::list<uint32_t>> table(nPorts);
    for(std::string line; std::getline(alignmentReferenceLogFile, line); )
    {
        nCscPorts = 0;

        std::istringstream tmp(line);

        for(auto it =processor.getInputPorts().getPorts().begin();
                 it!=processor.getInputPorts().getPorts().end();
                 ++it)
        {

            EmtfCscInputPort *port = dynamic_cast<EmtfCscInputPort *>(*it);

            if(port) // process only the CSC input ports
            {
                size_t value = 0;
                tmp >> value;
                table[nCscPorts++].push_back(value);
            }
        }
    }

    alignmentReferenceLogFile.close();

    // averaging and calculating standard deviation
    std::vector<double> mean(nCscPorts), sd(nCscPorts);
    for(size_t col=0; col<nCscPorts; ++col)
    {
        size_t sum = 0, sum2 = 0, cnt = 0;
        for(auto i : table[col])
        {
            cnt++;
            sum  += i;
            sum2 += i*i;
        }
        if(cnt>0) mean[col] = sum/double(cnt);
        if(cnt>1) sd  [col] = sqrt( (sum2-sum*sum/double(cnt))/(cnt-1) );
    }

    // current time
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    // convert current time to the C/POSIX format
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    // make a UTC tm structure from that format
    struct tm utc;
    gmtime_r(&t,&utc);
    char buf[128];
    strftime(buf, sizeof(buf), "%Y.%m.%d_%T", &utc);
    std::string ts = buf;

    // save the summary into a file
    std::ofstream alignmentReferenceSummaryFile
    (
        config::alignmentReferencesDir() + "/" + processor.getStub().id + "_" + ts + "_summary"
    );
    if( !alignmentReferenceSummaryFile )
    {
        setStatusMsg("Cannot open " + processor.getStub().id + "_" + ts + " file " + config::alignmentReferencesDir());
        return Functionoid::kError;
    }
    for(size_t col=0; col<nCscPorts; ++col)
        alignmentReferenceSummaryFile << std::setw(4) << mean[col] << "\t";

    alignmentReferenceSummaryFile << std::endl;

    for(size_t col=0; col<nCscPorts; ++col)
        alignmentReferenceSummaryFile << std::setw(4) << sd[col] << "\t";

    alignmentReferenceSummaryFile.close();

    // cleanup
    if( std::rename(
            std::string(config::alignmentReferencesDir() + "/" + processor.getStub().id).c_str(),
            std::string(config::alignmentReferencesDir() + "/" + processor.getStub().id + "_" + ts).c_str()
        )
    ){
        setStatusMsg("Cannot rename " + processor.getStub().id + " file in " + config::alignmentReferencesDir());
        return Functionoid::kError;
    }

    setProgress(1.);

    return commandStatus;
}

} // namespace

