#pragma once
#include <string>
#include <vector>
#include <set>

std::string convertToHitObject(int timeStamp, int columnNumber, int keyCount);

int get4kXValue(int columnNumber);
int get7kXValue(int columnNumber);

std::vector<std::string> createChartsHitObjects(std::set<int> timeStamps, int keyCount);


