#include "nanogram.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

Nanogram::Nanogram(string fileName) {
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
	long crossDefaultCol = allBitOne << colSize;
	long crossDefaultRow = allBitOne << rowSize;

	for (int i = 0; i < colSize; i++) {
		getline(myfile, line);
		istringstream entry(line);
		vector<unsigned short int> thisCol;
		vector<long> thisStart;

		for (unsigned short int condition; entry >> condition;) {
			thisCol.push_back(condition);
			thisStart.push_back(allBitOne);
		}
		allStartCol.push_back(thisStart);
		mustCrossCol.push_back(crossDefaultCol);
		mustCrossCol.push_back(crossDefaultCol);
		mustFillCol.push_back(~allBitOne);
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
			thisStart.push_back(allBitOne);
		}
		allStartRow.push_back(thisStart);
		mustCrossRow.push_back(crossDefaultRow);
		mustCrossRow.push_back(crossDefaultRow);
		mustFillRow.push_back(~allBitOne);
		mustFillRow.push_back(~allBitOne);
		rowCondition.push_back(thisRow);

	}
	myfile.close();
	reverse(colCondition.begin(), colCondition.end());
	reverse(rowCondition.begin(), rowCondition.end());
}

void Nanogram::printAll()
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

void Nanogram::printVariable(string variable)
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

void Nanogram::print(vector<vector<unsigned short int>> toPrint)
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

void Nanogram::print(vector<vector<long>> toPrint)
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

void Nanogram::print(vector<long> toPrint)
{
	int size = toPrint.size();
	for (int i = 0; i < size; i++) {
		cout << i << ": " << toPrint.at(i) << endl;
	}
	cout << "end." << endl << endl;
}

