#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

inline const std::string LEVELPATH = "data/";

using namespace std;

std::string ReadFileData(const std::string& filepath) {
	std::string data;

	std::ifstream file(filepath);
	std::string line;

	if (!file.is_open()) {
		std::cout << "failed to open" << std::endl;
		return "";
	}

	while (std::getline(file, line)) {
		data += line + "\n";
	}

	return data;
}

std::string ReadLevel(int curlevel) {
	std::string modifiedPath = LEVELPATH + std::to_string(curlevel) + ".txt";

	return ReadFileData(modifiedPath);
}

class LevelParser {

private:
	char state;
	std::string data;
	std::string levelDesignData;
	unordered_map<int, vector<char>> levelRowsCols;

public:
	int curLevel, rows, cols;

	LevelParser(int curLevel) : curLevel(curLevel){
		data = ReadLevel(curLevel);
		state = GetState();
		levelDesignData = GetLevelDesign();
		levelRowsCols = SetLevelRowsCols();
	}

	char GetState() {
		std::stringstream ss(data);

		for (std::string line; std::getline(ss, line); ) {
			if (line.find("@state") != std::string::npos) {
				return line.back();
			}
		}

		return 'v';
	}

	std::string GetLevelDesign() {
		std::stringstream ss(data);
		bool afterData = false;
		std::string levelDesignData;

		for (std::string line; std::getline(ss, line); ) {
			if (line.find("@data") != std::string::npos) {
				afterData = true;
				continue;
			}

			if (line.find("@end") != std::string::npos) {
				return levelDesignData;
			}

			if (afterData) {
				levelDesignData += line + "\n";
			}
		}

		return levelDesignData;
	}

	unordered_map<int, vector<char>> SetLevelRowsCols() {
		unordered_map<int, vector<char>> levelRowsCols;

		if (levelDesignData.empty()) return{};

		std::stringstream ss(levelDesignData);

		int lineCount = 0;
		for (std::string line; std::getline(ss, line);) {

			std::stringstream cc(line);
			vector<char> lineChars;

			for (char ch; cc >> ch;) {
				lineChars.push_back(ch);
			}

			if (!lineChars.empty() && !levelRowsCols.contains(lineCount + 1)) {
				levelRowsCols[lineCount + 1] = lineChars;
			}
			
			lineCount++;
		}

		return levelRowsCols;
	}

	void PrintLevelDesign() {
		for (const auto& map : levelRowsCols) {
			cout << map.first << " : ";

			for (char ch : map.second) {
				cout << ch;
			}

			cout << endl;
		}
	}
};