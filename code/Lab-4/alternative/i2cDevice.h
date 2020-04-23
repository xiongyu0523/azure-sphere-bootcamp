#pragma once

#include <stdbool.h>

#define LSM6DSO_ID         0x6C   // register value
#define LSM6DSO_ADDRESS	   0x6A	  // I2C Address

int initI2cDevice(void);
void closeI2cDevice(void);