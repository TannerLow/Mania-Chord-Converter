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
		std::cout << "Enter diff name: " << std::endl;
		std::cin.ignore();
		std::getline(std::cin, diffname);
	}

	//get key count
	std::vector<std::string> filedata = getLines(filename);
	int keyCount = getKeyMode(filedata);
	
	//prompt for detailed options
	std::cout << "Want in depth options?(y/n): " << std::endl;
	std::cin >> c;
	//set up default distributions
	std::vector<int> distributions;
	for (int i = 0; i < keyCount-1; i++) {
		distributions.push_back(100 / keyCount);
	}
	distributions.push_back(100 - (100 / keyCount * (keyCount-1)));
	//user chose to set options themselves
	if (c != 'n') {

		//set chord size distributions
		std::cout << "Would you like to set the distributions for chord size?(y/n): " << std::endl;
		std::cin >> c;
		if (c != 'n') {
			std::cout << "Enter whole numbers between 0-100.\n"
					  << "They are the percentages that a given chord size wil appear.\n"
					  << "Totals over 100% may result in errors." << std::endl;
			for (int i = 0; i < keyCount; i++) {
				std::cout << "Chords size " << i+1 << ": ";
				int n;
				std::cin >> n;
				if (n > 100 or n < 0) n = 0;
				distributions[i] = n;
			}
		}
	}

	//convert file
	std::vector<std::string> originalHitObjects = grabHitObjects(filename);
	std::set<int> timestamps = getTimeStampSet(originalHitObjects);
	std::vector<std::string> generatedHitObjects = createChartsHitObjects(timestamps, keyCount, distributions);
	std::vector<std::string> convert = convertFile(filename, generatedHitObjects, diffname);

	writeToFile(convert, filename, diffname);

	return 0;
}