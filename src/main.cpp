#include <raylib.h>
#include "types.h"

#include "../math/random.h"
#include "../math/mathutils.h"

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
s32 gTimeStop = 1;

int main(void) {

	const int screenWidth = 800;
	const int screenHeight = 450;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth, screenHeight, "Title?");
	HideCursor();

	Texture2D potions = LoadTexture("img/potions.png");
	Texture2D slime   = LoadTexture("img/cube.png");

	SetExitKey(-1);
	// SetTargetFPS(60);

	std::vector<Item *> item;
	item.push_back(new Item("NULL", 0, {-1000, -1000}, 0, &potions));
	// for (int i = 0; i < 5; i++) 
	// 	item.push_back(new Item{"HEAL", randomInt(1, 10), {randomFloat(0, 100), randomFloat(0, 100)}, (s32) item.size(), &potions});

	// for (int i = 0; i < 5; i++)
	// 	item.push_back(new Item{"ATT", randomInt(1, 10), {randomFloat(100, 200), randomFloat(100, 200)}, (s32) item.size(), &potions});

	// for (int i = 0; i < 5; i++)
	// 	item.push_back(new Item{"DEF", randomInt(1, 10), {randomFloat(100, 200), randomFloat(100, 200)}, (s32) item.size(), &potions});

	// for (int i = 0; i < 5; i++)
	// 	item.push_back(new Item{"STAM", randomInt(1, 10), {randomFloat(100, 200), randomFloat(100, 200)}, (s32) item.size(), &potions});

	std::vector<Entity *> entity;

	Camera2D camera = { 0 };
	Texture2D playerTexture = LoadTexture("img/player.png");
	Player *player = new Player({0.0f, 0.0f}, 0, 16.0f, item, &playerTexture);
	player->forwardVel = 120.0f;

	entity.push_back(player);

	player->setState(PLAYER_STATE_STATIONARY);

	camera.target = player->pos + (player->size / 2.0f);
	camera.offset = {screenWidth/2, screenHeight/2};
	camera.rotation = 0.0f;
	camera.zoom = 2.0f;


	// printf("%f\n", randomFloat(10.256f, 140.431f));

	// Camera2D camera2 = { 0 };

	// camera2.target = {110.0f, 110.0f};
	// camera2.offset = { screenWidth-250/2, screenHeight-250/2 };
	// camera2.rotation = 0.0f;
	// camera2.zoom = 1.0f;

	Map *currentMap = new Map("map1.map", "img/tileset.png", entity, slime);

	s32 itemNumber = 0;

	while (!WindowShouldClose()) {

		for (Entity *entities : entity)
			entities->deltaTime = GetFrameTime();

		// player->deltaTime = GetFrameTime();
		
		getPlayerInput(player);

		if (!(gTimeStop &= (TIME_STOP_ALL | TIME_STOP_ENABLED))) {
			for (auto &ent : entity)
				if (ent->id != 0)
					ent->Update(player, entity, currentMap);
		}
		
		if (!(gTimeStop &= TIME_STOP_ALL))
			player->Update(player, entity, currentMap);
			
		if (player->input & INPUT_PAUSE) {
			gTimeStop ^= TIME_STOP_ALL;
			puts("hello");
		}
	

		camera.target = (Vector2) (player->pos + player->size / 2.0f).floor();
		// camera2.target = (Vector2) (e[5]->pos + e[5]->size / 2.0f).floor();


		BeginDrawing();
		
		{

			ClearBackground(BLACK);


			BeginMode2D(camera);

			currentMap->Draw(camera);

			for (auto &ent : entity)
				ent->Draw();

			for (Item *drop : item)
				drop->Draw();

			DrawText(TextFormat("TIME: %X", gTimeStop), 10, 100, 20, BLACK);

			itemNumber += GetMouseWheelMove();
			if (itemNumber >= (s32) player->inventory.slots.size())
				itemNumber = 0;
			if (itemNumber < 0)
				itemNumber = (s32) player->inventory.slots.size() - 1;

			// DrawText(TextFormat("Item #%i: %s, %i", itemNumber, player->inventory.slots[itemNumber].item.name.c_str(), player->inventory.slots[itemNumber].item.count),
			//          player->pos.x, player->pos.y - 55, 10, BLACK);


			// DrawText(TextFormat("Pos: %.2f, %.2f", player->pos.x, player->pos.y), player->pos.x, player->pos.y - 25, 10, BLACK);
			// DrawText(TextFormat("AXIS: %f", player->rightAxis.x), 10, 200, 20, BLACK);
			// DrawText(TextFormat("AXIS: %f", player->rightAxis.y), 10, 225, 20, BLACK);
			// DrawText(TextFormat("dashTimer: %f", player->dashTimer), 10, 250, 20, BLACK);
			// DrawText(TextFormat("state: %i", player->input), 10, 275, 20, BLACK);
			// DrawText(TextFormat("eh: %i", GetGamepadAxisCount(0)), player->pos.x, player->pos.y - 10, 10, BLACK);

			v2f mouseTile = { GetMousePosition().x, GetMousePosition().y };
	 		Tile t = currentMap->getTilePos(mouseTile);
			EndMode2D();

			// BeginMode2D(camera2);
			// BeginScissorMode(GetScreenWidth() - 250, GetScreenHeight() - 250, 250, 250);
			// ClearBackground(LIGHTGRAY);
			// for (int i = 0; i < entity.size(); i++)
			// 	(e[i])->Draw();

			// currentMap->Draw(camera);

			// EndScissorMode();
			// EndMode2D();


		}

		f32 currentHP = map(player->stats.hp, 1, player->stats.maxhp, 10, GetScreenWidth() / 10);

		const char *hptxt = TextFormat("HP: %.2f/%.2f", player->stats.hp, player->stats.maxhp);
		const char *sptxt = TextFormat("Stanima: %.0f/%.0f", player->stamina, player->maxStamina);
		const char *xptxt = TextFormat("XP: %i/%i", player->xp, player->nextLevel);
		if (MeasureText(xptxt, 20) < MeasureText(sptxt, 20))
			DrawRectangle(8, 8, GetScreenWidth() / 10 + 20 + MeasureText(sptxt, 20), 72, GRAY);
		else
			DrawRectangle(8, 8, GetScreenWidth() / 10 + 20 + MeasureText(xptxt, 20), 72, GRAY);

		DrawRectangle(10, 10, currentHP, 20, RED );
		DrawText(hptxt, GetScreenWidth() / 10 + 20, 8, 20, RED);

		f32 currentSP = map(player->stamina, 1, player->maxStamina, 10, GetScreenWidth() / 10);

		DrawRectangle(10, 34, currentSP, 20, GREEN );
		DrawText(sptxt, GetScreenWidth() / 10 + 20, 32, 20, GREEN);

		f32 currentXP = map(player->xp, 1, player->nextLevel, 10, GetScreenWidth() / 10);

		DrawRectangle(10, 58, currentXP, 20, PURPLE );
		DrawText(xptxt, GetScreenWidth() / 10 + 20, 56, 20, PURPLE);	

		DrawRectangle(10, 90, 34, 34, DARKGRAY);
		player->inventory.slots[0].item.Draw();

		DrawRectangle(50, 90, 34, 34, DARKGRAY);
		player->inventory.slots[1].item.Draw();


		if (gTimeStop & (TIME_STOP_ALL))
			player->drawPauseScreen();

		DrawCircle(GetMouseX(), GetMouseY(), 5, GREEN);

		EndDrawing();

		if (IsWindowResized()) {
			camera.offset = { (f32) (GetScreenWidth()/2), (f32) (GetScreenHeight()/2) };
			// camera2.offset = { (f32) (GetScreenWidth()-250/2), (f32) (GetScreenHeight()-250/2)};
		}

		if (IsKeyPressed(KEY_EQUAL))
			camera.zoom += 0.1f;
		if (IsKeyPressed(KEY_MINUS))
			camera.zoom -= 0.1f;
	}

	for (auto &a : entity)
		delete a;
	entity.clear();

	for (auto &a : item)
		delete a;
	item.clear();

	UnloadTexture(slime);
	UnloadTexture(potions);

	CloseWindow();        // Close window and OpenGL context

	return 0;
}

