#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "cable_data.h"

#define SUPPLY_VOLTAGE 230

struct Installation
{
    int power;
    float length;
    bool insulated;
    int grouping;
    float amb_temp;
    int protection_device;
    float design_current;
    float real_current;
};

void calcDesignCurrent(struct Installation *inst)
{
    inst->design_current = inst->power / SUPPLY_VOLTAGE;
}

int findCircuitBreakerSize(struct Installation *inst)
{
    int circuitBreakers[6] = {6, 10, 20, 32, 40, 60};

    for (int i = 0; i < sizeof(circuitBreakers); i++)
    {
        if (inst->design_current < circuitBreakers[i])
        {
            inst->protection_device = circuitBreakers[i];
            return 0;
        }
    }

    errno = ERANGE;
    return -1;
}

void calcCableCapacity(struct Installation *inst)
{
    float c_a, c_g, c_i = 1; // Declare cable factors
    c_g = lookupGroupingValue(inst->grouping);
    c_a = lookupTemperatureValue(inst->amb_temp);

    inst->real_current = (inst->design_current / (c_a * c_g));
}

int main()
{
    struct Installation test;

    test.power = 5000;
    test.length = 10;
    test.insulated = false;

    test.amb_temp = 30;
    test.grouping = 2;

    // Calculate design current and CB size
    calcDesignCurrent(&test);
    int cb = findCircuitBreakerSize(&test);

    if (cb == -1)
    {
        perror("Error getting Circuit Breaker Size");
        return -1;
    }

    // Calculate i_t based on grouping/temp/insulated
    calcCableCapacity(&test);

    float cable_size = lookupCableSize(test.real_current);

    printf("Design Current: %.1fA\n", test.design_current);
    printf("Circuit Breaker Size: %dA\n", cb);
    printf("Real Current: %.1fA\n", test.real_current);
    printf("Cable Size: %.1fmm\n", cable_size);

    return 0;
}
