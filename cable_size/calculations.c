#include "cable_data.h"
#include <stdio.h>
#include <stdbool.h>
#include "calculations.h"

float calculateDesignCurrent(int power)
{
    return (power / SUPPLY_VOLTAGE);
}

// i_t = i_n / c_i * c_g * c_a
float calculateCableCapacity(float designCurrent, struct Installation *installation)
{
    float c_a, c_g, c_i, c_f = 1; // Declare cable factors
    c_g = lookupGroupingValue(installation->grouping);
    c_a = lookupTemperatureValue(installation->amb_temp, installation->cable);
    c_i = lookupInsulationValue(installation->insulation);

    if (installation->bs3036_fuse)
    {
        c_f = 0.725;
    }

    return (designCurrent / (c_a * c_g * c_i * c_f));
}

// v_d = factor * i_b * length / 1000
float calculateVoltageDrop(float csa, float designCurrent, float length)
{
    float factor = lookupVoltageDropFactor(csa);
    printf("Voltage Drop Factor (mV/m): %.1f\n", factor);

    return (factor * designCurrent * length) /
           1000;
}
