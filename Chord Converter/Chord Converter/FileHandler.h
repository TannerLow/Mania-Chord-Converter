#pragma once
#include <vector>
#include <string>
#include <set>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

std::vector<std::string> grabHitObjects(const std::string& filename);

std::set<int> getTimeStampSet(std::vector<std::string>& hitObjects);
int getTimeStamp(std::string& hitObject);
int charToInt(char c);

std::vector<std::string> convertFile(const std::string& filename, std::vector<std::string>& hitObjects, const std::string& versionName);
std::string createConvertFileName(const std::string& filename, const std::string& versionName);
std::string changeVersionName(const std::string& versionName);
std::vector<std::string> replaceHitObjects(std::vector<std::string>& originalChart, std::vector<std::string>& hitObjects);

std::string getOsuFile();

void writeToFile(std::vector<std::string>& data, const std::string& filename, const std::string& versionName);

int getKeyMode(std::vector<std::string>& fileData);

std::vector<std::string> getLines(const std::string& filename);