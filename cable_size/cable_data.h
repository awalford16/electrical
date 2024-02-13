#include <stddef.h>

#ifndef CABLE_DATA_H
#define CABLE_DATA_H

// Support multiple different reference values based on cable type and clipping etc.
struct CableData
{
    float id;
    float ref_a;
    float ref_b;
    float ref_c;
};

float searchData(struct CableData *data, size_t size, float value, int field);

float lookupTemperatureValue(int searchId, int ref);

float lookupInsulationValue(int length);

float lookupGroupingValue(int searchId);

float lookupCableSize(float currentDemand, int ref);

float lookupVoltageDropFactor(float csa);

float lookupCPCSize(float csa);

float lookupR1R2(float csa);

float lookupDisconnectionTime(float fuse);

float lookupKFactor(int cable_type_field);

#endif