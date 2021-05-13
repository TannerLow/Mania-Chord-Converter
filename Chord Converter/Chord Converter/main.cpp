#include <iostream>
#include "FileHandler.h"
#include "ChartCreator.h"
#include <set>
#include <vector>
#include <string>

int main() {
	//have user select file
	std::string filename = getOsuFile();
	if (filename.empty()) {
		std::cout << "An error occured when selecting the file." << std::endl;
		system("pause");
		return 0;
	}

	//resolve diff name of convert
	std::cout << "Want a custom diff name?(y/n): " << std::endl;
	char c;
	std::cin >> c;
	std::string diffname = "CHORDS";
	if (c != 'n') {
		std::cin.ignore();
		std::getline(std::cin, diffname);
	}

	//convert file
	std::vector<std::string> originalHitObjects = grabHitObjects(filename);
	std::set<int> timestamps = getTimeStampSet(originalHitObjects);
	std::vector<std::string> generatedHitObjects = createChartsHitObjects(timestamps);
	std::vector<std::string> convert = convertFile(filename, generatedHitObjects, diffname);

	writeToFile(convert, filename, diffname);

	return 0;
}