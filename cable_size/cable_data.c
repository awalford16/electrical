#include <stdio.h>
#include "cable_data.h"

float lookupTemperatureValue(int searchId)
{
    struct CableData data[] = {
        {25, 1.03},
        {30, 1.00},
        {35, 0.94},
        {40, 0.87}};

    int size = sizeof(data) / sizeof(struct CableData);
    for (int i = 0; i < size; i++)
    {
        if (data[i].id == searchId)
        {
            return data[i].value;
        }
    }

    // Return a value indicating that the ID was not found
    return 1.0;
}

float lookupGroupingValue(int searchId)
{
    struct CableData data[] = {
        {1, 1.0},
        {2, 0.8},
        {3, 0.7},
        {4, 0.65},
        {5, 0.60}};

    int size = sizeof(data) / sizeof(struct CableData);
    for (int i = 0; i < size; i++)
    {
        if (data[i].id == searchId)
        {
            return data[i].value;
        }
    }

    // Return a value indicating that the ID was not found
    return 1.0;
}

float lookupCableSize(float currentDemand)
{
    // Ignoring insulation and other installation demands
    struct CableData data[] = {
        {20, 1.0},
        {25, 1.5},
        {30, 2.5},
        {35, 4.0},
        {40, 6.0}};

    int size = sizeof(data) / sizeof(struct CableData);
    for (int i = 0; i < size; i++)
    {
        if (currentDemand < data[i].id)
        {
            return data[i].value;
        }
    }

    // Install largest cable if current demand is higher
    return 10.0;
}
