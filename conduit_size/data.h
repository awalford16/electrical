#include <stdlib.h>

#ifndef __DATA_H__
#define __DATA_H__

struct Factors
{
  float id;
  int factor;
};

struct CircuitFactors
{
  float length;
  struct Factors factors[10];
};

struct ConduitFactors
{
  int bends;
  struct CircuitFactors circuits[15];
};

int searchData(struct Factors *data, size_t size, float key);
struct Factors* getConduitFactors(struct CircuitFactors *data, size_t size, float value);
struct CircuitFactors* getCircuitFactors(struct ConduitFactors *data, size_t size, int value);
int getConduitFactor(struct ConduitFactors *data, size_t size, int bends, float length, float conduit_csa);

#endif

