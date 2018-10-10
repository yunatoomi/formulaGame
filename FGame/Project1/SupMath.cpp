#include "SupMath.h";
float clamp(float min, float max, float val) {
	if (val < min) {
		return min;
	}
	else if (val > max) {
		return max;
	}
	return val;
}

double normalizeFloor(double val, double cellVal) {
	return floor(val / cellVal) * cellVal;
}

double normalizeCeil(double val, double cellVal) {
	return ceil(val / cellVal) * cellVal;
}

double normalizeRound(double val, double cellVal) {
	return round(val / cellVal) * cellVal;
}

double normalizeTrunc(double val, double cellVal) {
	return trunc(val / cellVal) * cellVal;
}