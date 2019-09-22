#include "Utilities.h"
#include <bitset>
#include <iostream>
using namespace std;

Utilities::Utilities()
{
	long zero = 0;
	allZeroLong = zero ^ zero;
}

void Utilities::updateCrossFillColByRow(Nanograms* nanograms)
{
	long zero = 0;
	long allZero = zero ^ zero;
	long initCross = ~allZero << nanograms->colSize;
	vector<long> updateMustCrossCol(nanograms->colSize, initCross);
	vector<long> updateMustFillCol(nanograms->colSize, allZero);
	for (int i = 0; i < nanograms->rowSize; i++) {
		for (int j = 0; j < nanograms->colSize; j++) {
			if (nanograms->mustCrossRow.at(i) & (1 << j)) {
				updateMustCrossCol.at(j) |= 1UL << nanograms->rowSize - i - 1;
			}
			if (nanograms->mustFillRow.at(i) & (1 << j)) {
				updateMustFillCol.at(j) |= 1UL << nanograms->rowSize - i - 1;
			}
		}
	}
	reverse(updateMustCrossCol.begin(), updateMustCrossCol.end());
	reverse(updateMustFillCol.begin(), updateMustFillCol.end());
	for (int i = 0; i < nanograms->colSize; i++) {
		nanograms->mustCrossCol.at(i) |= updateMustCrossCol.at(i);
		nanograms->mustFillCol.at(i) |= updateMustFillCol.at(i);
	}
}


void Utilities::updateCrossFillRowByCol(Nanograms* nanograms)
{
	long zero = 0;
	long allZero = zero ^ zero;
	long initCross = ~allZero << nanograms->rowSize;
	vector<long> updateMustCrossRow(nanograms->rowSize, initCross);
	vector<long> updateMustFillRow(nanograms->rowSize, allZero);
	for (int i = 0; i < nanograms->colSize; i++) {
		for (int j = 0; j < nanograms->rowSize; j++) {
			if (nanograms->mustCrossCol.at(i) & (1 << j)) {
				updateMustCrossRow.at(j) |= 1UL << nanograms->colSize - i - 1;
			}
			if (nanograms->mustFillCol.at(i) & (1 << j)) {
				updateMustFillRow.at(j) |= 1UL << nanograms->colSize - i - 1;
			}
		}
	}
	reverse(updateMustCrossRow.begin(), updateMustCrossRow.end());
	reverse(updateMustFillRow.begin(), updateMustFillRow.end());
	for (int i = 0; i < nanograms->rowSize; i++) {
		nanograms->mustCrossRow.at(i) |= updateMustCrossRow.at(i);
		nanograms->mustFillRow.at(i) |= updateMustFillRow.at(i);
	}
}

void Utilities::initAllStart(Nanograms* nanograms)
{
	long thisCondition;
	for (int i = 0; i < nanograms->rowSize; i++) {
		for (int j = 0; j < nanograms->rowCondition.at(i).size(); j++) {
			unsigned short int leftPad = -2;
			unsigned short int rightPad = nanograms->rowSize;
			for (int k = j; k < nanograms->rowCondition.at(i).size(); k++) {
				leftPad += nanograms->rowCondition.at(i).at(k) + 1;
			}
			for (int k = 0; k < j; k++) {
				rightPad -= nanograms->rowCondition.at(i).at(k) + 1;
			}
			thisCondition = (((1 << (rightPad)) - 1) ^ ((1 << (leftPad)) - 1));
			nanograms->allStartRow.at(i).at(j) = thisCondition;
		}
	}
	for (int i = 0; i < nanograms->colSize; i++) {
		for (int j = 0; j < nanograms->colCondition.at(i).size(); j++) {
			unsigned short int leftPad = -2;
			unsigned short int rightPad = nanograms->colSize;
			for (int k = j; k < nanograms->colCondition.at(i).size(); k++) {
				leftPad += nanograms->colCondition.at(i).at(k) + 1;
			}
			for (int k = 0; k < j; k++) {
				rightPad -= nanograms->colCondition.at(i).at(k) + 1;
			}
			thisCondition = fillLongBitByRange(rightPad, leftPad);
			nanograms->allStartCol.at(i).at(j) = thisCondition;
		}
	}
}

