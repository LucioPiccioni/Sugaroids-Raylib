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

Texture2D playerImage = LoadTexture("../res/sprites/player/spaceship.png");
Texture2D sugaroidImage = LoadTexture("../res/sprites/player/spaceship.png");
Texture2D	bulletsImage = LoadTexture("../res/sprites/bullets/star.png");

Menus gameState = Menus::Playing;

int buttonSelected = 0;

void PlayerMovement(Player& player, float deltaTime, int screenWidth, int screenHeight);
void SpawnSugaroid(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids);
void SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets);
void ResetGame(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player);
void DrawCredits(int screenWidth, int screenHeight);
void DrawGamePlay(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player);
void DrawMenu();
void DrawRules();
void DrawGameOverButtons(int buttonSelected);

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

	bool gameOver = false;
	float deltaTime;

	Player player;

	int points = 0;

	float spawnTimer = 0;

	Vector2 mouse;

	InitAudioDevice();

	Music creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	Music  mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Music  gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Music  gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");

	Sound  hurtSound = LoadSound("../res/soundEffects/hurt.wav");
	Sound  boomSound = LoadSound("../res/soundEffects/boom.wav");
	Sound  dieSound = LoadSound("../res/soundEffects/die.wav");
	Sound  shootSound = LoadSound("../res/soundEffects/shoot.wav");

	std::vector<Bullet> bullets;
	std::vector<Sugaroid> sugaroids;

	Texture2D	spaceshipImage = LoadTexture("../res/sprites/player/spaceship.png");
	Texture2D	sugaroidImage = LoadTexture("../res/sprites/enemies/sugaroid.png");
	Texture2D	backgroundImage = LoadTexture("../res/backgrounds/pacific.jfif");

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		deltaTime = GetFrameTime();

		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		/*
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
		else if (gameOver && !IsMusicStreamPlaying(gameOverMusic))
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
		*/

		if (gameState == Menus::Playing)
		{
			PlayerMovement(player, deltaTime, screenWidth, screenHeight);

			mouse = GetMousePosition();

			player.angle = atan2f(mouse.y - player.pos.x, mouse.x - player.pos.y);

			spawnTimer += deltaTime;
			if (spawnTimer > 1)
			{
				SpawnSugaroid(player.pos, sugaroids);
				spawnTimer = 0;
			}

			for (int i = 0; i < sugaroids.size(); i++)
			{
				if (sugaroids[i].toDestroy)
					sugaroids.erase(sugaroids.begin() + i);
				else if (sugaroids[i].position.x + sugaroids[i].radius < 0 || sugaroids[i].position.x - sugaroids[i].radius > screenWidth ||
					sugaroids[i].position.y + sugaroids[i].radius < 0 || sugaroids[i].position.y - sugaroids[i].radius > screenWidth)
				{
					sugaroids.erase(sugaroids.begin() + i);
					points += 5;
				}
			}

			for (int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i].toDestroy)
					bullets.erase(bullets.begin() + i);
				else if (bullets[i].position.x + bullets[i].radius < 0 || bullets[i].position.x - bullets[i].radius > screenWidth ||
					bullets[i].position.y + bullets[i].radius < 0 || bullets[i].position.y - bullets[i].radius > screenWidth)
				{
					bullets.erase(bullets.begin() + i);
				}
			}
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		switch (gameState)
		{
		case Menus::MainMenu:

			DrawMenu();
			break;

		case Menus::Playing:

			if (gameOver)
			{
				DrawText("GAME OVER", screenHeight / 2, screenWidth - TextLength("GAME OVER"), 20, RED);

				DrawGameOverButtons(buttonSelected);

				break;
			}


			DrawGamePlay(bullets, sugaroids, player);

			break;

		case Menus::Rules:

			DrawRules();
			break;

		case Menus::Credits:
			break;
		case Menus::Exit:
			break;
		default:
			break;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	UnloadTexture(playerImage);
	UnloadTexture(sugaroidImage);
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

void PlayerMovement(Player& player, float deltaTime, int screenWidth, int screenHeight)
{
	const float friction = 0.95f;

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		player.targetPos = GetMousePosition();

		Vector2 direction = { player.targetPos.x - player.pos.x, player.targetPos.y - player.pos.y };
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);

		if (length > 0) {
			direction.x /= length;
			direction.y /= length;
		}

		player.speed.x = direction.x * player.velocity;
		player.speed.y = direction.y * player.velocity;
	}

	player.pos.x += player.speed.x * deltaTime;
	player.pos.y += player.speed.y * deltaTime;

	player.speed.x *= friction;
	player.speed.y *= friction;

	float speed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);

	if (speed > player.maxSpeed)
	{
		player.speed.x = (player.speed.x / speed) * player.maxSpeed;
		player.speed.y = (player.speed.y / speed) * player.maxSpeed;
	}

	if (player.pos.x < 0) player.pos.x = screenWidth;
	if (player.pos.x > screenWidth) player.pos.x = 0;
	if (player.pos.y < 0) player.pos.y = screenHeight;
	if (player.pos.y > screenHeight) player.pos.y = 0;
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

