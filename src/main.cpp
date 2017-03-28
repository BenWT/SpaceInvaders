//
// Copyright (c) 2017 by Ben Townshend. All Rights Reserved.
//

// Includes
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#define GLM_FORCE_RADIANS // force glm to use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/Shaders.h"
#include "headers/Vector3.h"
#include "headers/GameObject.h"
#include "headers/GameState.h"
#include "headers/ObjectTypes/Plane.h"
#include "headers/ObjectTypes/Player.h"
#include "headers/ObjectTypes/Alien.h"
#include "headers/ObjectTypes/Barricade.h"
#include "headers/ObjectTypes/PlayerBullet.h"
#include "headers/ObjectTypes/EnemyBullet.h"

#define PI 3.14159265

// Namespaces
using namespace std;
using namespace chrono;

// Global Variables
SDL_Window* window;
SDL_GLContext context;
bool running = false;
glm::mat4 projectionMat, viewMat, overlayMat;
GLfloat viewportHeight, viewportWidth;
GameState gameState;

void ProcessInput();
void Update(double deltaTime);
void Render(GLuint &shaderProgram, glm::mat4 &projectionMat, glm::mat4 &viewMat);
void GenerateGame(bool firstGenerate);
high_resolution_clock::time_point NowTime() {
	return chrono::high_resolution_clock::now();
}
double TimeSinceLastFrame(high_resolution_clock::time_point frameTime) {
	return (duration_cast<microseconds>(NowTime() - frameTime).count()) / 1000000.0;
}
void toggleFullScreen() {
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool toggle = SDL_GetWindowFlags(window) & FullscreenFlag;
	SDL_SetWindowFullscreen(window, toggle ? 0 : FullscreenFlag);
}
void SizeWindow() {
	int w, h, desiredW;
	w = SDL_GetWindowSurface(window)->w;
	h = SDL_GetWindowSurface(window)->h;
	desiredW = (h / 3.0f) * 4.0f;

	glViewport((w - desiredW) / 2, 0.0f, desiredW, h);
}

int main(int argc, char *argv[]) {
	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	}

	// Initialise SDL_image
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL_image failed to initialise. \n");
		return 1;
	}

	// Get Display Info
	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);
	int x = display.w, y = display.h;

	// Create Window
	window = SDL_CreateWindow("Ben Townshend - CGP2012M - 13480634", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x / 2, y	 / 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL failed to create the window. \n");
		return 1;
	}

	// OpenGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create Context
	context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL failed to create the OpenGL context. \n");
		return 1;
	}

	// Initialise GLEW
	GLenum rev;
	glewExperimental = GL_TRUE;
	rev = glewInit();
	if (GLEW_OK != rev) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL failed to initialise GLEW. \n");
		return 1;
	}

	// Initialise Shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();
	GLint success;

	// Vertex Shader
	glShaderSource(vertShader, 1, vertShaderSource, NULL);
	glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Vertex Shader error. \n");
		running = false;
    }

	// Fragment Shader
	glShaderSource(fragShader, 1, fragShaderSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Fragement Shader error. \n");
		running = false;
	}

	// Shader Program
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
    if (!success) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Shader Program failed to start. \n");
		running = false;
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

	// force 4:3 aspect using viewport
	SizeWindow();
	projectionMat = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	viewMat = glm::translate(viewMat, glm::vec3(2.0f, 1.5f, 0.0f));
	overlayMat = viewMat;

	gameState = GameState();

	for (int i = 0; i < 10; i++) {
		string s = "assets/numbers/" + to_string(i) + ".png";
		SDL_Surface* num = IMG_Load(s.c_str());
		gameState.images[i] = num;
	}

	SDL_Surface* image0 = IMG_Load("assets/background.png");
	SDL_Surface* image1 = IMG_Load("assets/asteroids.png");
	SDL_Surface* image2 = IMG_Load("assets/barricade.png");
	SDL_Surface* image3 = IMG_Load("assets/barricade-broken.png");
	SDL_Surface* image4 = IMG_Load("assets/player.png");
	SDL_Surface* image5 = IMG_Load("assets/alien.png");
	SDL_Surface* image6 = IMG_Load("assets/alien2.png");
	SDL_Surface* image7 = IMG_Load("assets/bullet.png"); // bullet
	SDL_Surface* image8 = IMG_Load("assets/white.png"); // edge
	SDL_Surface* image9 = IMG_Load("assets/numbers/score.png");
	gameState.images[10] = image0;
	gameState.images[11] = image1;
	gameState.images[12] = image2;
	gameState.images[13] = image3;
	gameState.images[14] = image4;
	gameState.images[15] = image5;
	gameState.images[16] = image6;
	gameState.images[17] = image7;
	gameState.images[18] = image8;
	gameState.images[19] = image9;

	gameState.GenerateTextures();

	GenerateGame(true);

	// Game Loop
	running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		ProcessInput();
		Update(deltaTime);
		Render(shaderProgram, projectionMat, viewMat);
	}

	// Cleanup on Close
	gameState.aliens.clear();
	gameState.aliens.shrink_to_fit();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		SDL_Keycode k = event.key.keysym.sym;

		switch (event.type) {
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					SizeWindow();
				}
			break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) gameState.PlayerFire();
				if (event.button.button == SDL_BUTTON_RIGHT) gameState.EnemyFire(0);
			break;

			// TODO Make not janky
			case SDL_KEYUP:
				if (k == SDLK_a || k == SDLK_d) gameState.player.movementInputX = 0;
			break;

			case SDL_KEYDOWN:
				if (k == SDLK_ESCAPE) running = false;
				if (k == SDLK_RETURN) toggleFullScreen();

				if (k == SDLK_a) gameState.player.movementInputX = -1;
				else if (k == SDLK_d) gameState.player.movementInputX = 1;

				if (k == SDLK_SPACE) gameState.PlayerFire();
			break;

			case SDL_QUIT:
				SDL_Log("Program quit.");
	 		    running = false;
			break;
		}
	}
}

