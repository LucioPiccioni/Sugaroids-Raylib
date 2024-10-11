#include <cmath>
#include <ctime>
#include "eventManager.h"
#include "player.h"
#include "sugaroid.h"
#include "Bullets.h"
#include "raylib.h"
#include <vector>
#include <iostream>

enum class MenuOption
{
	//Main Menu
	MainMenu,
	Play,
	Options,
	Credits,
	Exit,
	None,
};

struct Button
{
	Rectangle rec;
	Menus option = Menus::None;
	Color color = GRAY;
};

const int buttonWidth = 100;
const int buttonHeight = 25;
const int buttonSpacing = 20;

int titlesFonstSize = 100;
int textFontSize = 20;
int scoreFontSize = 50;

void PlayerMovement(Player& player, float deltaTime, int screenWidth, int screenHeight);
void SpawnSugaroid(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids, int screenWidth, int screenHeight);
void SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets);
void ResetGame(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player);
void DrawCredits(int screenWidth, int screenHeight);
void DrawGamePlay(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage);
void DrawMainMenu(Menus& selectedOption, Texture2D gamesTitle, int screenWidth, int screenHeight);
void DrawRules();
void DrawGameOverButtons(int buttonSelected);
bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button);
void DrawButton(Rectangle rect, const std::string& text, Color color);

Menus gameState = Menus::MainMenu;
int buttonSelected = 0;

