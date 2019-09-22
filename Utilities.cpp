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
