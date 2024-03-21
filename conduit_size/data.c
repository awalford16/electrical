#include <stdlib.h>
#include <stdio.h>
#include "data.h"

int searchData(struct Factors *data, size_t size, float key)
{
  for (int i = 0; i < size; i++)
  {
    if (data[i].id == key)
    {
      return data[i].factor;
    }
  }

  // Return a value indicating that the ID was not found
  return 0;
}


struct Factors* getFactors(struct CircuitFactors *data, size_t size, float value)
{
  printf("\nSize: %d", size);
  for (int i = 0; i < size; i++)
  {
    if (value <= data[i].length)
    {
        return data[i].factors;
    }
  }

  return NULL;
}

struct CircuitFactors* getCircuitFactors(struct ConduitFactors *data, size_t size, int value)
{
  for (int i = 0; i < size; i++)
  {
    if (value == data[i].bends)
    {
        return data[i].circuits;
    }
  }

  return NULL;
}

int getConduitFactor(struct ConduitFactors *data, size_t size, int bends, float length, float conduit_csa)
{
  struct CircuitFactors* circuit_factors = getCircuitFactors(data, size, bends);

  if (circuit_factors == NULL) {
    printf("\nFailed to get Circuit Factors");
    return 0;
  }

  size_t circuit_factor_size = sizeof(&circuit_factors) / sizeof(&circuit_factors[0]);
  struct Factors* factors = getFactors(circuit_factors, circuit_factor_size, length);

  if (factors == NULL) {
    printf("\nFailed to get factors");
    return 0;
  }

  size_t factor_size = sizeof(&factors) / sizeof(&factors[0]);
  return searchData(factors, factor_size, conduit_csa);
}