int main(void)
{
	int screenWidth = 800;
	int screenHeight = 450;

	srand(time(NULL));
	InitWindow(screenWidth, screenHeight, "Sugaroids");
	SetTargetFPS(144);

	bool gameOver = false;
	float deltaTime;
	Player player;
	int points = 0;
	float spawnTimer = 0;
	Vector2 mouse;

	InitAudioDevice();

	Image sourceSpaceShipImage = LoadImage("../res/sprites/player/spaceship.png");
	Texture2D playerImage = LoadTextureFromImage(sourceSpaceShipImage);

	UnloadImage(sourceSpaceShipImage);

	Image sourceSugaroidImage = LoadImage("../res/sprites/enemies/sugaroid.png");
	Texture2D sugaroidImage = LoadTextureFromImage(sourceSugaroidImage);

	UnloadImage(sourceSugaroidImage);

	Image sourceBulletImage = LoadImage("../res/sprites/bullets/star.png");
	Texture2D bulletsImage = LoadTextureFromImage(sourceBulletImage);

	UnloadImage(sourceBulletImage);

	Music creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	Music mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Music gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Music gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");

	Sound hurtSound = LoadSound("../res/soundEffects/hurt.wav");
	Sound boomSound = LoadSound("../res/soundEffects/boom.wav");
	Sound dieSound = LoadSound("../res/soundEffects/die.wav");
	Sound shootSound = LoadSound("../res/soundEffects/shoot.wav");

	std::vector<Bullet> bullets;
	std::vector<Sugaroid> sugaroids;

	Image sourceIamgeBackground = LoadImage("../res/backgrounds/pacific.png");
	Texture2D backgroundImage = LoadTextureFromImage(sourceIamgeBackground);
	UnloadImage(sourceIamgeBackground);

	Texture2D gamesTitle = LoadTexture("../res/title.png");

	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();

		Music* actualMusic = nullptr;
		Music* stopMusic[3] = { };

		switch (gameState)
		{
		case Menus::MainMenu:

			actualMusic = &mainMenuMusic;

			stopMusic[0] = &gamePlayMusic;
			stopMusic[1] = &gameOverMusic;
			stopMusic[2] = &creditsMusic;
			break;

		case Menus::Playing:

			actualMusic = &gamePlayMusic;

			stopMusic[0] = &mainMenuMusic;
			stopMusic[1] = &gameOverMusic;
			stopMusic[2] = &creditsMusic;
			break;

		case Menus::Rules:

			actualMusic = &mainMenuMusic;

			stopMusic[0] = &gamePlayMusic;
			stopMusic[1] = &gameOverMusic;
			stopMusic[2] = &creditsMusic;
			break;

		case Menus::Credits:

			actualMusic = &creditsMusic;

			stopMusic[0] = &gamePlayMusic;
			stopMusic[1] = &gameOverMusic;
			stopMusic[2] = &mainMenuMusic;
			break;

		case Menus::Exit:
			break;

		default:
			break;
		}

		if (!IsMusicStreamPlaying(*actualMusic))
			PlayMusicStream(*actualMusic);

		for (int i = 0; i < 3; i++)
		{
			if (IsMusicStreamPlaying(*stopMusic[i]))
				StopMusicStream(*stopMusic[i]);
		}

		UpdateMusicStream(*actualMusic);

		if (gameState == Menus::Playing)
		{
			mouse = GetMousePosition();

			player.angle = atan2f(mouse.y - player.pos.y, mouse.x - player.pos.x) * (180.0f / PI);

			PlayerMovement(player, deltaTime, screenWidth, screenHeight);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				StopSound(shootSound);
				PlaySound(shootSound);
				SpawnStarBullet(player.pos, player.angle, bullets);
			}

			spawnTimer += deltaTime;
			if (spawnTimer > 1)
			{
				SpawnSugaroid(player.pos, sugaroids, screenWidth, screenHeight);
				spawnTimer = 0;
			}

			for (int i = 0; i < sugaroids.size(); )
			{
				sugaroids[i].position.x += sugaroids[i].velocity.x * deltaTime;
				sugaroids[i].position.y += sugaroids[i].velocity.y * deltaTime;

				if (CheckCollisionCircles(player.pos, player.size / 2, sugaroids[i].position, sugaroids[i].radius))
				{
					StopSound(hurtSound);
					PlaySound(hurtSound);

					sugaroids[i].toDestroy = true;
					player.lives--;
				}

				if (sugaroids[i].toDestroy ||
					sugaroids[i].position.x + sugaroids[i].radius < 0 ||
					sugaroids[i].position.x - sugaroids[i].radius > screenWidth ||
					sugaroids[i].position.y + sugaroids[i].radius < 0 ||
					sugaroids[i].position.y - sugaroids[i].radius > screenHeight)
				{
					if (!sugaroids[i].toDestroy)
						points += 5;
					else
						points += 25;

					sugaroids.erase(sugaroids.begin() + i);
				}
				else
				{
					i++;
				}
			}

			for (int i = 0; i < bullets.size(); )
			{
				bullets[i].position.x += bullets[i].velocity.x * deltaTime;
				bullets[i].position.y += bullets[i].velocity.y * deltaTime;

				bool bulletDestroyed = false;

				for (int j = 0; j < sugaroids.size(); j++)
				{
					if (CheckCollisionCircles(bullets[i].position, bullets[i].radius, sugaroids[j].position, sugaroids[j].radius))
					{
						StopSound(boomSound);
						PlaySound(boomSound);

						sugaroids[j].toDestroy = true;
						bullets[i].toDestroy = true;
						bulletDestroyed = true;
						break;
					}
				}

				if (bullets[i].toDestroy ||
					bullets[i].position.x + bullets[i].radius < 0 ||
					bullets[i].position.x - bullets[i].radius > screenWidth ||
					bullets[i].position.y + bullets[i].radius < 0 ||
					bullets[i].position.y - bullets[i].radius > screenHeight)
				{
					bullets.erase(bullets.begin() + i);
				}
				else
				{
					i++;
				}
			}

			if (player.lives <= 0)
				gameOver = true;

		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTexturePro(
			backgroundImage,
			Rectangle{ 0, 0, (float)backgroundImage.width, (float)backgroundImage.height },
			Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);

		switch (gameState)
		{
		case Menus::MainMenu:

			DrawMainMenu(gameState, gamesTitle, screenWidth, screenHeight);
			break;

		case Menus::Playing:

			if (gameOver)
			{
				int textWidth = MeasureText("GAME OVER", 20);
				DrawText("GAME OVER", (screenWidth - textWidth) / 2, screenHeight / 2, 20, RED);
				DrawGameOverButtons(buttonSelected);
			}
			else
			{
				DrawGamePlay(bullets, sugaroids, player, bulletsImage, playerImage, sugaroidImage);
			}

			break;

		case Menus::Rules:

			DrawRules();
			break;

		case Menus::Credits:

			DrawCredits(screenWidth, screenHeight);
			break;

		case Menus::Exit:
			break;

		default:
			break;
		}

		EndDrawing();
	}

	UnloadTexture(playerImage);
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

	CloseAudioDevice();
	CloseWindow();

	return 0;
}


