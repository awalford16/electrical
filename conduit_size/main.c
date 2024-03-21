#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

struct Factors TABLE_E1[] = {
  {1.5, 31},
  {2.5, 43},
  {4, 58},
  {6, 88},
  {10, 146},
  {16, 202},
  {25, 385}};

struct Factors TABLE_E2[] = {
  {16, 290},
  {20, 460},
  {25, 800},
  {32, 1400},
  {38, 1900},
  {50, 3500},
  {63, 5600}};

struct Factors TABLE_E3[] = {
  {1.5, 22},
  {2.5, 30},
  {4, 43},
  {6, 58},
  {10, 105},
  {16, 145},
  {25, 217}};

// Use the # of bends as an identifier
struct ConduitFactors TABLE_E4[] = {
  {0,
    {4, {{16, 188}, {20, 370}}},
    {5, {{16, 188}, {20, 370}}}
  },
  {1,
    {1, {{16, 188}, {20, 303}, {25, 543}, {32, 947}}},
    {1.5, {{16, 188}, {20, 303}, {25, 543}, {32, 947}}},
    {2, {{16, 188}, {20, 303}, {25, 543}, {32, 947}}},
    {2.5, {{16, 188}, {20, 303}, {25, 543}, {32, 947}}}
  }
};
 

// Function to output the sizes supported for cable CSA
char* supported_sizes(struct Factors *data, size_t size) {
  char* output = malloc(5 * size);
  int k;

  for (k = 0; k < size; k++) {
      char temp[10] = ""; // Temporary string to hold the formatted number
      sprintf(temp, "%.1f", data[k].id); // Format the number as a string

      // Append the number to the main string, with a comma separator
      if (k > 0) {
          strcat(output, ", "); // Add a comma and space before the number, except for the first number
      }
      strcat(output, temp); // Append the number
  }

  return output;
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
  if (argc < 2)
  {
    printf("Usage: %s <length(M)> <cable CSA (mm)>\n", argv[0]);
    return 1; // Exit with an error code
  }

  char* endptr;

  // get args for cable diameter, conduit diameter, circuit length, num of bends
  int length = atoi(argv[1]);
  float cable_csa = strtof(argv[2], &endptr);
  int conduit_diameter = atoi(getArgOrDefault(argc, argv, 3, "20"));
  int bends = atoi(getArgOrDefault(argc, argv, 4, "0"));

  // Fail if float conversion is unsuccessful
  if (*endptr != '\0') {
    printf("Conversion error: %s\n", argv[2]);
    return 1;
  }
 
  int cable_factor;
  int conduit_factor;

  // Determine sizes of tables
  size_t e1_size = sizeof(TABLE_E1) / sizeof(TABLE_E1[0]);
  size_t e2_size = sizeof(TABLE_E2) / sizeof(TABLE_E2[0]);
  size_t e3_size = sizeof(TABLE_E3) / sizeof(TABLE_E3[0]);
  size_t e4_size = sizeof(TABLE_E4) / sizeof(TABLE_E4[1]);

  // Use tables E1 and E2 for <3m with no bends
  if (length < 3 && bends == 0) {
    cable_factor = searchData(TABLE_E1, e1_size, cable_csa);
    conduit_factor = searchData(TABLE_E2, e2_size, conduit_diameter);
  } else {
    cable_factor = searchData(TABLE_E3, e3_size, cable_csa);
    printf("\nCable Factor: %d", cable_factor);

    // Lookup circuits based on bends
    conduit_factor = getConduitFactor(TABLE_E4, e4_size, bends, length, conduit_diameter);
    if (conduit_factor == 0) {
      return 1;
    }
    printf("\nConduit Factor: %d", cable_factor);
  }

  // Error if conduit or cable size is not supported
  if (cable_factor == 0 || conduit_factor == 0) {
    printf("\nInvalid cable CSA or Conduit Diameter");
    printf("\nSupported CSA sizes: %s", supported_sizes(TABLE_E1, e1_size));
    return 1;
  }

  printf("Cable Count: %d", (conduit_factor/cable_factor));
  return 0;
}
