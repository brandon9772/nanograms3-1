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
	void updateCrossFillColByRow(Nanograms* nanograms);
	void initAllStart(Nanograms* nanograms);
	void updateFillCrossByStart(Nanograms* nanograms);
	void updateStartByFillCross(Nanograms* nanograms);
	void updateFillCrossBySolveStart(Nanograms* nanograms);
	bool isPossible(
		Nanograms* nanograms,
		bool isRow,
		unsigned short int rowNumber,
		unsigned short int gapStart,
		unsigned short int gapEnd,
		unsigned short int conditionStart,
		unsigned short int conditionEnd
	);
	long fillLongBitByRange(unsigned short int start, unsigned short int end);
	unsigned short int findLeftMostSetBitLong(long number, unsigned short int size);
	unsigned short int findRightMostSetBitLong(long number, unsigned short int size);
};

