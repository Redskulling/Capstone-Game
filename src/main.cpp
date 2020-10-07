#include "types.h"

#include <string>
#include <vector>

#define PHYSAC_IMPLEMENTATION
#include "commonincludes.h"

#include "player.h"
#include "playerstatefunc.h"

#include "entity.h"
#include "slime.h"

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

	SetTargetFPS(60);

	std::vector<Entity *> e;

	Camera2D camera = { 0 };
	Player *player = new Player({10.0f, 10.0f}, GAMEPAD_PLAYER1, 30.0f);
	player->forwardVel = 120.0f;

	e.push_back(player);

	// for (int i = 0; i < 2; i++) 
		e.push_back(new Slime({ (f32) GetRandomValue(0, 800),  (f32) GetRandomValue(0, 450) }, e.size()));


	setPlayerState(player, PLAYER_STATE_STATIONARY);

	camera.target = (Vector2){ player->pos.x + 20, player->pos.y + 20 };
	camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	while (!WindowShouldClose()) {

		for (Entity *entities : e)
			entities->deltaTime = GetFrameTime();

		// player->deltaTime = GetFrameTime();
		
		getPlayerInput(player);

		for (int i = 0; i < e.size(); i++)
				e[i]->Update(player, e);

		// runPlayerState(player);

		camera.target = (Vector2) (player->pos + player->size / 2.0f).floor();

		BeginDrawing();
		
		{

			ClearBackground(RAYWHITE);

			BeginMode2D(camera);

			for (int i = 0; i < e.size(); i++)
				(e[i])->Draw();

			// player->Draw();
			DrawCircle(player->hitbox.centre.x, player->hitbox.centre.y, player->hitbox.radius, GREEN);



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

		if (IsWindowResized())
			camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
	}

	CloseWindow();        // Close window and OpenGL context

	return 0;
}

