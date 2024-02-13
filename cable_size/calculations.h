#include <stdbool.h>
#include <stdio.h>

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

enum CableType
{
    THERMOPLASTIC = 1,
    THERMOSETTING = 2
};

enum Fuse
{
    BS60898 = 1
};

enum InstallationMethod
{
    THERMALLY_INSULATING_WALL = 1,
    CONDUIT = 2,
    CLIPPED = 3
};

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
    enum CableType cable;
    enum InstallationMethod method;
};

#define SUPPLY_VOLTAGE 230

float calculateDesignCurrent(int power);
float calculateCableCapacity(float designCurrent, struct Installation *installation);
float calculateVoltageDrop(float csa, float designCurrent, float length);
float calculateZs(float csa, float z_e, struct Installation *installation);
float calculateAdiabaticEquation(float z_s);

#endif