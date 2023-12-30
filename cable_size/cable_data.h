#include <stddef.h>

#ifndef CABLE_DATA_H
#define CABLE_DATA_H

struct CableData
{
    int id;
    float value;
};

float searchData(struct CableData *data, size_t size, float value);

float lookupTemperatureValue(int searchId);

float lookupInsulationValue(int length);

float lookupGroupingValue(int searchId);

float lookupCableSize(float currentDemand);

float lookupVoltageDropFactor(float csa);

#endif