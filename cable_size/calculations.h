#include <stdbool.h>

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

struct Installation
{
    int power;
    float length;
    int insulation;
    int grouping;
    float amb_temp;
    int protection_device;
    bool lighting_circuit;
    bool bs3036_fuse;
};

#define SUPPLY_VOLTAGE 230

float calculateDesignCurrent(int power);
float calculateCableCapacity(float designCurrent, struct Installation *installation);
float calculateVoltageDrop(float csa, float designCurrent, float length);

#endif