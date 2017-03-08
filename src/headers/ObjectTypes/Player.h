#pragma once
#include <iostream>
#include <string>
#include "Plane.h"

class Player : public Plane {
public:
	double moveAmount = 0.25f;
    double movementInputX = 0.0f;

    Player() {}
    Player(GLfloat xPos, GLfloat yPos, GLfloat size) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	Player(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = w;
		this->h = h;

		SetPlane();
	}

    void DoMove(double deltaTime) {
        Move(moveAmount * deltaTime * movementInputX, 0.0f, 0.0f);

        if (xPos > 0.9f) xPos = 0.9f;
        if (xPos < -0.9f) xPos = -0.9f;
    }
};
