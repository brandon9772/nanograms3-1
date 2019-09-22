#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <bitset>
#include <ctime>
#include "Nanograms.h"
#include "Utilities.h"
using namespace std;

int main()
{
	Nanograms nanograms = Nanograms("question1_5x5.txt");
	nanograms.printAllBit();
	Utilities utilities = Utilities();
	utilities.initAllStart(&nanograms);
	nanograms.printAllBit();
	utilities.updateFillCrossByStart(&nanograms);
	nanograms.printAllBit();
	utilities.updateStartByFillCross(&nanograms);
	nanograms.printAllBit();
}