void PlayerMovement(Player& player, float deltaTime, int screenWidth, int screenHeight)
{
	const float acceleration = 600.0f; // Aceleración al moverse
	const float minSpeed = 20.0f; // Velocidad mínima para continuar moviéndose

	// Captura la posición del ratón cuando se presiona el botón derecho
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		player.targetPos = GetMousePosition();

		// Calcular la dirección hacia el objetivo
		player.directionVector = { player.targetPos.x - player.pos.x, player.targetPos.y - player.pos.y };
		player.directionLength = sqrt(player.directionVector.x * player.directionVector.x + player.directionVector.y * player.directionVector.y);

		// Normalizar la dirección si la longitud es mayor que cero
		if (player.directionLength > 0)
		{
			player.directionVector.x /= player.directionLength;
			player.directionVector.y /= player.directionLength;

			// Acelera el jugador hacia la dirección del objetivo
			player.speed.x += player.directionVector.x * acceleration * deltaTime;
			player.speed.y += player.directionVector.y * acceleration * deltaTime;
		}
	}
	else
	{
		// Detener el movimiento si la velocidad es muy baja
		float currentSpeed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);
		if (currentSpeed < minSpeed)
		{
			player.speed.x = 0;
			player.speed.y = 0;
		}
	}

	// Limitar la velocidad máxima
	float currentSpeed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);
	if (currentSpeed > player.maxSpeed)
	{
		player.speed.x = (player.speed.x / currentSpeed) * player.maxSpeed;
		player.speed.y = (player.speed.y / currentSpeed) * player.maxSpeed;
	}

	// Actualizar la posición del jugador
	player.pos.x += player.speed.x * deltaTime;
	player.pos.y += player.speed.y * deltaTime;

	// Colisiones con los bordes de la pantalla
	if (player.pos.x < 0) player.pos.x = screenWidth;
	if (player.pos.x > screenWidth) player.pos.x = 0;
	if (player.pos.y < 0) player.pos.y = screenHeight;
	if (player.pos.y > screenHeight) player.pos.y = 0;
}

void SpawnSugaroid(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids, int screenWidth, int screenHeight)
{
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
	if (length > 0)
	{
		velocity.x = (direction.x / length) * speed;
		velocity.y = (direction.y / length) * speed;
	}

	// Create the new Sugaroid and add it to the vector
	Sugaroid newSugaroid;
	newSugaroid.position = spawnPosition;
	newSugaroid.velocity = velocity;
	newSugaroid.size = size;
	newSugaroid.radius = size / 2.0f;
	newSugaroid.toDestroy = false;

	sugaroids.push_back(newSugaroid);
}

void SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets)
{
	float size = 32.0f;   // Tamaño del sprite de la bala (aunque no parece estar en uso aquí)
	float speed = 500.0f; // Velocidad de la bala

	float playerAngleRadians = playerAngle * (PI / 180.0f);  // Inverso de la conversión anterior

	float directionX = cos(playerAngleRadians);
	float directionY = sin(playerAngleRadians);

	Vector2 velocity = { directionX * speed, directionY * speed };

	Bullet newBullet;
	newBullet.position = playerPosition;  // Start at the player's position
	newBullet.velocity = velocity;
	newBullet.radius = size / 2;  // Asumimos un radio de 16.0f
	newBullet.speed = speed;
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

void DrawGamePlay(std::vector<Bullet>& bullets, std::vector<Sugaroid>& sugaroids, Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		DrawTexturePro(bulletsImage, Rectangle{ 0, 0, (float)bulletsImage.width, (float)bulletsImage.height },  // Fuente (imagen completa)
			Rectangle{ bullets[i].position.x, bullets[i].position.y,  bullets[i].size,  bullets[i].size },  // Destino (posicion y tamaño)
			Vector2{ bullets[i].size / 2, bullets[i].size / 2 },  // Offset de origen para el centro
			0.0f,
			WHITE);
	}

	// Dibujar la textura redimensionada en pantalla
	DrawTexturePro(
		playerImage,  // La textura original
		Rectangle{ 0, 0, (float)playerImage.width, (float)playerImage.height },  // Fuente: toda la imagen original
		Rectangle{ player.pos.x, player.pos.y, 64, 64 },  // Destino: posición y nuevo tamaño 64x64
		Vector2{ 32, 32 },  // Offset (centro) si es necesario, depende de cómo quieras alinear el sprite
		player.angle,  // Rotación (0 si no quieres rotar)
		WHITE  // Color (generalmente WHITE para no aplicar ningún tinte)
	);


	for (int i = 0; i < sugaroids.size(); i++) {
		DrawTexturePro(
			sugaroidImage,
			Rectangle{ 0, 0, (float)sugaroidImage.width, (float)sugaroidImage.height },  // Fuente (imagen completa)
			Rectangle{ sugaroids[i].position.x, sugaroids[i].position.y, sugaroids[i].size, sugaroids[i].size },  // Destino (posición y tamaño)
			Vector2{ sugaroids[i].size / 2, sugaroids[i].size / 2 },  // Offset del centro
			0.0f,  // Rotación (puedes agregar rotación si es necesario)
			WHITE);
	}
}

