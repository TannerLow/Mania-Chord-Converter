#include "ChartCreator.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

std::string convertToHitObject(int timeStamp, int columnNumber) {
	// x ,  y , time, type , objParams , hitsounds
	//192, 192, 3664, 1    , 0         , 0:0:0:0:

	int x;
	switch (columnNumber) {
		case 0 : x = 64; break;
		case 1 : x = 192; break;
		case 2 : x = 320; break;
		case 3 : x = 448; break;
		default : x = 64; break;
	}

	std::string hitObject = "";
	hitObject += std::to_string(x);
	hitObject += ",192,";
	hitObject += std::to_string(timeStamp);
	hitObject += ",1,0,0:0:0:0:";
	return hitObject;
}

std::vector<std::string> createChartsHitObjects(std::set<int> timeStamps) {
	std::srand(std::time(0));
	std::vector<std::string> hitObjects;
	//for each timestamp
	for (int time : timeStamps) {
		int noteCount = std::rand() % 4 + 1;
		std::set<int> columns;
		//generate notes at timestamp
		for (int i = 0; i < noteCount; i++) {
			int column = std::rand() % 4;
			//regenerate if column already in use
			while (columns.find(column) == columns.end()) {
				column = std::rand() % 4;
				columns.insert(column);
			}
		}
		//Convert time stamp and columns to hit objects
		for (int column : columns) {
			hitObjects.push_back(convertToHitObject(time, column));
		}
		columns.clear();
	}
	return hitObjects;
}