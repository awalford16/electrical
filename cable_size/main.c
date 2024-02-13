#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cable_data.h"
#include "calculations.h"

#define SUPPLY_VOLTAGE 230
#define SUPPLY_ZE 0.2
#define PERMISSABLE_VD_POWER 11.5
#define PERMISSABLE_VD_LIGHTING 6.9

int findCircuitBreakerSize(float design_current)
{
    int circuitBreakers[7] = {6, 10, 20, 32, 40, 50, 60};

    for (int i = 0; i < sizeof(circuitBreakers); i++)
    {
        if (design_current < circuitBreakers[i])
        {
            return circuitBreakers[i];
        }
    }

    errno = ERANGE;
    return -1;
}

const char *getArgOrDefault(int argc, char *argv[], int index, const char *defaultValue)
{
    if (index < argc)
    {
        return argv[index];
    }
    else
    {
        return defaultValue;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <power (W)> <length (m)>\n", argv[0]);
        return 1; // Exit with an error code
    }

    struct Installation test;

    test.power = atoi(argv[1]);
    test.length = atoi(argv[2]);
    test.insulation = atoi(getArgOrDefault(argc, argv, 3, "0"));
    test.amb_temp = atoi(getArgOrDefault(argc, argv, 4, "25"));
    test.grouping = atoi(getArgOrDefault(argc, argv, 5, "1"));

    test.lighting_circuit = false;
    test.bs3036_fuse = false;
    test.cable = THERMOPLASTIC;
    test.method = CLIPPED;

    // Calculate design current and CB size
    float i_b = calculateDesignCurrent(test.power);
    printf("Design Current: %.1fA\n", i_b);

    int i_n = findCircuitBreakerSize(i_b);
    if (i_n == -1)
    {
        perror("Error getting Circuit Breaker Size");
        return -1;
    }
    printf("Circuit Breaker Size: %dA\n", i_n);

    // Calculate i_t based on grouping/temp/insulated
    float i_t = calculateCableCapacity(i_n, &test);
    printf("Real Current: %.1fA\n", i_t);

    float cable_size = lookupCableSize(i_t, test.method);
    printf("Cable Size: %.1fmm\n", cable_size);

    // Determine voltage drop
    // Permissable Vd is 6.9V for lighting, 11.5V for power
    float v_d = calculateVoltageDrop(cable_size, i_b, test.length);
    printf("Votage Drop: %.2fV\n", v_d);

    if ((v_d < PERMISSABLE_VD_POWER && !test.lighting_circuit) || (v_d < PERMISSABLE_VD_LIGHTING && test.lighting_circuit))
    {
        printf("Voltage drop is acceptable\n\n");
    }
    else
    {
        printf("Voltage drop is not permissable\n\n");
    }

    // Determine PFC - enough to trip breaker?

    // Z_s = Z_e + ((R1+R2)*1.2*L/1000)
    // Include correction factors for R1+R2 if temp not 20
    float cpc = lookupCPCSize(cable_size);
    printf("Selected CPC Size: %.2fmm\n", cpc);
    float z_s = calculateZs(cable_size, SUPPLY_ZE, &test);

    // Check the CPC is large enough to handle fault current
    // Determine I_fault = 230/Z_s
    // Determine factor k from tables
    float s = calculateAdiabaticEquation(z_s);
    printf("Minimum CPC Size: %.2fmm\n", s);

    if (s < cpc)
    {
        printf("CPC size is acceptable\n");
    }
    else
    {
        printf("CPS size is not permissable\n");
    }

    return 0;
}
