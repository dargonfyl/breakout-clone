#pragma once

#include <math.h>


/*
A collection of helpers for math and whatever.
*/

/**
 * Clamps a value between two other values.
 * 
 * @param value Value to clamp
 * @param max   Max value
 * @param min   Min value
 * 
 * @return `float`: clamped value
 */
float clamp(float value, float max, float min);
