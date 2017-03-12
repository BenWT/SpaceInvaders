#pragma once
#include <iostream>
#include <string>
#include "Plane.h"

class Alien : public Plane {
public:
	double moveAmount = 0.05f;
	bool moveRight = true, isAlive = true;

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
		if (xPos > 0.9f && moveRight) return true;
		else if (xPos < -0.9f && !moveRight) return true;

		if (moveRight) Move(moveAmount * deltaTime, 0.0f, 0.0f);
		else Move(-moveAmount * deltaTime, 0.0f, 0.0f);

		return false;
	}

	void MoveDown() {
		Move(0, -h, 0);
		moveRight = !moveRight;
	}
};
