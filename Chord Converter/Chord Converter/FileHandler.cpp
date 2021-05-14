#include "FileHandler.h"
#include <fstream>
#include <algorithm>
#include <iostream>

std::vector<std::string> grabHitObjects(const std::string& filename) {

	std::vector<std::string> lines = getLines(filename);

	//remove non-hit objects
	auto it = std::find(lines.begin(), lines.end(), "[HitObjects]");
	if (it != lines.end()) {
		lines.erase(lines.begin(), it + 1);
	}

	return lines;
}

std::set<int> getTimeStampSet(std::vector<std::string>& hitObjects, int bpm) {
	//calculate threshold based on desired bpm, prevents impossible jacks
	int threshold = 60000 / bpm / 4 - 5;

	//compile timestamps
	std::set<int> timeStamps;
	int previousTime = -100000;
	for (auto& hitObject : hitObjects) {
		int time = getTimeStamp(hitObject);
		if (bpm != -1 and time - previousTime < threshold) {
			continue;
		}
		timeStamps.insert(time);
		previousTime = time;

	}
	return timeStamps;
}

int getTimeStamp(std::string& hitObject) {
	int commaCount = 0;
	int timeStamp = 0;
	int i;

	//get rid of x and y pos
	for (i = 0; commaCount < 2; i++) {
		if (hitObject[i] == ',') commaCount++;
	}

	//parse timestamp
	while (hitObject[i] != ',') {
		timeStamp *= 10;
		timeStamp += charToInt(hitObject[i++]);
	}
	return timeStamp;
}

int charToInt(char c) {
	if (c >= '0' and c <= '9') {
		return c - '0';
	}
}

std::vector<std::string> convertFile(const std::string& filename, std::vector<std::string>& hitObjects, const std::string& versionName) {	

	std::vector<std::string> lines = getLines(filename);

	//get line number of metadata tag
	int metadataLine;
	for (metadataLine = 0; metadataLine < lines.size(); metadataLine++) {
		if (lines[metadataLine] == "[Metadata]")
			break;
	}
	metadataLine += 6; //skip to version name
	lines[metadataLine] = changeVersionName(versionName);

	return replaceHitObjects(lines, hitObjects);
}

std::string createConvertFileName(const std::string& filename, const std::string& versionName) {
	std::string convertFileName = filename;
	std::size_t pos = filename.find('[');
	if (pos == std::string::npos) {
		std::cout << "Error, Version not found in filename" << std::endl;
		return ""; //error, return empty string
	}
	convertFileName.erase(convertFileName.begin() + pos + 1, convertFileName.end());
	return convertFileName + versionName + "].osu";
}

std::string changeVersionName(const std::string& versionName) {
	return "Version:" + versionName;
}

std::vector<std::string> replaceHitObjects(std::vector<std::string>& originalChart, std::vector<std::string>& hitObjects) {
	std::vector<std::string> convertLines;
	//copy same original data, excluding hit objects
	for (int i = 0; i < originalChart.size(); i++) {
		convertLines.push_back(originalChart[i]);
		if (originalChart[i] == "[HitObjects]") break;
	}
	
	//copy auto generated hit objects
	for (auto& hitObject : hitObjects) {
		convertLines.push_back(hitObject);
	}
	return convertLines;
}

std::string getOsuFile() {
	char filename[MAX_PATH];

	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "OSU Files\0*.osu\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a chart";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		return std::string(filename);
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
		return std::string(); // empty string invalid file selected or error occured
	}
}

void writeToFile(std::vector<std::string>& data, const std::string& filename, const std::string& versionName) {
	//generate file name of convert
	std::string convertFileName = createConvertFileName(filename, versionName);

	std::ofstream file(convertFileName);
	for (auto line : data) {
		file << line << std::endl;
	}

	file.close();
}

int getKeyMode(std::vector<std::string>& fileData) {
	int difficultyLine;
	for (difficultyLine = 0; difficultyLine < fileData.size(); difficultyLine++) {
		if (fileData[difficultyLine] == "[Difficulty]")
			break;
	}
	difficultyLine += 2; //skip to circle size
	std::string copy = fileData[difficultyLine];

	//parse circle size line for key count
	while (copy[0] < '0' or copy[0] > '9') {
		copy.erase(copy.begin());
	}
	return std::stoi(copy);
}

std::vector<std::string> getLines(const std::string& filename) {
	//get orignal chart data
	std::ifstream file(filename);
	std::vector<std::string> lines;
	std::string buffer;

	//read original chart file
	while (std::getline(file, buffer)) {
		lines.push_back(buffer);
	}

	file.close();
	return lines;
}