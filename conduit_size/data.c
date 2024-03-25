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

int getConduitFactor(struct ConduitFactors *data, size_t size, int bends, float length, float conduit_csa)
{
  // Use bends to identify index in Table E4
  // Determine size of Circuits based on bends
  size_t circuit_factor_size = sizeof(data[bends].circuits) / sizeof(data[bends].circuits[0]);
  printf("\nCircuit Factor Size: %zu", circuit_factor_size);

  // Determine which factors to consider based on length
  for (int i = 0; i < circuit_factor_size; i++) {
    if (length <= data[bends].circuits[i].length) {
      size_t conduit_factor_size = sizeof(data[bends].circuits[i].factors) / sizeof(data[bends].circuits[i].factors[0]);
      return searchData(data[bends].circuits[i].factors, conduit_factor_size, conduit_csa);
    }
    printf("\nLength is >%fm", data[bends].circuits[i].length);
  }

  return 0;
}

