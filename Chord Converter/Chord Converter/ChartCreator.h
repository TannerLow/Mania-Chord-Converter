#pragma once
#include <string>
#include <vector>
#include <set>

std::string convertToHitObject(int timeStamp, int columnNumber);

std::vector<std::string> createChartsHitObjects(std::set<int> timeStamps);


