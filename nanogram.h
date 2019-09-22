#pragma once
#ifndef NANOGRAM_H
#define NANOGRAM_H
#endif
#include <string>
#include <vector>
using namespace std;

class Nanogram {

public:
	unsigned short int colSize;
	unsigned short int rowSize;
	vector<vector<unsigned short int>> colCondition;
	vector<vector<unsigned short int>> rowCondition;
	// a long where 1 is potential start for each condition
	vector<vector<long>> allStartCol;
	vector<vector<long>> allStartRow;
	vector<long> mustCrossCol;
	vector<long> mustFillCol;
	vector<long> mustCrossRow;
	vector<long> mustFillRow;

	Nanogram(string fileName);
	void printAll();
	void printVariable(string variable);
	void print(vector<vector<unsigned short int>> toPrint);
	void print(vector<vector<long>> toPrint);
	void print(vector<long> toPrint);


};