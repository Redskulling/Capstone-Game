#include "types.h"

#include <string>
#include <raylib.h>
#include "../math/Vector2.h"
#include "player.h"
#include "playerstatefunc.h"

#include "enimy.h"

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

	InitWindow(screenWidth, screenHeight, "Title?");	

	// SetTargetFPS(1);

    Camera2D camera = { 0 };
	Player *player = new Player(10.0f, 10.0f, GAMEPAD_PLAYER1, 30.0f);
	player->forwardVel = 120.0f;

	Enimy *enimy = new Enimy({50.0f, 50.0f}, {1, 0, 5, 0}, {25.0f, 25.0f});

	setPlayerState(player, PLAYER_STATE_STATIONARY);

	camera.target = (Vector2){ player->pos.x + 20, player->pos.y + 20 };
	camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	while (!WindowShouldClose()) {

		player->deltaTime = GetFrameTime();
		
		getPlayerInput(player);

		runPlayerState(player);

		camera.target = (Vector2) (player->pos + v2f{ 25.0f, 25.0f }).floor();

		BeginDrawing();
		
        {

			ClearBackground(RAYWHITE);

            BeginMode2D(camera);

			DrawRectangle((int) player->pos.x, (int) player->pos.y, 50, 50, BLACK);
			DrawCircle(player->hitbox.centre.x, player->hitbox.centre.y, player->hitbox.radius, GREEN);

			DrawRectangle(enimy->pos.x, enimy->pos.y, enimy->size.x, enimy->size.y, RED);

			DrawText(TextFormat("TIME: %f", GetFrameTime()), 10, 100, 20, BLACK);

			DrawText(TextFormat("AXIS: %f", player->pos.x),  10, 150, 20, BLACK);
			DrawText(TextFormat("AXIS: %f", player->pos.y),  10, 175, 20, BLACK);
			DrawText(TextFormat("AXIS: %f", player->rightAxis.x), 10, 200, 20, BLACK);
			DrawText(TextFormat("AXIS: %f", player->rightAxis.y), 10, 225, 20, BLACK);
			DrawText(TextFormat("dashTimer: %f", player->dashTimer), 10, 250, 20, BLACK);
			DrawText(TextFormat("state: %i", player->input), 10, 275, 20, BLACK);
     
			
			EndMode2D();

		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}