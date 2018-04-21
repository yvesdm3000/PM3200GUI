/***************************************************************
 * Name:      PM3200Data.cpp
 * Purpose:   Code for PM3200 Data fields
 * Author:    Yves De Muyter (yves@alfavisio.be)
 * Created:   2018-04-02
 * Copyright: Yves De Muyter (yves@alfavisio.be)
 * License:   MIT
 **************************************************************/
/*
Copyright 2018 Yves De Muyter (yves@alfavisio.be)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#include "PM3200Data.h"
#include <string.h>

struct PM3200RegisterElement
{
    int Reg;
    PM3200Data::RegisterType Type;
//    unsigned int Size; // Size in uint16 elements
    const char* Units;
    const char* ShortName;
    const char* LongName;
};

static PM3200RegisterElement elements[] = {
    { 2014,  PM3200Data::RegisterType::UInt16,  "",     "Phases", "Number of phases"},
    { 2015,  PM3200Data::RegisterType::UInt16,  "",     "Wires", "Number of wires"},
    { 2016,  PM3200Data::RegisterType::UInt16,  "",     "Power", "Power system configuration"},
    { 2017,  PM3200Data::RegisterType::UInt16,  "Hz",   "NFreq", "Nominal frequency"},
    { 2024,  PM3200Data::RegisterType::UInt16,  "",     "NPhase", "Nominal phase order"},
    { 2025,  PM3200Data::RegisterType::UInt16,  "",     "VTs", "Number VTs"},
    { 2026,  PM3200Data::RegisterType::Float32, "V",    "VT1", "VT Primary"},
    { 2028,  PM3200Data::RegisterType::UInt16,  "V",    "VT2", "VT Secondary"},
    { 2029,  PM3200Data::RegisterType::UInt16,  "",     "CTs", "Number CTs"},
    { 2030,  PM3200Data::RegisterType::UInt16,  "A",    "CT1", "CT Primary"},
    { 2031,  PM3200Data::RegisterType::UInt16,  "A",    "CT2", "CT Secondary"},
    { 2031,  PM3200Data::RegisterType::UInt16,  "",     "VT_type", "VT Connection type"},
    { 3000,  PM3200Data::RegisterType::Float32, "A",    "I_1", "I1: phase 1 current"},
    { 3002,  PM3200Data::RegisterType::Float32, "A",    "I_2", "I2: phase 2 current"},
    { 3004,  PM3200Data::RegisterType::Float32, "A",    "I_3", "I3: phase 3 current"},
    { 3006,  PM3200Data::RegisterType::Float32, "A",    "I_n", "In: Neutral current"},
    { 3010,  PM3200Data::RegisterType::Float32, "A",    "I_avg", "Current Avg"},
    { 3020,  PM3200Data::RegisterType::Float32, "V",    "V_L1_L2", "Voltage L1-L2"},
    { 3022,  PM3200Data::RegisterType::Float32, "V",    "V_L2_L3", "Voltage L2-L3"},
    { 3024,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L3-L1"},
    { 3026,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L-L Avg"},
    { 3028,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L1-N"},
    { 3030,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L2-N"},
    { 3032,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L3-N"},
    { 3034,  PM3200Data::RegisterType::Float32, "V",    "", "Voltage L-N Avg"},
    { 3054,  PM3200Data::RegisterType::Float32, "kW",   "", "Active Power Phase 1"},
    { 3056,  PM3200Data::RegisterType::Float32, "kW",   "", "Active Power Phase 2"},
    { 3058,  PM3200Data::RegisterType::Float32, "kW",   "", "Active Power Phase 3"},
    { 3060,  PM3200Data::RegisterType::Float32, "kW",   "", "Total Active Power"},
    { 3062,  PM3200Data::RegisterType::Float32, "kVAR", "", "Reactive Power Phase 1"},
    { 3064,  PM3200Data::RegisterType::Float32, "kVAR", "", "Reactive Power Phase 2"},
    { 3066,  PM3200Data::RegisterType::Float32, "kVAR", "", "Reactive Power Phase 3"},
    { 3068,  PM3200Data::RegisterType::Float32, "kVAR", "", "Total Reactive Power"},
    { 3070,  PM3200Data::RegisterType::Float32, "kVA",  "", "Apparent Power Phase 1"},
    { 3072,  PM3200Data::RegisterType::Float32, "kVA",  "", "Apparent Power Phase 2"},
    { 3074,  PM3200Data::RegisterType::Float32, "kVA",  "", "Apparent Power Phase 3"},
    { 3076,  PM3200Data::RegisterType::Float32, "kVA",  "", "Total Apparent Power"},
    { 3078,  PM3200Data::RegisterType::Float32, "",     "", "Power Factor Phase 1 (Complex format)"},
    { 3080,  PM3200Data::RegisterType::Float32, "",     "", "Power Factor Phase 2 (Complex format)"},
    { 3082,  PM3200Data::RegisterType::Float32, "",     "", "Power Factor Phase 3 (Complex format)"},
    { 3084,  PM3200Data::RegisterType::Float32, "",     "", "Power Factor Total"},
    { 3012,  PM3200Data::RegisterType::Float32, "%",    "", "Current Unbalance I1"},
    { 3014,  PM3200Data::RegisterType::Float32, "%",    "", "Current Unbalance I2"},
    { 3016,  PM3200Data::RegisterType::Float32, "%",    "", "Current Unbalance I3"},
    { 3018,  PM3200Data::RegisterType::Float32, "%",    "", "Current Unbalance Worst"},
    { 3038,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L1-L2"},
    { 3040,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L2-L3"},
    { 3042,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L3-L1"},
    { 3044,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L-L Worst"},
    { 3046,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L1-N"},
    { 3048,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L2-N"},
    { 3050,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L3-N"},
    { 3052,  PM3200Data::RegisterType::Float32, "%",    "", "Voltage Unbalance L-N Worst"},
    { 3108,  PM3200Data::RegisterType::Float32, "",     "", "Tangent Phi, Total"},
    { 3110,  PM3200Data::RegisterType::Float32, "Hz",   "Freq", "Frequency"},
    { 3132,  PM3200Data::RegisterType::Float32, "deg C","Temp", "Temperature"},
    { 3252,  PM3200Data::RegisterType::DateTime,"",     "", "Energy Reset Date/Time"},
    { 3204,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Total Active Energy Import"},
    { 3208,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Total Active Energy Export"},
    { 3220,  PM3200Data::RegisterType::Int64,   "VARh", "", "Total Reactive Energy Import"},
    { 3224,  PM3200Data::RegisterType::Int64,   "VARh", "", "Total Reactive Energy Export"},
    { 3236,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Total Apparent Energy Import"},
    { 3240,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Total Apparent Energy Export"},
    { 3256,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Partial Active Energy Import"},
    { 3272,  PM3200Data::RegisterType::Int64,   "VARh", "", "Partial Reactive Energy Import"},
    { 3588,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Partial Apparent Energy Import"},
    { 3518,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Active Energy Import Phase 1"},
    { 3522,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Active Energy Import Phase 2"},
    { 3526,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Active Energy Import Phase 3"},
    { 3530,  PM3200Data::RegisterType::Int64,   "VARh", "", "Reactive Energy Import Phase 1"},
    { 3534,  PM3200Data::RegisterType::Int64,   "VARh", "", "Reactive Energy Import Phase 2"},
    { 3538,  PM3200Data::RegisterType::Int64,   "VARh", "", "Reactive Energy Import Phase 3"},
    { 3542,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Apparent Energy Import Phase 1"},
    { 3546,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Apparent Energy Import Phase 2"},
    { 3550,  PM3200Data::RegisterType::Int64,   "VAh",  "", "Apparent Energy Import Phase 3"},
    { 3554,  PM3200Data::RegisterType::DateTime,"",     "", "Input Metering Accumulation Reset Date/Time"},
    { 3558,  PM3200Data::RegisterType::Int64,   "Unit", "", "Input Metering Accumulation Channel 01"},
    { 3562,  PM3200Data::RegisterType::Int64,   "Unit", "", "Input Metering Accumulation Channel 02"},

    { 3701,  PM3200Data::RegisterType::UInt16,  "",     "", "Demand method"},
    { 3702,  PM3200Data::RegisterType::UInt16,  "Min",  "", "Demand interval duration"},
    { 3706,  PM3200Data::RegisterType::DateTime,"",     "", "Demand Peak Reset Date/Time"},
    { 3766,  PM3200Data::RegisterType::Float32, "kW",   "", "Active Power Present Demand"},
    { 3770,  PM3200Data::RegisterType::Float32, "kW",   "", "Active Power Peak Demand"},
    { 3772,  PM3200Data::RegisterType::DateTime,"",     "", "Active Power Peak Demand Date/Time"},
    { 3782,  PM3200Data::RegisterType::Float32, "kVAR", "", "Reactive Power Present Demand"},
    { 3786,  PM3200Data::RegisterType::Float32, "kVAR", "", "Reactive Power Peak Demand"},
    { 3788,  PM3200Data::RegisterType::DateTime,"",     "", "Reactive Power Peak Demand Date/Time"},
    { 3798,  PM3200Data::RegisterType::Float32, "kVA",  "", "Apparent Power Present Demand"},

    { 3802,  PM3200Data::RegisterType::Float32, "kVA",  "", "Apparent Power Peak Demand"},
    { 3804,  PM3200Data::RegisterType::DateTime,"",     "", "Apparent Power Peak Demand Date/Time"},
    { 3814,  PM3200Data::RegisterType::Float32, "A",    "", "Current I1 Present Demand"},
    { 3818,  PM3200Data::RegisterType::Float32, "A",    "", "Current I1 Peak Demand"},
    { 3820,  PM3200Data::RegisterType::DateTime,"",     "", "Current I1 Peak Demand Date/Time"},
    { 3830,  PM3200Data::RegisterType::Float32, "A",    "", "Current I2 Present Demand"},
    { 3834,  PM3200Data::RegisterType::Float32, "A",    "", "Current I2 Peak Demand"},
    { 3836,  PM3200Data::RegisterType::DateTime,"",     "", "Current I2 Peak Demand Date/Time"},
    { 3846,  PM3200Data::RegisterType::Float32, "A",    "", "Current I3 Present Demand"},
    { 3850,  PM3200Data::RegisterType::Float32, "A",    "", "Current I3 Peak Demand"},
    { 3852,  PM3200Data::RegisterType::DateTime,"",     "", "Current I3 Peak Demand Date/Time"},
    { 3862,  PM3200Data::RegisterType::Float32, "A",    "", "Current In Present Demand"},
    { 3866,  PM3200Data::RegisterType::Float32, "A",    "", "Current In Peak Demand"},
    { 3868,  PM3200Data::RegisterType::DateTime,"",     "", "Current In Peak Demand Date/Time"},
    { 3878,  PM3200Data::RegisterType::Float32, "A",    "", "Current Avg Present Demand"},
    { 3882,  PM3200Data::RegisterType::Float32, "A",    "", "Current Avg Peak Demand"},
    { 3884,  PM3200Data::RegisterType::DateTime,"",     "", "Current Avg Peak Demand Date/Time"},

    { 4196,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Rate 1 Active Energy Import"},
    { 4200,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Rate 2 Active Energy Import"},
    { 4204,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Rate 3 Active Energy Import"},
    { 4208,  PM3200Data::RegisterType::Int64,   "Wh",   "", "Rate 4 Active Energy Import"},

// Minimum
    { 27214, PM3200Data::RegisterType::DateTime,"",     "", "Minimum/Maximum Reset Date/Time"},
    { 27218, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current I1"},
    { 27220, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current I2"},
    { 27222, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current I3"},
    { 27224, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current N"},
    { 27228, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current Avg"},

    { 27238, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L1-L2"},
    { 27240, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L2-L3"},
    { 27242, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L3-L1"},
    { 27244, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L-L Avg"},
    { 27246, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L1-N"},
    { 27248, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L2-N"},
    { 27250, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L3-N"},
    { 27254, PM3200Data::RegisterType::Float32, "V",    "", "Minimum Voltage L-N Avg"},

    { 27272, PM3200Data::RegisterType::Float32, "kW",   "", "Minimum Active Power Phase 1"},
    { 27274, PM3200Data::RegisterType::Float32, "kW",   "", "Minimum Active Power Phase 2"},
    { 27276, PM3200Data::RegisterType::Float32, "kW",   "", "Minimum Active Power Phase 3"},
    { 27278, PM3200Data::RegisterType::Float32, "kW",   "", "Minimum Active Power Phase Total"},
    { 27280, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Reactive Power Phase 1"},
    { 27282, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Reactive Power Phase 2"},
    { 27284, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Reactive Power Phase 3"},
    { 27286, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Reactive Power Phase Total"},
    { 27288, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Apparent Power Phase 1"},
    { 27290, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Apparent Power Phase 2"},
    { 27292, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Apparent Power Phase 3"},
    { 27294, PM3200Data::RegisterType::Float32, "kVAR", "", "Minimum Apparent Power Phase Total"},

    { 27336, PM3200Data::RegisterType::Float32, "",     "", "Minimum Tangent Phi, Total"},

    { 27338, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Current I1"},
    { 27340, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Current I2"},
    { 27342, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Current I3"},
    { 27344, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Current N"},

    { 27360, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L1-L2"},
    { 27362, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L2-L3"},
    { 27364, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L3-L1"},
    { 27366, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L-L Avg"},
    { 27368, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L1-N"},
    { 27370, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L2-N"},
    { 27372, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L3-N"},
    { 27376, PM3200Data::RegisterType::Float32, "%",    "", "Minimum Total Harmonic Distortion Voltage L-N Avg"},
    { 27616, PM3200Data::RegisterType::Float32, "Hz",   "", "Minimum Frequency"},


// Maximum

    { 27694, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current I1"},
    { 27696, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current I2"},
    { 27698, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current I3"},
    { 27700, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current N"},
    { 27704, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current Avg"},

    { 27714, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L1-L2"},
    { 27716, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L2-L3"},
    { 27718, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L3-L1"},
    { 27720, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L-L Avg"},
    { 27722, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L1-N"},
    { 27724, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L2-N"},
    { 27726, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L3-N"},
    { 27730, PM3200Data::RegisterType::Float32, "V",    "", "Maximum Voltage L-N Avg"},

    { 27748, PM3200Data::RegisterType::Float32, "kW",   "", "Maximum Active Power Phase 1"},
    { 27750, PM3200Data::RegisterType::Float32, "kW",   "", "Maximum Active Power Phase 2"},
    { 27752, PM3200Data::RegisterType::Float32, "kW",   "", "Maximum Active Power Phase 3"},
    { 27754, PM3200Data::RegisterType::Float32, "kW",   "", "Maximum Active Power Phase Total"},
    { 27756, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Reactive Power Phase 1"},
    { 27758, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Reactive Power Phase 2"},
    { 27760, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Reactive Power Phase 3"},
    { 27762, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Reactive Power Phase Total"},
    { 27764, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Apparent Power Phase 1"},
    { 27766, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Apparent Power Phase 2"},
    { 27768, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Apparent Power Phase 3"},
    { 27770, PM3200Data::RegisterType::Float32, "kVAR", "", "Maximum Apparent Power Phase Total"},

    { 27812, PM3200Data::RegisterType::Float32, "",     "", "Maximum Tangent Phi, Total"},

    { 27814, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Current I1"},
    { 27816, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Current I2"},
    { 27818, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Current I3"},
    { 27820, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Current N"},

    { 27836, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L1-L2"},
    { 27838, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L2-L3"},
    { 27840, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L3-L1"},
    { 27842, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L-L Avg"},
    { 27844, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L1-N"},
    { 27846, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L2-N"},
    { 27848, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L3-N"},
    { 27852, PM3200Data::RegisterType::Float32, "%",    "", "Maximum Total Harmonic Distortion Voltage L-N Avg"},
    { 28092, PM3200Data::RegisterType::Float32, "Hz",   "", "Maximum Frequency"},


    { 45130, PM3200Data::RegisterType::DateTime,"",     "", "Minimum Current of I1,I2,I3 - Date/Time"},
    { 45134, PM3200Data::RegisterType::Float32, "A",    "", "Minimum Current of I1,I2,I3 - Value"},
    { 45136, PM3200Data::RegisterType::DateTime,"",     "", "Minimum Power Factor Total - Date/Time"},
    { 45140, PM3200Data::RegisterType::Float32, "",     "", "Minimum Power Factor Total - Value"},
    { 45142, PM3200Data::RegisterType::DateTime,"",     "", "Maximum Current of I1,I2,I3 - Date/Time"},
    { 45146, PM3200Data::RegisterType::Float32, "A",    "", "Maximum Current of I1,I2,I3 - Value"},

    { 45148, PM3200Data::RegisterType::DateTime,"",     "", "Maximum Active Power Total - Date/Time"},
    { 45152, PM3200Data::RegisterType::Float32, "kW",   "", "Maximum Active Power Total - Value"},
    { 45154, PM3200Data::RegisterType::DateTime,"",     "", "Maximum Apparent Power Total - Date/Time"},
    { 45158, PM3200Data::RegisterType::Float32, "kVA",  "", "Maximum Apparent Power Total - Value"},
    { 45160, PM3200Data::RegisterType::DateTime,"",     "", "Maximum Power Factor Total - Date/Time"},
    { 45164, PM3200Data::RegisterType::Float32, "",     "", "Maximum Power Factor Total - Value"},

    { 45166, PM3200Data::RegisterType::Float32, "Wh",   "", "Total Active Energy Import"},
    { 45168, PM3200Data::RegisterType::Float32, "Wh",   "", "Total Active Energy Export"},
    { 45170, PM3200Data::RegisterType::Float32, "VARh", "", "Total Reactive Energy Import"},
    { 45172, PM3200Data::RegisterType::Float32, "VARh", "", "Total Reactive Energy Export"},
    { 45174, PM3200Data::RegisterType::Float32, "VAh",  "", "Total Apparent Energy Import"},
    { 45176, PM3200Data::RegisterType::Float32, "VAh",  "", "Total Apparent Energy Export"},
    { 45178, PM3200Data::RegisterType::Float32, "Wh",   "", "Partial Active Energy Import"},
    { 45180, PM3200Data::RegisterType::Float32, "VARh", "", "Partial Reactive Energy Import"},
    { 45182, PM3200Data::RegisterType::Float32, "VAh",  "", "Partial Apparent Energy Import"},
    { 45184, PM3200Data::RegisterType::Float32, "Wh",   "", "Active Energy Import Phase 1"},
    { 45186, PM3200Data::RegisterType::Float32, "Wh",   "", "Active Energy Import Phase 2"},
    { 45188, PM3200Data::RegisterType::Float32, "Wh",   "", "Active Energy Import Phase 3"},
    { 45190, PM3200Data::RegisterType::Float32, "VARh", "", "Reactive Energy Import Phase 1"},
    { 45192, PM3200Data::RegisterType::Float32, "VARh", "", "Reactive Energy Import Phase 2"},
    { 45194, PM3200Data::RegisterType::Float32, "VARh", "", "Reactive Energy Import Phase 3"},
    { 45196, PM3200Data::RegisterType::Float32, "VAh",  "", "Apparent Energy Import Phase 1"},
    { 45198, PM3200Data::RegisterType::Float32, "VAh",  "", "Apparent Energy Import Phase 2"},
    { 45200, PM3200Data::RegisterType::Float32, "VAh",  "", "Apparent Energy Import Phase 3"},
    { 45206, PM3200Data::RegisterType::Float32, "Wh",   "", "Rate 1 Active Energy Import"},
    { 45208, PM3200Data::RegisterType::Float32, "Wh",   "", "Rate 2 Active Energy Import"},
    { 45210, PM3200Data::RegisterType::Float32, "Wh",   "", "Rate 3 Active Energy Import"},
    { 45212, PM3200Data::RegisterType::Float32, "Wh",   "", "Rate 4 Active Energy Import"},
    { 45202, PM3200Data::RegisterType::Float32, "Unit", "", "Input Metering Accumulation Channel 01"},
    { 45204, PM3200Data::RegisterType::Float32, "Unit", "", "Input Metering Accumulation Channel 02"},
};

int PM3200Data::LookupRegister(const char* name) const
{
    for (unsigned i = 0; i<sizeof(elements)/sizeof(PM3200RegisterElement); ++i)
    {
        if (strcmp(elements[i].LongName, name) == 0)
        {
            return elements[i].Reg;
        }
        if (strcmp(elements[i].ShortName, name) == 0)
        {
            return elements[i].Reg;
        }
    }
    return -1;
}
const char* PM3200Data::LookupUnit(int reg) const
{
    for (unsigned i = 0; i<sizeof(elements)/sizeof(PM3200RegisterElement); ++i)
    {
        if (elements[i].Reg == reg)
            return elements[i].Units;
    }
    return "";
}
const char* PM3200Data::LookupName(int reg) const
{
    for (unsigned i = 0; i<sizeof(elements)/sizeof(PM3200RegisterElement); ++i)
    {
        if (elements[i].Reg == reg)
            return elements[i].LongName;
    }
    return "";
}

wxArrayString PM3200Data::GetRegisterNames() const
{
    wxArrayString list;
    for (unsigned i = 0; i<sizeof(elements)/sizeof(PM3200RegisterElement); ++i)
    {
        list.Add(wxString(elements[i].LongName));
    }
    return list;
}

PM3200Data::RegisterType PM3200Data::LookupType(int reg) const
{
    for (unsigned i = 0; i<sizeof(elements)/sizeof(PM3200RegisterElement); ++i)
    {
        if (elements[i].Reg == reg)
            return elements[i].Type;
    }
    return PM3200Data::RegisterType::UInt16;
}

int PM3200Data::LookupRegisterCount(int reg) const
{
    PM3200Data::RegisterType t = LookupType(reg);
    switch (t)
    {
    case PM3200Data::RegisterType::UInt16:
        return 1;
    case PM3200Data::RegisterType::UInt32:
        return 2;
    case PM3200Data::RegisterType::Float32:
        return 2;
    case PM3200Data::RegisterType::Int64:
        return 4;
    case PM3200Data::RegisterType::DateTime:
        return 4;
    }
    return 0;
}
