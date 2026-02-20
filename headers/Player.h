#pragma once

#include "resources.h"

#include <vector>

using namespace std;

class Player {

	vector<Vector3> startPositions;
	Vector3 center{0, 1, 0};

	char state;
	float GRIDSIZE;
	Vector3 offsets{};

	Vector3 size{};
	Mesh playerMesh{};
	Model playerModel{};

	vector<Vector3> blockPositions;
	vector<Vector3> goalPositions;

public:
	bool haveWon = false;

	Player(char state, float GRIDSIZE, vector<Vector3> startPositions, vector<Vector3> goalPositions, vector<Vector3> blockPositions) : state(state), GRIDSIZE(GRIDSIZE), startPositions(startPositions), goalPositions(goalPositions), blockPositions(blockPositions) {
		
		UpdateSize();

		offsets = { GRIDSIZE / 2, GRIDSIZE / 2, GRIDSIZE / 2 };

		playerMesh = GenMeshCube(size.x, size.y, size.z);

		playerModel = LoadModelFromMesh(playerMesh);

		CorrectifyStartingPositions();
	}

	void CorrectifyStartingPositions() {
		if (state == 'v') {
			center = Vector3Add(center, startPositions[0]);
			center = Vector3Add(center, { 0, offsets.y, 0 }); // already reposition the center to the start of grid top-left point from center
		}

		if (state == 'h') {
			center = Vector3Add(center, startPositions[0]);
			center = Vector3Add(center, { offsets.x, 0, 0});
		}
	}

	void UpdateSize() {
		if (state == 'v') {
			size = { GRIDSIZE, GRIDSIZE * 2, GRIDSIZE };
		}

		if (state == 'h') {
			size = { GRIDSIZE * 2, GRIDSIZE, GRIDSIZE };
		}

		if (state == 'z') {
			size = { GRIDSIZE, GRIDSIZE, GRIDSIZE * 2 };
		}
	}

	void RebuildModel() {
		UnloadModel(playerModel);

		UpdateSize();
		playerMesh = GenMeshCube(size.x, size.y, size.z);
		playerModel = LoadModelFromMesh(playerMesh);
	}

	vector<Vector3> GetBackPositions() {
		vector<Vector3> positions{};

		if (state == 'h') {
			positions.push_back({ center.x - offsets.x, center.y, center.z });
			positions.push_back({ center.x + offsets.x, center.y, center.z });
		}

		if (state == 'v') {
			positions.push_back({ center.x, center.y - offsets.y, center.z });
		}

		if (state == 'z') {
			positions.push_back({ center.x, center.y, center.z - offsets.z });
			positions.push_back({ center.x, center.y, center.z + offsets.z });
		}

		return positions;
	}

	bool CheckEdges(vector<Vector3> blockPositions) {

		vector<Vector3> positions = GetBackPositions();

		if (state == 'h' || state == 'z') {
			Vector3 cposition1 = { positions[0].x, positions[0].y - 1, positions[0].z }; // matched the same axis for Y
			Vector3 cposition2 = { positions[1].x, positions[1].y - 1, positions[1].z };

			bool part1 = false; bool part2 = false;

			for (Vector3 position : blockPositions) {
				if (Vector3Equals(cposition1, position)) part1 = true;
				if (Vector3Equals(cposition2, position)) part2 = true;
			}

			return part1 && part2;
		}

		if (state == 'v') {
			Vector3 cposition = { positions[0].x, positions[0].y - 1, positions[0].z };

			for (Vector3 position : blockPositions) {
				if (Vector3Equals(cposition, position)) {
					return true;
				}
			}
		}

		return false;
	}

	bool CheckGoal() {
		return CheckEdges(goalPositions);
	}

	void AdjustCenterY() {
		if (state != 'v') {
			center.y = 0;
		}
		
		if (state == 'v') {
			center.y = offsets.y;
		}
	}

	void HandleInputs() {

		// changine axis of the cuboid needs offset to adjust itself from the positioned center to the top-left of the blocks

		if (state == 'h') {
			if (IsKeyPressed(KEY_LEFT)) {
				state = 'v';
				center.x -= GRIDSIZE + offsets.x;
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				state = 'v';
				center.x += GRIDSIZE + offsets.x;
			}

			if (IsKeyPressed(KEY_UP)) {
				state = 'h';
				center.z -= GRIDSIZE;
			}

			if (IsKeyPressed(KEY_DOWN)) {
				state = 'h';
				center.z += GRIDSIZE;
			}
		}

		else if (state == 'v') {

			if (IsKeyPressed(KEY_LEFT)) {
				state = 'h';
				center.x -= GRIDSIZE + offsets.x;
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				state = 'h';
				center.x += GRIDSIZE + offsets.x;
			}

			if (IsKeyPressed(KEY_UP)) {
				state = 'z';
				center.z -= GRIDSIZE + offsets.z;
			}

			if (IsKeyPressed(KEY_DOWN)) {
				state = 'z';
				center.z += GRIDSIZE + offsets.z;
			}
		}

		else if (state == 'z') {
			if (IsKeyPressed(KEY_LEFT)) {
				state = 'z';
				center.x -= GRIDSIZE;
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				state = 'z';
				center.x += GRIDSIZE;
			}

			if (IsKeyPressed(KEY_UP)) {
				state = 'v';
				center.z -= GRIDSIZE + offsets.y;
			}

			if (IsKeyPressed(KEY_DOWN)) {
				state = 'v';
				center.z += GRIDSIZE + offsets.y;
			}
		}

	}

	void PrintBlockPositions() {
		for (Vector3 position : blockPositions) {
			cout << position.x << " " << position.y << " " << position.z << endl;
		}
	}

	void PrintCurrentPosition() {
		cout << center.x << " " << center.y << " " << center.z << endl;
		cout << "state : " << state << endl;
	}

	void PrintDesignerKindPosition() {
		vector<Vector3> positions = GetBackPositions();

		for (Vector3 position : positions) {
			cout << position.x << " " << position.y << " " << position.z << endl;
		}
	}

	void Update() {
		Vector3 oldCenter = center;
		char oldState = state;

		HandleInputs();
		UpdateSize();
		AdjustCenterY();

		if (!CheckEdges(blockPositions)) {
			center = oldCenter;
			state = oldState;
		}
		else {
			RebuildModel();
		}

		if (CheckGoal()) {
			haveWon = true;
		}
	}

	void Draw() const {
		DrawModel(playerModel, center, 1.0f, BLACK);

		DrawCubeWiresV(center, size, WHITE);
	}
};
