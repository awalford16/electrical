#include "cable_data.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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

float calculateZs(float csa, float z_e, struct Installation *installation)
{
    // Determine r1_r2 correction factor
    float r1r2_correction = 1 + (0.004 * (installation->amb_temp - 20));

    // Include calculation for length and 70 degree correction factor
    float r1_r2 = lookupR1R2(csa);
    float r1_r2_final = ((r1_r2 * r1r2_correction) * installation->length * 1.2) / 1000;

    return (z_e + r1_r2_final);
}

float calculateAdiabaticEquation(float z_s)
{
    // Lookup disconnection time
    float t = lookupDisconnectionTime(1.0);

    // Lookup k factor
    float k = lookupKFactor(THERMOPLASTIC);

    // Determine PFC
    float i = SUPPLY_VOLTAGE / z_s;

    return (sqrt((i * i) * t) / k);
}