///////sad/sad/sad/
void Utilities::updateFillCrossByStart(Nanograms* nanograms)
{
	long one = 0;
	long allOne = (~one) | one;
	long mustFill;
	long mustCross;
	long mayFill;
	long startRow;
	long startCol;
	long mayFillLeft;
	long mayFillRight;
	unsigned short int thisConditionSize;
	unsigned short int rightSetBit;
	unsigned short int leftSetBit;
	long thisCondition;
	for (int i = 0; i < nanograms->rowSize; i++) {
		mustCross = allOne;
		for (int j = 0; j < nanograms->allStartRow.at(i).size(); j++) {
			startRow = nanograms->allStartRow.at(i).at(j);
			thisConditionSize = nanograms->rowCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			rightSetBit = findRightMostSetBitLong(startRow, nanograms->rowSize);
			leftSetBit = findLeftMostSetBitLong(startRow, nanograms->rowSize);
			mayFillRight = thisCondition << rightSetBit + 1 - thisConditionSize;
			mayFillLeft = thisCondition << leftSetBit + 1 - thisConditionSize;
			mustFill = mayFillLeft & mayFillRight;
			mayFill = (startRow | mayFillRight);
			mustCross &= ~mayFill;
			nanograms->mustFillRow.at(i) |= mustFill;
		}
		nanograms->mustCrossRow.at(i) |= mustCross;
	}
	updateCrossFillColByRow(nanograms);
	for (int i = 0; i < nanograms->colSize; i++) {
		mustCross = allOne;
		for (int j = 0; j < nanograms->allStartCol.at(i).size(); j++) {
			startCol = nanograms->allStartCol.at(i).at(j);
			thisConditionSize = nanograms->colCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			rightSetBit = findRightMostSetBitLong(startCol, nanograms->colSize);
			leftSetBit = findLeftMostSetBitLong(startCol, nanograms->colSize);
			mayFillRight = thisCondition << rightSetBit + 1 - thisConditionSize;
			mayFillLeft = thisCondition << leftSetBit + 1 - thisConditionSize;
			mustFill = mayFillLeft & mayFillRight;
			mayFill = (startCol | mayFillRight);
			mustCross &= ~mayFill;
			nanograms->mustFillCol.at(i) |= mustFill;
		}
		nanograms->mustCrossCol.at(i) |= mustCross;
	}
	updateCrossFillRowByCol(nanograms);
}

void Utilities::updateStartByFillCross(Nanograms* nanograms)
{
	long one = 0;
	long allOne = (~one) | one;
	long allZero = (~one) & one;
	long mustFill;
	long mustCross;
	long mayFill;
	long start;
	long mayFillLeft;
	long mayFillRight;
	long leftRightIsFill;
	unsigned short int thisConditionSize;
	unsigned short int rightSetBit;
	unsigned short int leftSetBit;
	long thisCondition;
	//cross only filter
	cout << "updateStartByFillCross starst" << endl;
	nanograms->printAllBit();
	cout << "updateStartByFillCross end" << endl;

	for (int i = 0; i < nanograms->rowSize; i++) {
		for (int j = 0; j < nanograms->allStartRow.at(i).size(); j++) {
			start = nanograms->allStartRow.at(i).at(j);
			thisConditionSize = nanograms->rowCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			mustCross = nanograms->mustCrossRow.at(i);
			mustFill = nanograms->mustFillRow.at(i);
			rightSetBit = findRightMostSetBitLong(start, nanograms->rowSize);
			leftSetBit = findLeftMostSetBitLong(start, nanograms->rowSize);
			for (int k = rightSetBit + 1 - thisConditionSize; k < leftSetBit + 2 - thisConditionSize; k++) {
				if (((thisCondition << k) & mustCross) != allZero) {
					nanograms->allStartRow.at(i).at(j) = nanograms->allStartRow.at(i).at(j) & ~(1 << k + thisConditionSize - 1);
					continue;
				}
				if (k == 0) {
					leftRightIsFill = 1UL << k + thisConditionSize;
				}
				else {
					leftRightIsFill = (1UL << k + thisConditionSize) | (1UL << k - 1);
				}
				if ((leftRightIsFill & mustFill) != allZero) {
					nanograms->allStartRow.at(i).at(j) = nanograms->allStartRow.at(i).at(j) & ~(1 << k + thisConditionSize - 1);
					continue;
				}
			}
		}
	}
	for (int i = 0; i < nanograms->colSize; i++) {
		for (int j = 0; j < nanograms->allStartCol.at(i).size(); j++) {
			start = nanograms->allStartCol.at(i).at(j);
			thisConditionSize = nanograms->colCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			mustCross = nanograms->mustCrossCol.at(i);
			rightSetBit = findRightMostSetBitLong(start, nanograms->colSize);
			leftSetBit = findLeftMostSetBitLong(start, nanograms->colSize);
			for (int k = rightSetBit + 1 - thisConditionSize; k < leftSetBit + 1 - thisConditionSize; k++) {
				if (((thisCondition << k) & mustCross) != allZero) {
					nanograms->allStartCol.at(i).at(j) = nanograms->allStartCol.at(i).at(j) & ~(1 << k);
				}
			}
		}
	}
}

