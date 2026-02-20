#pragma once

#include "resources.h"

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class Designer {
	Camera3D camera{};
	unordered_map<int, vector<char>> levelDesign;

	vector<Vector3> blockPositions;
	float GRIDSIZE = 1.0;

	Mesh blockMesh = GenMeshCube(GRIDSIZE, GRIDSIZE, GRIDSIZE);
	Model blockModel = LoadModelFromMesh(blockMesh);

	vector<Vector3> goalPositions{};
	Vector3 startPosition{}, startPositionDir{};

public:
	int rows, cols;
	Vector3 center = {0, -1, 0};

	Designer(Camera3D camera, unordered_map<int, vector<char>> levelDesign, char state) : camera(camera), levelDesign(levelDesign) {
		rows = levelDesign.size();
		cols = levelDesign[1].size();

		CalculatePositions(state);

		//PrintBlockPositions();
	}

	void CalculatePositions(char state) {

		int r_center = ceil((float)rows / 2);
		int c_center = ceil((float)cols / 2);

		for (int i = 1; i < rows + 1; i ++) {
			for (int j = 1; j < cols + 1; j ++) {
				Vector3 tempPosition = { center.x - c_center + j, center.y, center.z - r_center + i };

				if (levelDesign[i][j - 1] != '0') {
					blockPositions.push_back(tempPosition);
				}

				if (levelDesign[i][j - 1] == 'e') {
					goalPositions.push_back(tempPosition);
				}
			}
		}

		Vector2 startCoordinate = GetSpecialBlocksPositions('s');
		Vector2 goalCoordinate = GetSpecialBlocksPositions('e');

		startPosition = {center.x - c_center + startCoordinate.y, center.y, center.z - r_center + startCoordinate.x};

		if (state == 'h') {
			Vector2 nextStartCoordinate = NearBlocks(startCoordinate);

			startPositionDir = { center.x - c_center + nextStartCoordinate.y, center.y, center.z - r_center + nextStartCoordinate.x };
		}
	}

	vector<Vector3> GetBlockPositions() {
		return blockPositions;
	}

	vector<Vector3> GetStartingPositions(char state) {
		vector<Vector3> positions;

		if (state == 'h') {

			// right
			if (Vector3Equals(Vector3Add({1, 0, 0}, startPosition), startPositionDir)) {
				positions.push_back(startPosition);
				positions.push_back(startPositionDir);
			}

			// left
			if (Vector3Equals(Vector3Add({ -1, 0, 0 }, startPosition), startPositionDir)) {
				positions.push_back(startPositionDir);
				positions.push_back(startPosition);
			}
		}

		if (state == 'v') {
			positions.push_back(startPosition);
		}

		return positions;
		
	}

	vector<Vector3> GetGoalPositions() {
		return goalPositions;
	}

	Vector2 NearBlocks(Vector2 keyPosition) {
		char left{}, mid{}, right{};
		bool keyFound = false;

		int chIdx = 0;
		for (char ch : levelDesign[keyPosition.x]) {
			
			left = mid;
			mid = ch;

			if (chIdx + 1 <= levelDesign[keyPosition.x].size() - 1) {
				right = levelDesign[keyPosition.x][chIdx + 1];
			}
			else {
				right = '0';
			}

			if (mid == 's') {
				keyFound = true;
				break;
			}

			chIdx++;
		}

		if (keyFound) {
			if (left == '1' && right == '1') {
				return { keyPosition.x, keyPosition.y + 1 };
			}

			if (left == '1') {
				return { keyPosition.x, keyPosition.y - 1 };
			}

			if (right == '1') {
				return { keyPosition.x, keyPosition.y + 1 };
			}
		}
		
		return {};
	}

	Vector2 GetSpecialBlocksPositions(char key) {
		for (const auto& map : levelDesign) {

			int chIdx = 1;
			for (char ch : map.second) {
				if (ch == key) {
					return {(float)map.first, (float)chIdx};
				}
				chIdx++;
			}
		}

		return {};
	}

	void PrintBlockPositions() {
		for (Vector3 pos : blockPositions) {
			cout << " x : " << pos.x << " y : " << pos.y << " z : " << pos.z << endl;
		}

		cout << "Start Position : " << startPosition.x << " " << startPosition.y << " " << startPosition.z << endl;
		cout << "Next Start Position : " << startPositionDir.x << " " << startPositionDir.y << " " << startPositionDir.z << endl;

		cout << "Goal Position : " << endl;
		for (Vector3 pos : goalPositions) {
			cout << pos.x << " " << pos.y << " " << pos.z << endl;
		}
	}

	void Update() {
	}

	void Draw() const {
		//DrawGrid(10, GRIDSIZE);

		float wireScale = GRIDSIZE * 1.01;

		for (Vector3 position : blockPositions) {

			if (Vector3Equals(position, startPosition) || Vector3Equals(position, startPositionDir)) {
				DrawModel(blockModel, position, 1.0, YELLOW);
			}

			else {
				DrawModel(blockModel, position, 1.0, GREEN);
			}

			for (Vector3 gposition : goalPositions) {
				if (Vector3Equals(position, gposition)) {
					DrawModel(blockModel, position, 1.0, RED);
				}
			}

			DrawCubeWiresV(position, {wireScale, wireScale, wireScale}, RED);
		}
	}
};
