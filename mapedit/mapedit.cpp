// #define MAP_DEBUG
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include "../src/types.h"
#include <cmath>

#include "../src/map/map.h"


int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 432;

	const char *file = "test.map";

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth, screenHeight, "Title?");	

	SetExitKey(-1);

	v2u8 cTile = { 0, 0 };


	Map *currentMap = new Map(32.0f, {50, 50}, "img/tileset.png");

	Rectangle player = /*new*/ Rectangle{ currentMap->size.x * currentMap->tileSize / 2, currentMap->size.y * currentMap->tileSize / 2, 32, 32 };
	Camera2D camera = { 0 };

	camera.target = { player.x + 20, player.y + 20 };
	camera.offset = { screenWidth/2, screenHeight/2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	u8 type = 1;

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_DOWN) || (IsKeyPressed(KEY_S) && !IsKeyDown(KEY_LEFT_CONTROL)))
			player.y += 32;

		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
			player.y -= 32;

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
			player.x -= 32;

		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
			player.x += 32;

		if (player.x < 0)
			player.x = 0;

		if (player.x > currentMap->size.x * 32 - 32)
			player.x = currentMap->size.x * 32 - 32;

		if (player.y > currentMap->size.y * 32 - 32)
			player.y = currentMap->size.y * 32 - 32;

		if (player.y < 0)
			player.y = 0;

		camera.target = (Vector2) { player.x + 25.0f, player.y + 25.0f };

		if (IsKeyPressed(KEY_ONE)) { // WALL
			type &= (0x40 | 0x1);
			type ^= 0x1;
		}
		if (IsKeyPressed(KEY_TWO)) { // HOLE
			type &= (0x40 | 0x2);
			type ^= 0x2;
		}
		if (IsKeyPressed(KEY_THREE)) { // STAIR
			type &= (0x10 | 0x20 | 0x4);
			type ^= 0x4;
		}
		if (IsKeyPressed(KEY_FOUR)) { // PLAYER
			type &= (0x40 | 0x8);
			type ^= 0x8;
		}
		if (IsKeyPressed(KEY_FIVE)) { // SLIME
			type &= (0x40 | 0x10);
			type ^= 0x10;
		}
		if (IsKeyPressed(KEY_SIX)) { // VOID
			type &= (0x40 | 0x20);
			type ^= 0x20;
		}
		if (IsKeyPressed(KEY_SEVEN)) { // HIDDEN STAIR
			type &= (0x1 | 0x2 | 0x8 | 0x10 | 0x20 | 0x40);
			type ^= 0x40;
		}

		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			if (IsKeyPressed(KEY_S))
				currentMap->WriteToFile(file);
			if (IsKeyPressed(KEY_O))
				currentMap->ReadFromFile(file);
		}

		if (GetMouseWheelMove() > 0) {
			camera.zoom += 0.1f * (camera.zoom == 0 ? 1.0f : camera.zoom); 
		} else if (GetMouseWheelMove() < 0) {
			camera.zoom -= 0.1f * (camera.zoom == 0 ? 1.0f : camera.zoom);
		}

		v2f mouseTile = GetScreenToWorld2D(GetMousePosition(), camera);

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT)) {
			currentMap->getTilePos(mouseTile).type |= type;
			if (!type)
				currentMap->getTilePos(mouseTile).tilePos = cTile;
		}
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			cTile = (v2u8)((v2i) GetMousePosition() / 64);

		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT)) 
			currentMap->getTilePos(mouseTile).type &= ~(type);

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT))
			currentMap->getTilePos(mouseTile).tilePos = cTile;

		BeginDrawing();
		
		{
			ClearBackground(RAYWHITE);

			if (!IsKeyDown(KEY_LEFT_SHIFT)) {
				BeginMode2D(camera);
				currentMap->Draw(camera);
				DrawRectangleLines(player.x, player.y, player.width, player.height, BLACK);

				DrawText(TextFormat("%X", type), player.x + 20, player.y + 20, 15/camera.zoom, GRAY);
				EndMode2D();
			} else {
				DrawTextureEx(currentMap->tileSet, {0, 0}, 0.0f, 2.0f, WHITE);
			}

		}

		EndDrawing();
		if (IsWindowResized())
			camera.offset = { (f32) (GetScreenWidth()/2), (f32) (GetScreenHeight()/2) };

	}

	delete currentMap;

	CloseWindow();
}
