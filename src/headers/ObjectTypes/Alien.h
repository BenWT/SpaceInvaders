#pragma once
#include <iostream>
#include <string>
#include "Plane.h"

class Alien : public Plane {
public:
	double moveAmount = 0.05f;
	bool moveRight = true, isAlive = true;

	double deathAnimTimer = 0.0f;

	Alien(GLfloat x, GLfloat y, GLfloat size) {
		this->position.x = x;
		this->position.y = y;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	Alien(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->position.x = x;
		this->position.y = y;
		this->w = w;
		this->h = h;

		SetPlane();
	}

	bool DoMove(double deltaTime) {
		if (position.x > 1.9f - (w / 2) && moveRight) return true;
		else if (position.x < -1.9f + (w / 2) && !moveRight) return true;

		if (moveRight) position.Move(moveAmount * deltaTime, 0.0f, 0.0f);
		else position.Move(-moveAmount * deltaTime, 0.0f, 0.0f);

		return false;
	}

	void MoveDown() {
		position.Move(0, -h, 0);
		moveRight = !moveRight;
		moveAmount += 0.01f;
	}
};
