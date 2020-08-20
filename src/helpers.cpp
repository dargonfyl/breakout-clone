#include "helpers.h"


float clamp(float value, float max, float min) {
	return fmaxf(min, fminf(value, max));
}
