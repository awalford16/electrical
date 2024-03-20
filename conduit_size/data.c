#include <stdlib.h>
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
