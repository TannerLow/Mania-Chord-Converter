#include "ChartCreator.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

std::string convertToHitObject(int timeStamp, int columnNumber, int keyCount) {
	// x ,  y , time, type , objParams , hitsounds
	//192, 192, 3664, 1    , 0         , 0:0:0:0:

	int x;
	if (keyCount == 4) {
		x = get4kXValue(columnNumber);
	}
	else if (keyCount == 6) {
		x = get6kXValue(columnNumber);
	}
	else {
		x = get7kXValue(columnNumber);
	}

	std::string hitObject = "";
	hitObject += std::to_string(x);
	hitObject += ",192,";
	hitObject += std::to_string(timeStamp);
	hitObject += ",1,0,0:0:0:0:";
	return hitObject;
}

int get4kXValue(int columnNumber) {
	int x;
	switch (columnNumber) {
	case 0 : x = 64; break;
	case 1 : x = 192; break;
	case 2 : x = 320; break;
	case 3 : x = 448; break;
	default: x = 64; break;
	}
	return x;
}
int get6kXValue(int columnNumber) {
	int x;
	switch (columnNumber) {
	case 0: x = 42; break;
	case 1: x = 128; break;
	case 2: x = 213; break;
	case 3: x = 298; break;
	case 4: x = 384; break;
	case 5: x = 469; break;
	default: x = 42; break;
	}
	return x;
}
int get7kXValue(int columnNumber) {
	int x;
	switch (columnNumber) {
	case 0 : x = 36; break;
	case 1 : x = 109; break;
	case 2 : x = 182; break;
	case 3 : x = 256; break;
	case 4 : x = 329; break;
	case 5 : x = 402; break;
	case 6 : x = 475; break;
	default: x = 36; break;
	}
	return x;
}


std::vector<std::string> createChartsHitObjects(std::set<int> timeStamps, int keyCount, std::vector<int> distributions) {
	std::srand(std::time(0));
	std::vector<std::string> hitObjects;
	//for each timestamp
	for (int time : timeStamps) {
		int noteCount = generateChordSize(distributions, keyCount);
		std::set<int> columns;
		//generate notes at timestamp
		for (int i = 0; i < noteCount; i++) {
			int column = std::rand() % keyCount;
			//regenerate if column already in use
			while (columns.find(column) != columns.end()) {
				column = std::rand() % keyCount;
			}
			columns.insert(column);

			//WORKS WELL FOR LONG JACKS ?
			/*while (columns.find(column) == columns.end()) {
				column = std::rand() % 4;
				columns.insert(column);
			}*/
		}
		//Convert time stamp and columns to hit objects
		for (int column : columns) {
			hitObjects.push_back(convertToHitObject(time, column, keyCount));
		}
		columns.clear();
	}
	return hitObjects;
}

int generateChordSize(std::vector<int>& distributions, int keyCount) {
	int roll = std::rand() % 100 + 1;
	for (int i = 0; i < distributions.size(); i++) {
		if (roll <= distributions[i]) return i + 1;
		roll -= distributions[i];
	}
	return keyCount;
}