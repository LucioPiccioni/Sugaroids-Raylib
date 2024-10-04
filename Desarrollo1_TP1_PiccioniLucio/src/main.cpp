#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Sugaroids");

	SetTargetFPS(144);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	InitAudioDevice();

	Texture2D mainTitle = LoadTexture("res/title.png");

	Texture2D playerImage = LoadTexture("res/sprites/player/spaceship.png");
	Texture2D sugaroidImage = LoadTexture("res/sprites/player/spaceship.png");

	Sound  creditsMusic = LoadSound("res/music/Game Over! - Harris Cole.mp3");
	Sound  mainMenuMusic = LoadSound("res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Sound  gameOverMusic = LoadSound("res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Sound  gamePlayMusic = LoadSound("res/music/JEN - FACADE - soulmate.mp3");
	Sound  hurtSound = LoadSound("res/soundEffects/hurt.wav");
	Sound  boomSound = LoadSound("res/soundEffects/boom.wav");
	Sound  dieSound = LoadSound("res/soundEffects/die.wav");
	Sound  shootSound = LoadSound("res/soundEffects/shoot.wav");

	Texture2D	confirmMenu = LoadTexture("res/sprites/menu/confirm.png");
	Texture2D	menuMove = LoadTexture("res/sprites/menu/menuMove.png");
	Texture2D	spaceshipImage = LoadTexture("res/sprites/player/spaceship.png");
	Texture2D	sugaroidImage = LoadTexture("res/sprites/enemies/sugaroid.png");
	Texture2D	backgroundImage = LoadTexture("res/backgrounds/pacific.jfif");
	Texture2D	bulletsImage = LoadTexture("res/sprites/bullets/star.png");

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	UnloadTexture(mainTitle);
	UnloadTexture(playerImage);
	UnloadTexture(sugaroidImage);
	UnloadTexture(confirmMenu);
	UnloadTexture(menuMove);
	UnloadTexture(spaceshipImage);
	UnloadTexture(sugaroidImage);
	UnloadTexture(backgroundImage);
	UnloadTexture(bulletsImage);

	UnloadSound(creditsMusic);
	UnloadSound(mainMenuMusic);
	UnloadSound(gameOverMusic);
	UnloadSound(gamePlayMusic);
	UnloadSound(hurtSound);
	UnloadSound(boomSound);
	UnloadSound(dieSound);
	UnloadSound(shootSound);

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseAudioDevice();
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}