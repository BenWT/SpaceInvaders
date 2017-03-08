//
// Copyright (c) 2017 by Ben Townshend. All Rights Reserved.
//

// Includes
#include "headers/Globals.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <cmath>
#include <GL/glew.h>
#include "SDL.h"
#define GLM_FORCE_RADIANS // force glm to use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/Shaders.h"
#include "headers/GameObject.h"
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
Player player;
vector<Alien> aliens;

void ProcessInput();
void Update(double deltaTime);
void Render(GLuint shaderProgram);
void GenerateGame();
high_resolution_clock::time_point NowTime() {
	return chrono::high_resolution_clock::now();
}
double TimeSinceLastFrame(high_resolution_clock::time_point frameTime) {
	return (duration_cast<microseconds>(NowTime() - frameTime).count()) / 1000000.0;
}

int main(int argc, char *argv[]) {
	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	}

	// Get Display Info
	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);
	int x = display.w, y = display.h;

	// Create Window
	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x / 2, y / 2, SDL_WINDOW_OPENGL);
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

	// TODO read and compile shaders from file
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

	GenerateGame();

	// Game Loop
	running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		ProcessInput();
		Update(deltaTime);
		Render(shaderProgram);
	}

	// Cleanup on Close
	aliens.clear();
	aliens.shrink_to_fit();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

void ProcessInput() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		SDL_Keycode k = event.key.keysym.sym;

		switch (event.type) {
			case SDL_KEYUP:
				if (k == SDLK_a || k == SDLK_d) player.movementInputX = 0;
			break;

			case SDL_KEYDOWN:
				if (k == SDLK_ESCAPE) {
					running = false;
				}

				if (k == SDLK_a) {
					player.movementInputX = -1;
				} else if (k == SDLK_d) {
					player.movementInputX = 1;
				}
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

	player.DoMove(deltaTime);

	vector<Alien>::iterator it;
	for (it = aliens.begin(); it < aliens.end(); it++) {
		if (it->DoMove(deltaTime)) shouldMoveDown = true;
	}

	if (shouldMoveDown) {
		for (it = aliens.begin(); it < aliens.end(); it++) {
			it->MoveDown();
		}
	}
}

void Render(GLuint shaderProgram) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	player.Render(shaderProgram);

	vector<Alien>::iterator it;
	for (it = aliens.begin(); it < aliens.end(); it++) {
		it->Render(shaderProgram);
	}

	SDL_GL_SwapWindow(window);
}

void GenerateGame() {
	aliens.clear();

	int columns = 6, rows = 3;
	GLfloat top = 1.0, left = -1.0, size = 0.2;

	Player* p = new Player(0.0f, -0.9f, size);
	player = *p;
	delete p;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Alien* a = new Alien((left + size / 2) + (j * size), (top - size / 2) - (i * size), size);
			aliens.push_back(*a);
			delete a;
		}
	}
}