void DrawMainMenu(Menus& selectedOption, Texture2D gamesTitle, int screenWidth, int screenHeight)
{
	const int maxButtons = 5;

	Vector2 mouse;
	Button button[maxButtons];

	int startX, startY;

	startX = (screenWidth - buttonWidth) / 2;
	startY = (screenHeight - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1)));

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { static_cast<float>(startX), static_cast<float>(startY + i * (buttonHeight + buttonSpacing)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
	}

	button[0].option = Menus::Playing;
	button[1].option = Menus::Rules;
	button[2].option = Menus::Credits;
	button[3].option = Menus::Exit;

	mouse = GetMousePosition();

	for (int i = 0; i < maxButtons; i++)
	{
		if (CheckMouseButtonCollition(mouse, button[i].rec))
		{
			button[i].color = WHITE;

			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				button[i].color = YELLOW;
			}

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
				selectedOption = button[i].option;
		}
	}

	// Calcula el factor de escala basado en las proporciones de la pantalla y la imagen
	float scaleFactor = std::min((float)screenWidth / gamesTitle.width, (float)screenHeight / gamesTitle.height);

	// Calcula el nuevo tamaño del título basado en el factor de escala
	float newWidth = gamesTitle.width * scaleFactor;
	float newHeight = gamesTitle.height * scaleFactor;

	// Dibuja el título centrado y escalado
	DrawTexturePro(
		gamesTitle,
		Rectangle{ 0, 0, (float)gamesTitle.width, (float)gamesTitle.height },  // Fuente: toda la imagen original
		Rectangle{ (screenWidth - newWidth) / 2, (screenHeight / 2 - newHeight), newWidth, newHeight },  // Destino: posición centrada con tamaño escalado
		Vector2{ 0, 0 },  // Offset de origen (esquina superior izquierda)
		0.0f,  // Sin rotación
		WHITE  // Sin tintes de color
	);

		for (int i = 0; i < maxButtons; i++)
		{
			switch (button[i].option)
			{
			case Menus::Playing:

				DrawButton(button[i].rec, "Play", button[i].color);
				break;

			case Menus::Rules:

				DrawButton(button[i].rec, "Options", button[i].color);
				break;

			case Menus::Credits:

				DrawButton(button[i].rec, "Credits", button[i].color);
				break;

			case Menus::Exit:

				DrawButton(button[i].rec, "Exit", button[i].color);
				break;

			default: break;
			}
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

bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button)
{
	// is the point inside the rectangle's bounds?
	if (mouse.x >= button.x &&        // right of the left edge AND
		mouse.x <= button.x + button.width &&   // left of the right edge AND
		mouse.y >= button.y &&        // below the top AND
		mouse.y <= button.y + button.height) {   // above the bottom

		return true;
	}

	return false;
}

void DrawButton(Rectangle rect, const std::string& text, Color color)
{
	DrawRectangleRec(rect, color);
	Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), textFontSize, 1);
	Vector2 textPosition = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
	DrawText(text.c_str(), static_cast<int>(textPosition.x), static_cast<int>(textPosition.y), textFontSize, BLACK);
}