void Update(double deltaTime) {
	bool shouldMoveDown = false;

	gameState.background.uvoff.y -= deltaTime * 0.005f;
	gameState.asteroids.uvoff.x += deltaTime * 0.005f;
	gameState.asteroids.uvoff.y -= deltaTime * 0.05f;

	if (gameState.aliens.size() > 0 && !gameState.isEndgame) {
		if (gameState.DoCollisions(deltaTime)) {
			viewMat = glm::translate(viewMat, glm::vec3(gameState.player.position.x / 4, 0.0, 0.0));
			GenerateGame(true);
		}

		float moveAmount = gameState.player.DoMove(deltaTime);
		viewMat = glm::translate(viewMat, glm::vec3(-moveAmount / 4, 0.0, 0.0));

		int v = rand() % 1200;
		if (v < gameState.aliens.size()) gameState.EnemyFire(v);

		vector<PlayerBullet>::iterator pBulletIT;
		vector<EnemyBullet>::iterator eBulletIT;
		for (pBulletIT = gameState.playerBullets.begin(); pBulletIT < gameState.playerBullets.end(); pBulletIT++) {
			pBulletIT->DoMove(deltaTime);
		}
		for (eBulletIT = gameState.enemyBullets.begin(); eBulletIT < gameState.enemyBullets.end(); eBulletIT++) {
			eBulletIT->DoMove(deltaTime);
		}

		vector<Alien>::iterator alienIT;
		for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
			if (alienIT->DoMove(deltaTime)) shouldMoveDown = true;
		}
		if (shouldMoveDown) {
			for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
				alienIT->MoveDown();

				if (alienIT->position.y < -1.2f) {
					viewMat = glm::translate(viewMat, glm::vec3(gameState.player.position.x / 4, 0.0, 0.0));
					GenerateGame(true);
					break;
				}
			}
		}
	} else {
		if (!gameState.isEndgame) gameState.isEndgame = true;

		if (gameState.endgameCounter > 3.0) {
			gameState.isEndgame = false;
			gameState.endgameCounter = 0.0;
		} else if (gameState.endgameCounter > 2.0) {
			if (!gameState.aliens.size() > 0) GenerateGame(false);

			vector<Alien>::iterator alienIT;
			for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
				alienIT->position.y -= 1.5f * deltaTime;
			}
		} else {
			if (gameState.playerBullets.size() > 0) gameState.playerBullets.clear();
			if (gameState.enemyBullets.size() > 0) gameState.enemyBullets.clear();
			gameState.player.position.y += 1.5f * deltaTime;
		}

		gameState.endgameCounter += deltaTime;
	}

	gameState.gameTime += deltaTime;
}