void ResetGame(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player)
{
	sugaroids.clear();
	bullets.clear();
}

void DrawCredits(int screenWidth, int screenHeight)
{
	ClearBackground(BLACK);

	DrawText("Credits", screenWidth / 2, screenHeight / 4, 20, BLACK);


	DrawText("Developer: Lucio Stefano Piccioni.", screenWidth / 2, screenHeight / 5, 20, BLACK);


	DrawText("Music:", screenWidth / 2, screenHeight / 2 + 50, 15, BLACK);
	DrawText("1. 'Game Over!' - Harris Cole", screenWidth / 2 + 70, screenHeight / 2 + 70, 10, BLACK);
	DrawText("2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", screenWidth / 2 + 70, screenHeight / 2 + 90, 10, BLACK);
	DrawText("3. 'Quiet Nights' - JEN", screenWidth / 2 + 70, screenHeight / 2 + 110, 10, BLACK);
	DrawText("4. 'Facade' - JEN", screenWidth / 2 + 70, screenHeight / 2 + 130, 10, BLACK);

	DrawText("Sound Effects:", screenWidth / 2 + 70, screenHeight / 2 + 160, 10, BLACK);
	DrawText("  ChipTone", screenWidth / 2 + 70, screenHeight / 2 + 180, 10, BLACK);


	DrawText("Press ESC to go back to the Menu.", screenWidth / 2 + 70, screenHeight / 2 + 210, 10, BLACK);

}

void DrawGamePlay(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player)
{
	for (int i = 0; i < bullets.size(); i++)
	{

		DrawTexturePro(bulletsImage, Rectangle{ 0, 0, (float)bulletsImage.width, (float)bulletsImage.height },  // Fuente (imagen completa)
			Rectangle{ bullets[i].position.x, bullets[i].position.y,  bullets[i].size,  bullets[i].size },  // Destino (posicion y tamaño)
			Vector2{ bulletsImage.width / 2.0f, bulletsImage.height / 2.0f },  // Offset de origen para el centro
			0.0f,
			WHITE);
	}

	DrawTexturePro(playerImage,
		Rectangle{ 0, 0, (float)playerImage.width, (float)playerImage.height },  // Fuente (imagen completa)
		Rectangle{ player.pos.x, player.pos.y, player.width, player.height },  // Destino (posición y tamaño)
		Vector2{ playerImage.width / 2.0f, playerImage.height / 2.0f },  // Offset del centro
		player.angle * RAD2DEG,  // Ángulo en grados (Raylib usa grados, no radianes)
		WHITE);

	for (int i = 0; i < sugaroids.size(); i++) {
		DrawTexturePro(
			sugaroidImage,
			Rectangle{ 0, 0, (float)sugaroidImage.width, (float)sugaroidImage.height },  // Fuente (imagen completa)
			Rectangle{ sugaroids[i].position.x, sugaroids[i].position.y, sugaroids[i].size, sugaroids[i].size },  // Destino (posición y tamaño)
			Vector2{ sugaroidImage.width / 2.0f, sugaroidImage.height / 2.0f },  // Offset del centro
			0.0f,  // Rotación (puedes agregar rotación si es necesario)
			WHITE);
	}
}

