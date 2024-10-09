#include <cmath>
#include <ctime>
#include "eventManager.h"
#include "player.h"
#include "sugaroid.h"
#include "Bullets.h"
#include "raylib.h"
#include <vector>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	srand(time(NULL));

	InitWindow(screenWidth, screenHeight, "Sugaroids");

	SetTargetFPS(144);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	Menus gameState = Menus::MainMenu;
	bool gameOver = false;
	float deltaTime;

	Player player;


	float spawnTimer = 0;

	Vector2 mouse;

	InitAudioDevice();

	Texture2D mainTitle = LoadTexture("res/title.png");

	Texture2D playerImage = LoadTexture("res/sprites/player/spaceship.png");
	Texture2D sugaroidImage = LoadTexture("res/sprites/player/spaceship.png");

	Music creditsMusic = LoadMusicStream("res/music/Game Over! - Harris Cole.mp3");
	Music  mainMenuMusic = LoadMusicStream("res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Music  gameOverMusic = LoadMusicStream("res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Music  gamePlayMusic = LoadMusicStream("res/music/JEN - FACADE - soulmate.mp3");

	Sound  hurtSound = LoadSound("res/soundEffects/hurt.wav");
	Sound  boomSound = LoadSound("res/soundEffects/boom.wav");
	Sound  dieSound = LoadSound("res/soundEffects/die.wav");
	Sound  shootSound = LoadSound("res/soundEffects/shoot.wav");

	std::vector<Bullet> bullets;
	std::vector<Sugaroid> sugaroids;

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
		
		deltaTime = GetFrameTime();
	
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		if (gameState == Menus::MainMenu && !IsMusicStreamPlaying(mainMenuMusic))
		{
			StopMusicStream(gamePlayMusic);
			StopMusicStream(gameOverMusic);
			StopMusicStream(creditsMusic);

			PlayMusicStream(mainMenuMusic);
		}
		else if (gameState == Menus::Credits && !IsMusicStreamPlaying(creditsMusic))
		{
			StopMusicStream(gamePlayMusic);
			StopMusicStream(gameOverMusic);
			StopMusicStream(mainMenuMusic);

			StopMusicStream(creditsMusic);
		}
		else if (gameOver && !IsMusicStreamPlaying(gameOverMusic) && !IsSoundPlaying(dieSound))
		{
			StopMusicStream(creditsMusic);
			StopMusicStream(gamePlayMusic);
			StopMusicStream(mainMenuMusic);

			StopMusicStream(gameOverMusic);
		}
		else if (!gameOver && gameState == Menus::Playing && !IsMusicStreamPlaying(gamePlayMusic))
		{
			StopMusicStream(creditsMusic);
			StopMusicStream(gameOverMusic);
			StopMusicStream(mainMenuMusic);

			StopMusicStream(gamePlayMusic);
		}

		if (gameState == Menus::Playing)
		{
			if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
			{
				if (player.pos.x < screenWidth)
					player.pos.x += player.speed * deltaTime;
			}
			else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
			{
				if (player.pos.x > 0)
					player.pos.x -= player.speed * deltaTime;
			}

			if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
			{
				if (player.pos.y < screenHeight)
					player.pos.y += player.speed * deltaTime;
			}
			else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
			{
				if (player.pos.y > 0)
					player.pos.y -= player.speed * deltaTime;
			}

			mouse = GetMousePosition();

			player.angle = atan2f(mouse.y - player.pos.x, mouse.x - player.pos.y);

			spawnTimer += deltaTime;
			if (spawnTimer > 1)
			{
				SpawnSugaroid(player.pos, sugaroids);
				spawnTimer = 0;
			}
		}

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

	UnloadMusicStream(creditsMusic);
	UnloadMusicStream(mainMenuMusic);
	UnloadMusicStream(gameOverMusic);
	UnloadMusicStream(gamePlayMusic);
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

void SpawnSugaroid(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids)
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	// Randomly pick an edge (1: top, 2: bottom, 3: left, 4: right)
	int edge = rand() % 4 + 1;
	Vector2 spawnPosition = { 0, 0 };

	switch (edge) 
	{
	case 1:  // Top edge
		spawnPosition.x = rand() % screenWidth;
		spawnPosition.y = 0;
		break;
	case 2:  // Bottom edge
		spawnPosition.x = rand() % screenWidth;
		spawnPosition.y = screenHeight;
		break;
	case 3:  // Left edge
		spawnPosition.x = 0;
		spawnPosition.y = rand() % screenHeight;
		break;
	case 4:  // Right edge
		spawnPosition.x = screenWidth;
		spawnPosition.y = rand() % screenHeight;
		break;
	}

	// Random Sugaroid size and speed
	float size = rand() % 33 + 32;  // Random size between 32 and 64
	float speed = rand() % 101 + 100;  // Random speed between 100 and 200

	// Calculate direction towards the player
	Vector2 direction = { playerPosition.x - spawnPosition.x, playerPosition.y - spawnPosition.y };
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	Vector2 velocity = { 0.0f, 0.0f };
	if (length > 0) {
		velocity.x = (direction.x / length) * speed;
		velocity.y = (direction.y / length) * speed;
	}

	// Create the new Sugaroid and add it to the vector
	Sugaroid newSugaroid;
	newSugaroid.position = spawnPosition;
	newSugaroid.velocity = velocity;
	newSugaroid.radius = size / 2.0f;
	newSugaroid.toDestroy = false;

	sugaroids.push_back(newSugaroid);
}

void SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets)
{
	float size = 32.0f;
	float radius = 16.0f;

	float directionX = cos(playerAngle);
	float directionY = sin(playerAngle);
	float length = sqrt(directionX * directionX + directionY * directionY);


	float speed = 500.0f;

	Vector2 velocity = { 0.0f, 0.0f };
	if (length > 0) {
		velocity.x = (directionX / length) * speed;
		velocity.y = (directionY / length) * speed;
	}

	Bullet newBullet;
	newBullet.position = playerPosition;  // Start at the player's position
	newBullet.velocity = velocity;
	newBullet.radius = radius;
	newBullet.toDestroy = false;

	bullets.push_back(newBullet);
}