void Render(GLuint &shaderProgram, glm::mat4 &projectionMat, glm::mat4 &viewMat) {
	int animTime = (int)gameState.gameTime % 2;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	gameState.background.Render(shaderProgram, projectionMat, viewMat, gameState.textures[10]);
	gameState.asteroids.Render(shaderProgram, projectionMat, viewMat, gameState.textures[11]);
	gameState.player.Render(shaderProgram, projectionMat, viewMat, gameState.textures[14]);

	vector<PlayerBullet>::iterator pBulletIT;
	vector<EnemyBullet>::iterator eBulletIT;
	for (pBulletIT = gameState.playerBullets.begin(); pBulletIT < gameState.playerBullets.end(); pBulletIT++) {
		pBulletIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[17]);
	}
	for (eBulletIT = gameState.enemyBullets.begin(); eBulletIT < gameState.enemyBullets.end(); eBulletIT++) {
		eBulletIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[17]);
	}

	vector<Alien>::iterator alienIT;
	for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
		if (animTime == 0) alienIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[15]);
		else alienIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[16]);
	}

	vector<Barricade>::iterator barricadeIT;
	for (barricadeIT = gameState.barricades.begin(); barricadeIT < gameState.barricades.end(); barricadeIT++) {
		if (barricadeIT->lives == 2) barricadeIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[12]);
		else barricadeIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[13]);
	}

	vector<Plane>::iterator edgeIT;
	for (edgeIT = gameState.edges.begin(); edgeIT < gameState.edges.end(); edgeIT++) {
		edgeIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[18]);
	}

	vector<Plane>::iterator lifeIT;
	for (lifeIT = gameState.playerLifeIndicators.begin(); lifeIT < gameState.playerLifeIndicators.end(); lifeIT++) {
		lifeIT->Render(shaderProgram, projectionMat, overlayMat, gameState.textures[14]);
	}

	int n = gameState.playerScore, i = 0;
	if (n == 0) {
		if (gameState.playerScoreIndicators.size() == 0) {
			Plane* p = new Plane(1.8, 1.4, 0.1, 0.1);
			gameState.playerScoreIndicators.push_back(*p);
			delete p;
		}

		gameState.playerScoreIndicators[0].Render(shaderProgram, projectionMat, overlayMat, gameState.textures[0]);
	} else {
		while (n > 0) {
			int digit = n % 10;
	    	n /= 10;

			if (gameState.playerScoreIndicators.size() <= i) {
				Plane* p = new Plane(gameState.playerScoreIndicators[i - 1].position.x - 0.1, 1.4, 0.1, 0.1);
				gameState.playerScoreIndicators.push_back(*p);
				delete p;
			}

			gameState.playerScoreIndicators[i].Render(shaderProgram, projectionMat, overlayMat, gameState.textures[digit]);
			i++;
		}
	}

	gameState.scoreText.position.x = gameState.playerScoreIndicators.back().position.x - 0.3;
	gameState.scoreText.Render(shaderProgram, projectionMat, overlayMat, gameState.textures[19]);

	SDL_GL_SwapWindow(window);
}

void GenerateGame(bool firstGenerate) {
	gameState.aliens.clear();
	gameState.playerBullets.clear();
	gameState.enemyBullets.clear();

	int columns = 11, rows = 5;
	GLfloat top = 1.3f, bottom = -1.3f, left = -1.9f, right = 1.9f, width = 0.25f, height = 0.18, gap = 0.1f;

	if (firstGenerate) {
		gameState.playerScore = 0.0;
		gameState.playerLives = 3;
		gameState.edges.clear();
		gameState.playerLifeIndicators.clear();
		gameState.barricades.clear();

		Plane* bg = new Plane(0.0f, 0.0f, 8.0f, 6.0f, 2.0f);
		gameState.background = *bg;
		delete bg;

		Plane* asteroids = new Plane(0.0f, 0.0f, 8.0f, 6.0f, 2.0f);
		gameState.asteroids = *asteroids;
		delete asteroids;

		Plane* scoreText = new Plane(0.0f, 1.4, 0.4, 0.1);
		gameState.scoreText = *scoreText;
		delete scoreText;

		Player* p = new Player(0.0f, bottom + (height / 2), width, height);
		gameState.player = *p;
		delete p;

		Plane* edgeL = new Plane(left - 0.05, 0.0, 0.1, 2.6);
		gameState.edges.push_back(*edgeL);
		delete edgeL;
		Plane* edgeR = new Plane(right + 0.05, 0.0, 0.1, 2.6);
		gameState.edges.push_back(*edgeR);
		delete edgeR;

		for (int i = 0; i < 3; i++) {
			GLfloat w = 0.1388, h = 0.1;

			Plane* p = new Plane(left + (w / 2) + i * w, bottom - h / 2 - 0.05f, w, h);
			gameState.playerLifeIndicators.push_back(*p);
			delete p;
		}

		for (int i = 0; i < 4; i++) {
			GLfloat w = (right * 2) / 4;

			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 3; y++) {
					GLfloat sectionWidth = (w - 0.2) / 10, sectionHeight;
					sectionHeight = sectionWidth;

					Barricade* p = new Barricade(0.1 + left + i * w + x * sectionWidth + (sectionWidth / 2), -1.0 + y * sectionHeight + (sectionHeight / 2), sectionWidth, sectionHeight);
					gameState.barricades.push_back(*p);
					delete p;
				}
			}
		}
	} else {
		gameState.player.position.y = bottom + (height / 2);
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Alien* a = new Alien((left + width / 2 + gap) + (j * width), (top - height / 2) - (i * height), width, height);
			if (!firstGenerate) a->position.y += 1.5f;
			gameState.aliens.push_back(*a);
			delete a;
		}
	}
}
