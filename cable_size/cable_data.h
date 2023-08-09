#ifndef CABLE_DATA_H
#define CABLE_DATA_H

struct CableData
{
    int id;
    float value;
};

float lookupTemperatureValue(int searchId);

float lookupGroupingValue(int searchId);

float lookupCableSize(float currentDemand);

#endif