//
// Copyright (c) 2017 by Ben Townshend. All Rights Reserved.
//

// Includes
#include "headers/Globals.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <GL/glew.h>
#include "SDL.h"
#define GLM_FORCE_RADIANS // force glm to use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/Shaders.h"

#define PI 3.14159265

// Namespaces
using namespace std;
using namespace chrono;

// Global Variables
SDL_Window* window;
SDL_GLContext context;
bool running = false;
bool hasChanged = true;
vector<GLfloat> vertices;
vector<GLuint> indices;

void ProcessInput();
void Render(GLuint shaderProgram, GLuint vertArray);
void setVertices(int noOfIterations);
void setIndices(int noOfIterations);
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

	//int x = display.w, y = display.h;
	// TODO Change back
	int x = 1200, y = 1200;

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

	// Set viewport
	glViewport(0, 0, x / 2, y / 2);

	// Initialise Shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();
	GLint success;

	// Vertex Shader
	glShaderSource(vertShader, 1, &vertShaderSource, NULL);
	glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Vertex Shader error. \n");
		running = false;
    }

	// Fragment Shader
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
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

	// Initialise Buffers
	GLuint vertBuffer, vertArray, elementBuffer;

	// Game Loop
	running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	setVertices(360);
	setIndices(360);
	hasChanged = true;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		if (hasChanged) {
			// Fill Buffers
			glGenVertexArrays(1, &vertArray);
			glGenBuffers(1, &vertBuffer);
		    glGenBuffers(1, &elementBuffer);

			glBindVertexArray(vertArray);

			glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			hasChanged = false;
		}

		ProcessInput();
		Render(shaderProgram, vertArray);
	}

	// Cleanup on Close
	glDeleteVertexArrays(1, &vertArray);
	glDeleteBuffers(1, &vertBuffer);
	glDeleteBuffers(1, &elementBuffer);
	SDL_GL_DeleteContext(context);
	window = NULL;
	SDL_Quit();
	return 0;
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		SDL_Keycode k = event.key.keysym.sym;

		switch (event.type) {
			case SDL_KEYDOWN:
				if (k == SDLK_ESCAPE) {
					running = false;
				}
			break;

			case SDL_QUIT:
				SDL_Log("Program quit.");
	 		    running = false;
			break;
		}
	}
}

void Render(GLuint shaderProgram, GLuint vertArray) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(vertArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(window);
}

void setVertices(int noOfIterations) {
	float angle = 360 / noOfIterations;
	vertices.clear();

	// Centre Point
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	// Rest of points
	for (int i = 0; i < noOfIterations; i++) {
		// cos does x, sin does y
		vertices.push_back(sin(i * angle * PI / 180));
		vertices.push_back(cos(i * angle * PI / 180));
		vertices.push_back(0.0f);
	}
}

void setIndices(int noOfIterations) {
	indices.clear();

	for (int i = 1; i < noOfIterations + 1; i++) {
		if (i == noOfIterations) {
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(i);
		} else {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}
}
