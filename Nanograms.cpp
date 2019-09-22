#include "Nanograms.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <bitset>
using namespace std;

Nanograms::Nanograms(string fileName) {
	cout << "started" << endl;
	string line;
	ifstream myfile(fileName);
	getline(myfile, line);
	istringstream issCol(line);
	issCol >> colSize;

	getline(myfile, line);
	istringstream issRow(line);
	issRow >> rowSize;
	long one = 1;
	long allBitOne = one | ~one;
	long startCol = (unsigned)(allBitOne << 64 - colSize) >> 64 - colSize;
	long startRow = (unsigned)(allBitOne << 64 - rowSize) >> 64 - colSize;
	long crossDefaultCol = allBitOne << colSize;
	long crossDefaultRow = allBitOne << rowSize;

	for (int i = 0; i < colSize; i++) {
		getline(myfile, line);
		istringstream entry(line);
		vector<unsigned short int> thisCol;
		vector<long> thisStart;

		for (unsigned short int condition; entry >> condition;) {
			thisCol.push_back(condition);
			thisStart.push_back(startCol);
		}
		allStartCol.push_back(thisStart);
		mustCrossCol.push_back(crossDefaultCol);
		mustFillCol.push_back(~allBitOne);
		colCondition.push_back(thisCol);

	}

	for (int i = 0; i < rowSize; i++) {
		getline(myfile, line);
		istringstream entry(line);
		vector<unsigned short int> thisRow;
		vector<long> thisStart;

		for (unsigned short int condition; entry >> condition;) {
			thisRow.push_back(condition);
			thisStart.push_back(startRow);
		}
		allStartRow.push_back(thisStart);
		mustCrossRow.push_back(crossDefaultRow);
		mustFillRow.push_back(~allBitOne);
		rowCondition.push_back(thisRow);
	}
	myfile.close();
}

void Nanograms::printAll()
{
	cout << "colSize" << endl;
	cout << colSize << endl << endl;
	cout << "rowSize" << endl;
	cout << rowSize << endl << endl;
	cout << "colCondition" << endl;
	print(colCondition);
	cout << "rowCondition" << endl;
	print(rowCondition);
	cout << "allStartCol" << endl;
	print(allStartCol);
	cout << "allStartRow" << endl;
	print(allStartRow);
	cout << "mustCrossCol" << endl;
	print(mustCrossCol);
	cout << "mustFillCol" << endl;
	print(mustFillCol);
	cout << "mustCrossRow" << endl;
	print(mustCrossRow);
	cout << "mustFillRow" << endl;
	print(mustFillRow);
}

void Nanograms::printAllBit()
{
	cout << "colSize" << endl;
	cout << colSize << endl << endl;
	cout << "rowSize" << endl;
	cout << rowSize << endl << endl;
	cout << "colCondition" << endl;
	print(colCondition);
	cout << "rowCondition" << endl;
	print(rowCondition);
	cout << "allStartCol" << endl;
	printBit(allStartCol);
	cout << "allStartRow" << endl;
	printBit(allStartRow);
	cout << "mustCrossCol" << endl;
	printBit(mustCrossCol);
	cout << "mustFillCol" << endl;
	printBit(mustFillCol);
	cout << "mustCrossRow" << endl;
	printBit(mustCrossRow);
	cout << "mustFillRow" << endl;
	printBit(mustFillRow);
}

void Nanograms::printVariable(string variable)
{
	if (variable == "colSize") {
		cout << "colSize" << endl;
		cout << colSize << endl << endl;
	}
	else if (variable == "rowSize") {
		cout << "rowSize" << endl;
		cout << rowSize << endl << endl;
	}
	else if (variable == "colCondition") {
		cout << "colCondition" << endl;
		print(colCondition);
	}
	else if (variable == "rowCondition") {
		cout << "rowCondition" << endl;
		print(rowCondition);
	}
	else if (variable == "allStartCol") {
		cout << "allStartCol" << endl;
		print(allStartCol);
	}
	else if (variable == "allStartRow") {
		cout << "allStartRow" << endl;
		print(allStartRow);
	}
	else if (variable == "mustCrossCol") {
		cout << "mustCrossCol" << endl;
		print(mustCrossCol);
	}
	else if (variable == "mustFillCol") {
		cout << "mustFillCol" << endl;
		print(mustFillCol);
	}
	else if (variable == "mustCrossRow") {
		cout << "mustCrossRow" << endl;
		print(mustCrossRow);
	}
	else if (variable == "mustFillRow") {
		cout << "mustFillRow" << endl;
		print(mustFillRow);
	}
	else {
		cout << "no such variable" << endl << endl;
	}
}

void Nanograms::printBitVariable(string variable)
{
	if (variable == "colSize") {
		cout << "colSize" << endl;
		cout << colSize << endl << endl;
	}
	else if (variable == "rowSize") {
		cout << "rowSize" << endl;
		cout << rowSize << endl << endl;
	}
	else if (variable == "colCondition") {
		cout << "colCondition" << endl;
		print(colCondition);
	}
	else if (variable == "rowCondition") {
		cout << "rowCondition" << endl;
		print(rowCondition);
	}
	else if (variable == "allStartCol") {
		cout << "allStartCol" << endl;
		printBit(allStartCol);
	}
	else if (variable == "allStartRow") {
		cout << "allStartRow" << endl;
		printBit(allStartRow);
	}
	else if (variable == "mustCrossCol") {
		cout << "mustCrossCol" << endl;
		printBit(mustCrossCol);
	}
	else if (variable == "mustFillCol") {
		cout << "mustFillCol" << endl;
		printBit(mustFillCol);
	}
	else if (variable == "mustCrossRow") {
		cout << "mustCrossRow" << endl;
		printBit(mustCrossRow);
	}
	else if (variable == "mustFillRow") {
		cout << "mustFillRow" << endl;
		printBit(mustFillRow);
	}
	else {
		cout << "no such variable" << endl << endl;
	}
}

void Nanograms::printBit(vector<long> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": " << bitset<64>(toPrint.at(i)) << endl;
	}
	cout << "end." << endl << endl;
}

void Nanograms::printBit(vector<vector<long>> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": ";
		for (int j = 0; j < toPrint.at(i).size(); j++) {
			cout << bitset<64>(toPrint.at(i).at(j)) << ", ";
		}
		cout << endl;
	}
	cout << "end." << endl << endl;
}

void Nanograms::print(vector<vector<unsigned short int>> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": ";
		for (int j = 0; j < toPrint.at(i).size(); j++) {
			cout << toPrint.at(i).at(j) << ", ";
		}
		cout << endl;
	}
	cout << "end." << endl << endl;
}

void Nanograms::print(vector<vector<long>> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": ";
		for (int j = 0; j < toPrint.at(i).size(); j++) {
			cout << toPrint.at(i).at(j) << ", ";
		}
		cout << endl;
	}
	cout << "end." << endl << endl;
}

void Nanograms::print(vector<long> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": " << toPrint.at(i) << endl;
	}
	cout << "end." << endl << endl;
}

