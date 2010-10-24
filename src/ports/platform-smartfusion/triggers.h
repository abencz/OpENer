/*
 * debug outputs for using GPIO to generate pulses on a scope
 * to indicate the real-time behavior of the system
 */

#ifndef TRIGGERS_H
#define TRIGGERS_H

#include "mss_gpio/mss_gpio.h"

#define trigon(n) GPIO_BITBAND->GPIO_OUT[n+11] = 1;
#define trigoff(n) GPIO_BITBAND->GPIO_OUT[n+11] = 0;

#endif
