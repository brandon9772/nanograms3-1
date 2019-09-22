#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <bitset>
#include <ctime>
#include "nanogram.h"
using namespace std;

int main()
{
	Nanogram nanogram = Nanogram("..question/question1_5x5.txt");
	nanogram.printAll();

}

