#include "headers/resources.h"
#include "headers/Parser.h"
#include "headers/Designer.h"
#include "headers/Player.h"

#include <optional>

using namespace std;

class Game {
public:

	Camera3D camera{};
	std::optional <LevelParser> Parser;
	std::optional <Designer> designer;
	std::optional <Player> player;

	int currLevel = 0;
	bool levelChanged = false;

	Game() {
		InitWindow(WIDTH, HEIGHT, "Cuboid");

		camera.position = {0, 5, 5};
		camera.up = {0, 1, 0};
		camera.target = {0, 0, 0};
		camera.fovy = 60;
		camera.projection = CAMERA_PERSPECTIVE;

		Parser.emplace(currLevel);

		ReInitiate();
	}

	void ReInitiate() {
		char state = Parser->GetState();

		designer.emplace(camera, Parser->SetLevelRowsCols(), state);
		player.emplace(state, 1.0, designer->GetStartingPositions(state), designer->GetGoalPositions(), designer->GetBlockPositions());
	}

	void ReChange(bool required) {
		if (required) {
			Parser.emplace(currLevel);

			ReInitiate();

			levelChanged = false;
			player->haveWon = false;
		}
	}

	void Update(float dt) {

		designer->Update();
		player->Update();

		if (player->haveWon) {
			currLevel++;
			levelChanged = true;
		}

		ReChange(levelChanged);
	}

	void Draw() const {
		BeginDrawing();

		BeginMode3D(camera);
		ClearBackground(WHITE);

		designer->Draw();
		player->Draw();

		EndMode3D();
		EndDrawing();
	}

	~Game() {
		CloseWindow();
	}
};

int main() {
	Game game{};

	float dt;

	while (!WindowShouldClose()) {
		dt = GetFrameTime();

		game.Update(dt);

		game.Draw();
	}

	return 0;
}