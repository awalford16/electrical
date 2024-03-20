#include <stdlib.h>

#ifndef __DATA_H__
#define __DATA_H__

struct Factors
{
    float id;
    int factor;
};

int searchData(struct Factors *data, size_t size, float key);

#endif

