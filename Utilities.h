#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H
#endif
#include "Nanograms.h"

class Utilities
{
public:
	long allZeroLong;

	Utilities();
	void updateCrossFillRowByCol(Nanograms* nanograms);
	void updateCrossFillColByRow(Nanograms* Nanograms);
};

