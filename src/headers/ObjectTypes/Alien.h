#pragma once
#include <iostream>
#include <string>
#include "Plane.h"

class Alien : public Plane {
public:
	double moveAmount = 0.05f;
	bool moveRight = true, isAlive = true;

	double deathAnimTimer = 0.0f;

	Alien(GLfloat xPos, GLfloat yPos, GLfloat size) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	Alien(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = w;
		this->h = h;

		SetPlane();
	}

	bool DoMove(double deltaTime) {
		if (xPos > 2.0f - (w / 2) && moveRight) return true;
		else if (xPos < -2.0f + (w / 2) && !moveRight) return true;

		if (moveRight) position.Move(moveAmount * deltaTime, 0.0f, 0.0f);
		else position.Move(-moveAmount * deltaTime, 0.0f, 0.0f);

		return false;
	}

	void MoveDown() {
		position.Move(0, -h, 0);
		moveRight = !moveRight;
	}
};
