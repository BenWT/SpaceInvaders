//
// Copyright (c) 2017 by Ben Townshend. All Rights Reserved.
//

// Includes
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <cmath>
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

#define PI 3.14159265

// Namespaces
using namespace std;
using namespace chrono;

// Global Variables
SDL_Window* window;
SDL_GLContext context;
bool running = false;
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
	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	glm::mat4 projectionMat, viewMat;

	// force 4:3 aspect using viewport
	SizeWindow();
	projectionMat = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	viewMat = glm::translate(viewMat, glm::vec3(2.0f, 1.5f, 0.0f));

	gameState = GameState();

	SDL_Surface* image0 = IMG_Load("assets/test.png");
	SDL_Surface* image1 = IMG_Load("assets/test2.png");
	gameState.images[0] = image0;
	gameState.images[1] = image1;

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
			break;

			// TODO Make not janky
			case SDL_KEYUP:
				if (k == SDLK_a || k == SDLK_d) gameState.player.movementInputX = 0;
			break;

			case SDL_KEYDOWN:
				if (k == SDLK_ESCAPE) running = false;

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

	if (gameState.aliens.size() > 0 && !gameState.isEndgame) {
		gameState.DoCollisions(deltaTime);

		gameState.player.DoMove(deltaTime);

		vector<PlayerBullet>::iterator pBulletIT;
		for (pBulletIT = gameState.playerBullets.begin(); pBulletIT < gameState.playerBullets.end(); pBulletIT++) {
			pBulletIT->DoMove(deltaTime);
		}

		vector<Alien>::iterator alienIT;
		for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
			// TODO alien death animation here
			if (alienIT->DoMove(deltaTime)) shouldMoveDown = true;
		}
		if (shouldMoveDown) {
			for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
				alienIT->MoveDown();
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
			// do animate
			//gameState.player.Move(0, asd, 0);
			if (gameState.playerBullets.size() > 0) gameState.playerBullets.clear();
			gameState.player.position.y += 1.5f * deltaTime;
		}

		gameState.endgameCounter += deltaTime;
		cout << gameState.endgameCounter << endl;
	}
}

void Render(GLuint &shaderProgram, glm::mat4 &projectionMat, glm::mat4 &viewMat) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	gameState.background.Render(shaderProgram, projectionMat, viewMat, gameState.textures[0]);
	gameState.player.Render(shaderProgram, projectionMat, viewMat, gameState.textures[1]);

	vector<PlayerBullet>::iterator pBulletIT;
	for (pBulletIT = gameState.playerBullets.begin(); pBulletIT < gameState.playerBullets.end(); pBulletIT++) {
		pBulletIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[1]);
	}

	vector<Alien>::iterator alienIT;
	for (alienIT = gameState.aliens.begin(); alienIT < gameState.aliens.end(); alienIT++) {
		alienIT->Render(shaderProgram, projectionMat, viewMat, gameState.textures[1]);
	}

	SDL_GL_SwapWindow(window);
}

void GenerateGame(bool firstGenerate) {
	gameState.aliens.clear();
	gameState.playerBullets.clear();

	int columns = 11, rows = 1;
	GLfloat top = 1.5f, bottom = -1.5f, left = -2.0f, right = 2.0f, width = 0.25f, height = 0.18, gap = 0.1f;

	if (firstGenerate) {
		Plane* bg = new Plane(0.0f, 0.0f, 4.0f, 3.0f);
		gameState.background = *bg;
		delete bg;

		Player* p = new Player(0.0f, bottom + (height / 2), width, height);
		gameState.player = *p;
		delete p;
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
