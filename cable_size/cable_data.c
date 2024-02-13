#include <stdio.h>
#include <stddef.h>
#include "cable_data.h"

float selectField(struct CableData *data, int field)
{
    switch (field)
    {
    case 1:
        return data->ref_a;
    case 2:
        return data->ref_b;
    case 3:
        return data->ref_c;
    // Add more cases for other data types as needed
    default:
        printf("Unsupported field\n");
        return -1;
    }
}

float searchData(struct CableData *data, size_t size, float value, int field)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i].id == value)
        {
            return selectField(&data[i], field);
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
            return data[i].ref_a;
        }
    }

    return -1;
}

float lookupTemperatureValue(int searchId, int ref)
{
    // indexes: 1 = thermoplastic, 2 = thermosetting
    struct CableData data[] =
        {
            {25, 1.03, 1.02},
            {30, 1.00, 1.00},
            {35, 0.94, 0.96},
            {40, 0.87, 0.91}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return searchData(&data, l, searchId, ref);
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
    return searchData(&data, l, searchId, 1);
}

float lookupCableSize(float currentDemand, int ref)
{
    // indexes: #1 = thermally insulating, #2 = conduit, #3 = clipped
    struct CableData data[] = {
        {1.0, 11, 13.5, 15.5},
        {1.5, 14.5, 17.5, 20},
        {2.5, 20, 24, 27},
        {4.0, 26, 32, 37},
        {6.0, 34, 41, 47},
        {10.0, 46, 57, 65},
        {16.0, 56, 68, 87},
        {25.0, 80, 101, 114}};
    // {20, 1.5},
    // {27, 2.5},
    // {37, 4.0},
    // {47, 6.0},
    // {65, 10.0},
    // {87, 16.0},
    // {114, 25.0},
    // {141, 35.0},
    // {382, 50.0}};

    size_t l = sizeof(data) / sizeof(struct CableData);
    for (int i = 0; i < l; i++)
    {
        if (currentDemand < selectField(&data[i], ref))
        {
            return data[i].id;
        }
    }

    return -1;
}

// Determine Vd in mV/m based on cable CSA
float lookupVoltageDropFactor(float csa)
{
    // For 2 cable single-phase AC, all values are same up to 16mm
    // Irrelevant of installation method
    struct CableData data[] = {
        {1.5, 29},
        {2.5, 18},
        {4, 11},
        {6, 7.3},
        {10, 4.4},
        {16, 2.8}};

    size_t l = sizeof(data) / sizeof(data[0]);
    return searchData(&data, l, csa, 1);
}

// Lookup possible size of CPC given line CSA
float lookupCPCSize(float csa)
{
    struct CableData data[] = {
        {1.0, 1.0},
        {1.5, 1.0, 1.5},
        {2.5, 1.0, 1.5, 2.5},
        {4, 1.5, 2.5, 4},
        {6, 2.5, 4, 6},
        {10, 4, 6, 10},
        {16, 6, 10, 16},
    };

    size_t l = sizeof(data) / sizeof(data[0]);

    // Add logic to use higher field indexes if the CPC is not sufficient
    return searchData(&data, l, csa, 1);
}

// Lookup R1R2 value given CPC CSA
float lookupR1R2(float csa)
{
    // Fields in the struct are based off of the respective field of cpc CSAs
    struct CableData data[] = {
        {1.0, 36.20},
        {1.5, 30.2, 24.2},
        {2.5, 25.51, 19.51, 14.82},
        {4, 16.71, 12.02, 9.22},
        {6, 10.49, 7.69, 6.16},
        {10, 6.44, 4.91, 3.66},
        {16, 4.23, 2.98, 2.30},
    };

    size_t l = sizeof(data) / sizeof(data[0]);

    // Add logic to use higher field indexes if the CPC is not sufficient
    return searchData(&data, l, csa, 1);
}

float lookupDisconnectionTime(float fuse)
{
    return 0.1;
}

// Select K based on thermoplastic/thermosetting
float lookupKFactor(int cable_type_field)
{
    struct CableData data[] = {
        {0.0, 115, 130}};

    size_t l = sizeof(data) / sizeof(data[0]);

    // Add logic to use higher field indexes if the CPC is not sufficient
    return searchData(&data, l, 0.0, cable_type_field);
}