void DrawMenu()
{
	Texture2D mainTitle = LoadTexture("res/title.png");
	Texture2D	confirmMenu = LoadTexture("res/sprites/menu/confirm.png");
	Texture2D	menuMove = LoadTexture("res/sprites/menu/menuMove.png");

	// Adaptar el tamaño del título principal
	float scaleX = (float)GetScreenWidth() / mainTitle.width;
	float scaleY = (float)GetScreenHeight() / mainTitle.height;
	float scale = fminf(scaleX, scaleY);

	// Centrar el título
	float drawX = (GetScreenWidth() - (mainTitle.width * scale)) / 2;
	float drawY = (GetScreenHeight() / 2 - (mainTitle.height * scale)) / 2;

	// Dibujar el título
	DrawTextureEx(mainTitle, { drawX, drawY }, 0.0f, scale, WHITE);

	float confirmX = 10;
	float confirmY = GetScreenHeight() - 100 - 10;

	float menuMoveX = confirmX + 100 + 10;
	float menuMoveY = confirmY;

	// Dibujar imágenes de confirmación
	DrawTexturePro(confirmMenu,
		{ 0, 0, (float)confirmMenu.width, (float)confirmMenu.height },
		{ confirmX, confirmY, 100, 100 },
		{ 0, 0 },
		0.0f,
		WHITE);

	DrawTexturePro(menuMove,
		{ 0, 0, (float)menuMove.width, (float)menuMove.height },
		{ menuMoveX, menuMoveY, 100, 100 },
		{ 0, 0 },
		0.0f,
		WHITE);

	// Dibujar botones
	const char* buttons[] = { "Play", "Rules", "Credits", "Exit" };

	for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
	{
		Color buttonColor = (i == buttonSelected) ? Color{ 255, 0, 0, 255 } : Color{ 0, 0, 0, 255 };

		DrawText(buttons[i], GetScreenWidth() / 2 - MeasureText(buttons[i], 20) / 2, GetScreenHeight() / 2 + (i - 2) * 30, 20, buttonColor);
	}

	if (gameState != Menus::MainMenu)
	{
		UnloadTexture(mainTitle);
		UnloadTexture(confirmMenu);
		UnloadTexture(menuMove);
	}
}

void DrawRules()
{
	float titleY = GetScreenHeight(); // Altura de la pantalla
	DrawText("Game Rules", GetScreenWidth() / 2 - MeasureText("Game Rules", 20) / 2, titleY, 20, BLACK);

	// Controles
	float controlsY = GetScreenHeight() / 6 - 80;
	DrawText("Controls:", GetScreenWidth() / 2 - MeasureText("Controls:", 20) / 2, controlsY, 20, BLACK);

	DrawText("Up Arrow: Move Up", GetScreenWidth() / 2 - MeasureText("Up Arrow: Move Up", 20) / 2, controlsY + 40, 20, BLACK);
	DrawText("Down Arrow: Move Down", GetScreenWidth() / 2 - MeasureText("Down Arrow: Move Down", 20) / 2, controlsY + 80, 20, BLACK);
	DrawText("Left Arrow: Move Left", GetScreenWidth() / 2 - MeasureText("Left Arrow: Move Left", 20) / 2, controlsY + 120, 20, BLACK);
	DrawText("Right Arrow: Move Right", GetScreenWidth() / 2 - MeasureText("Right Arrow: Move Right", 20) / 2, controlsY + 160, 20, BLACK);
	DrawText("Left Click: Fire", GetScreenWidth() / 2 - MeasureText("Left Click: Fire", 20) / 2, controlsY + 200, 20, BLACK);
	DrawText("ESC: Pause", GetScreenWidth() / 2 - MeasureText("ESC: Pause", 20) / 2, controlsY + 240, 20, BLACK);

	// Puntos / vidas
	DrawText("Points for destroying sugaroids: 25", GetScreenWidth() / 2 - MeasureText("Points for destroying sugaroids: 25", 20) / 2, controlsY + 320, 20, BLACK);
	DrawText("Points for avoiding sugaroids: 5", GetScreenWidth() / 2 - MeasureText("Points for avoiding sugaroids: 5", 20) / 2, controlsY + 360, 20, BLACK);
	DrawText("You start with 3 lives.", GetScreenWidth() / 2 - MeasureText("You start with 3 lives.", 20) / 2, controlsY + 400, 20, BLACK);
	DrawText("Lose all lives to end the game.", GetScreenWidth() / 2 - MeasureText("Lose all lives to end the game.", 20) / 2, controlsY + 440, 20, BLACK);

	DrawText("Press ESC to return to menu", GetScreenWidth() / 2 - MeasureText("Press ESC to return to menu", 20) / 2, GetScreenHeight() - 60, 20, BLACK);
}

void DrawGameOverButtons(int buttonSelected)
{
	Color textColor = WHITE;

	const char* buttons[] = { "Replay", "Menu", "Exit" };
	int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

	for (int i = 0; i < buttonCount; ++i) {
		// Cambiar el color si el botón está seleccionado
		if (i == buttonSelected) {
			textColor = RED; // Rojo para el botón seleccionado
		}
		else {
			textColor = BLACK; // Negro para los demás
		}

		// Dibujar el botón
		DrawText(buttons[i], GetScreenWidth() / 2 - MeasureText(buttons[i], 20) / 2,
			GetScreenHeight() / 2 + (i * 30), 20, textColor);
	}

	// Resetear el color a blanco
	textColor = WHITE; // Este valor se puede usar si hay más gráficos después
}


