#include "types.h"

#include <time.h>

u32 lehmerSeed = time(NULL);

#include "../math/random.h"

#include <string>
#include <vector>

#include "commonincludes.h"

#include "entity/player.h"

#include "entity/entity.h"
#include "entity/slime.h"

#include "map/map.h"

#include "item/item.h"

typedef Color Colour;

#if defined(PLATFORM_RPI)
	#define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
	#define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
	#define XBOX360_NAME_ID     "Xbox 360 Controller"
	#define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif


#define MAX_BUILDINGS 100

int main(void) {

	const int screenWidth = 800;
	const int screenHeight = 450;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth, screenHeight, "Title?");	

	SetExitKey(-1);	
	SetTargetFPS(60);

	std::vector<Item *> item;
	for (int i = 0; i < 20; i++) 
		item.push_back(new Item{"TEST", randomInt(1, 10), {randomFloat(0, 100), randomFloat(0, 100)}, (s32) item.size()});

	for (int i = 0; i < 5; i++)
		item.push_back(new Item{"TEST2", randomInt(1, 10), {randomFloat(100, 200), randomFloat(100, 200)}, (s32) item.size()});

	std::vector<Entity *> e;

	Camera2D camera = { 0 };
	Player *player = new Player({0.0f, 0.0f}, GAMEPAD_PLAYER1, 30.0f, item);
	player->forwardVel = 120.0f;

	e.push_back(player);

	for (int i = 0; i < 25; i++) 
		e.push_back(new Slime({ (f32) GetRandomValue(0, 800),  (f32) GetRandomValue(0, 450) }, e.size()));

	player->setState(PLAYER_STATE_STATIONARY);

	camera.target = { player->pos.x + 20, player->pos.y + 20 };
	camera.offset = {screenWidth/2, screenHeight/2};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;


	printf("%f\n", randomFloat(10.256f, 140.431f));

	// Camera2D camera2 = { 0 };

	// camera2.target = e[5]->pos + e[5]->size / 2.0f;
	// camera2.offset = { screenWidth-250/2, screenHeight-250/2 };
	// camera2.rotation = 0.0f;
	// camera2.zoom = 1.0f;

	Map *currentMap = new Map(50.0f, {5, 10});

	for (int j = 0; j < currentMap->size.y; j++) {
		for (int i = 0; i < currentMap->size.x; i++) {
			printf("%X", currentMap->getTile({i, j}).type);
		}
		puts(" ");
	}

	s32 itemNumber = 0;

	while (!WindowShouldClose()) {

		for (Entity *entities : e)
			entities->deltaTime = GetFrameTime();

		// player->deltaTime = GetFrameTime();
		
		getPlayerInput(player);

		for (int i = 0; i < e.size(); i++)
				e[i]->Update(player, e, currentMap);

		// runPlayerState(player);

		camera.target = (Vector2) (player->pos + player->size / 2.0f).floor();
		// camera2.target = (Vector2) (e[5]->pos + e[5]->size / 2.0f).floor();


		BeginDrawing();
		
		{

			ClearBackground(RAYWHITE);

			BeginMode2D(camera);

			for (int i = 0; i < e.size(); i++)
				(e[i])->Draw();

			for (Item *drop : item)
				drop->Draw();

			DrawCircle(player->hitbox.centre.x, player->hitbox.centre.y, player->hitbox.radius, GREEN);

			currentMap->Draw();

			DrawText(TextFormat("TIME: %f", GetFrameTime()), 10, 100, 20, BLACK);

			itemNumber += GetMouseWheelMove();
			if (itemNumber >= (s32) player->inventory.slots.size())
				itemNumber = 0;
			if (itemNumber < 0)
				itemNumber = (s32) player->inventory.slots.size() - 1;

			DrawText(TextFormat("Item #%i: %s, %i", itemNumber, player->inventory.slots[itemNumber].item.name.c_str(), player->inventory.slots[itemNumber].item.count),
			         player->pos.x, player->pos.y - 55, 10, BLACK);


			DrawText(TextFormat("Pos: %.2f, %.2f", player->pos.x, player->pos.y), player->pos.x, player->pos.y - 25, 10, BLACK);
			DrawText(TextFormat("AXIS: %f", player->rightAxis.x), 10, 200, 20, BLACK);
			DrawText(TextFormat("AXIS: %f", player->rightAxis.y), 10, 225, 20, BLACK);
			DrawText(TextFormat("dashTimer: %f", player->dashTimer), 10, 250, 20, BLACK);
			DrawText(TextFormat("state: %i", player->input), 10, 275, 20, BLACK);
			DrawText(TextFormat("eh: %X", player->map->getTilePos(player->pos).type), player->pos.x, player->pos.y - 10, 10, BLACK);
	 
			v2f mouseTile = { GetMousePosition().x, GetMousePosition().y };
	 		Tile t = currentMap->getTilePos(mouseTile);
			EndMode2D();

			// BeginMode2D(camera2);
			// BeginScissorMode(GetScreenWidth() - 250, GetScreenHeight() - 250, 250, 250);
			// ClearBackground(LIGHTGRAY);
			// for (int i = 0; i < e.size(); i++)
			// 	(e[i])->Draw();

			// EndScissorMode();
			// EndMode2D();

		}

		EndDrawing();

		if (IsWindowResized()) {
			camera.offset = { (f32) (GetScreenWidth()/2), (f32) (GetScreenHeight()/2) };
			// camera2.offset = { (f32) (GetScreenWidth()-250/2), (f32) (GetScreenHeight()-250/2)};
		}
	}

	for (std::vector<Entity *>::reverse_iterator i = e.rbegin(); i != e.rend(); i++) {
		delete (*i);
		e.pop_back();
	}

	delete currentMap;

	CloseWindow();        // Close window and OpenGL context

	return 0;
}

