#include <stdio.h>
#include <stddef.h>
#include "cable_data.h"

float searchData(struct CableData *data, size_t size, float value)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i].id == value)
        {
            return data[i].value;
        }
    }

    // Return a value indicating that the ID was not found
    return 1.0;
}

float checkUnderValue(struct CableData *data, size_t size, float value)
{
    for (int i = 0; i < size; i++)
    {
        if (value < data[i].id)
        {
            return data[i].value;
        }
    }

    return -1;
}

float lookupTemperatureValue(int searchId)
{
    // Considering just 70 degree thermoplastic
    struct CableData data[] = {
        {25, 1.03},
        {30, 1.00},
        {35, 0.94},
        {40, 0.87}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return searchData(&data, l, searchId);
}

// Insulation ratings 50-100, 100-200, 200-400
float lookupInsulationValue(int length)
{
    if (length > 500)
    {
        return 0.5;
    }

    if (length < 50)
    {
        return 1;
    }

    struct CableData data[] = {
        {100, 0.88},
        {200, 0.78},
        {400, 0.63},
        {500, 0.51}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return checkUnderValue(&data, l, length);
}

float lookupGroupingValue(int searchId)
{
    struct CableData data[] = {
        {1, 1.0},
        {2, 0.8},
        {3, 0.7},
        {4, 0.65},
        {5, 0.60},
        {6, 0.57}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return searchData(&data, l, searchId);
}

float lookupCableSize(float currentDemand)
{
    // Only considering 70 degrees thermoplastic clipped direct
    struct CableData data[] = {
        {16, 1.0},
        {20, 1.5},
        {27, 2.5},
        {37, 4.0},
        {47, 6.0},
        {65, 10.0},
        {87, 16.0},
        {114, 25.0},
        {141, 35.0},
        {382, 50.0}};

    size_t l = sizeof(data) / sizeof(struct CableData);
    return checkUnderValue(data, l, currentDemand);
}

// Determine Vd in mV/m based on cable CSA
float lookupVoltageDropFactor(float csa)
{
    struct CableData data[] = {
        {1.5, 29},
        {2.5, 18},
        {4, 11},
        {6, 7.3},
        {10, 4.4},
        {16, 2.8}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return searchData(&data, l, csa);
}
