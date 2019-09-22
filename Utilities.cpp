#include "Utilities.h"
#include <bitset>

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
	nanograms->mustCrossCol = updateMustCrossCol;
	nanograms->mustFillCol = updateMustFillCol;
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
	unsigned short int thisConditionSize;
	unsigned short int rightSetBit;
	unsigned short int leftSetBit;
	long thisCondition;
	//cross only filter
	for (int i = 0; i < nanograms->rowSize; i++) {
		for (int j = 0; j < nanograms->allStartRow.at(i).size(); j++) {
			start = nanograms->allStartRow.at(i).at(j);
			thisConditionSize = nanograms->rowCondition.at(i).at(j);
			thisCondition = fillLongBitByRange(0, thisConditionSize);
			mustCross = nanograms->mustCrossRow.at(i);
			rightSetBit = findRightMostSetBitLong(start, nanograms->rowSize);
			leftSetBit = findLeftMostSetBitLong(start, nanograms->rowSize);
			for (int k = rightSetBit + 1 - thisConditionSize; k < leftSetBit + 1 - thisConditionSize; k++) {
				if (((thisCondition << k) & mustCross) != allZero) {
					nanograms->allStartRow.at(i).at(j) = nanograms->allStartRow.at(i).at(j) & ~(1 << k);
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

long Utilities::fillLongBitByRange(unsigned short int start, unsigned short int end)
{
	return (((1 << (start)) - 1) ^ ((1 << (end)) - 1));;
}

unsigned short int Utilities::findRightMostSetBitLong(long number, unsigned short int size)
{
	long allZero = ~number & number;
	for (unsigned short int i = 0; i < size; i++) {
		if ((number & (1UL << i))!= allZero) {
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
	return - 1;
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
	nanograms->mustCrossRow = updateMustCrossRow;
	nanograms->mustFillRow = updateMustFillRow;
}