void Utilities::updateFillCrossBySolveStart(Nanograms* nanograms)
{
	long one = 0;
	long allOne = (~one) | one;
	long mustFill;
	long mustCross;
	long mayFill;
	long start;
	long startCol;
	long mayFillLeft;
	long mayFillRight;
	int solveCounter;
	unsigned short int thisConditionSize;
	unsigned short int rightSetBit;
	unsigned short int leftSetBit;
	long thisCondition;
	for (int i = 0; i < nanograms->rowSize; i++) {
		mustCross = allOne;
		solveCounter = 0;
		for (int j = 0; j < nanograms->allStartRow.at(i).size(); j++) {
			start = nanograms->allStartRow.at(i).at(j);
			rightSetBit = findRightMostSetBitLong(start, nanograms->rowSize);
			leftSetBit = findLeftMostSetBitLong(start, nanograms->rowSize);
			thisConditionSize = nanograms->rowCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			if (rightSetBit == leftSetBit) {
				solveCounter++;
				nanograms->mustFillRow.at(i) |= (thisCondition << rightSetBit + 1 - thisConditionSize);
				nanograms->mustCrossRow.at(i) |= (thisCondition << rightSetBit + 1);
				if ((rightSetBit - thisConditionSize + 1) != 0) {
					nanograms->mustCrossRow.at(i) |= (thisCondition << rightSetBit - thisConditionSize);
				}
			}
		}
		if (solveCounter == nanograms->allStartRow.at(i).size()) {
			nanograms->mustCrossRow.at(i) = ~(nanograms->mustFillRow.at(i));
		}
	}
	for (int i = 0; i < nanograms->colSize; i++) {
		mustCross = allOne;
		solveCounter = 0;
		for (int j = 0; j < nanograms->allStartCol.at(i).size(); j++) {
			start = nanograms->allStartCol.at(i).at(j);
			rightSetBit = findRightMostSetBitLong(start, nanograms->colSize);
			leftSetBit = findLeftMostSetBitLong(start, nanograms->colSize);
			thisConditionSize = nanograms->colCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			if (rightSetBit == leftSetBit) {
				solveCounter++;
				nanograms->mustFillCol.at(i) |= (thisCondition << rightSetBit + 1 - thisConditionSize);
				nanograms->mustCrossCol.at(i) |= (thisCondition << rightSetBit + 1);
				if ((rightSetBit - thisConditionSize + 1) != 0) {
					nanograms->mustCrossCol.at(i) |= (thisCondition << rightSetBit - thisConditionSize);
				}
			}
		}
		if (solveCounter == nanograms->allStartCol.at(i).size()) {
			nanograms->mustCrossCol.at(i) = ~(nanograms->mustFillCol.at(i));
		}
	}
}

//not done
bool isPossible(
	Nanograms* nanograms,
	bool isRow,
	unsigned short int rowNumber,
	unsigned short int gapStart,
	unsigned short int gapEnd,
	unsigned short int conditionStart,
	unsigned short int conditionEnd
) {
	unsigned short int size;
	if (isRow) {
		size = nanograms->rowSize;
	}
	else {
		size = nanograms->colSize;
	}
}

long Utilities::fillLongBitByRange(unsigned short int start, unsigned short int end)
{
	return (((1 << (start)) - 1) ^ ((1 << (end)) - 1));;
}

unsigned short int Utilities::findRightMostSetBitLong(long number, unsigned short int size)
{
	long allZero = ~number & number;
	for (unsigned short int i = 0; i < size; i++) {
		if ((number & (1UL << i)) != allZero) {
			return i;
		}
	}
	return -1;
}

unsigned short int Utilities::findLeftMostSetBitLong(long number, unsigned short int size)
{
	long allZero = ~number & number;
	for (unsigned short int i = size - 1; i > 0; i--) {
		if ((number & (1UL << i)) != allZero) {
			return i;
		}
	}
	if ((number & (1UL << 0)) != allZero) {
		return 0;
	}
	return -1;
}



