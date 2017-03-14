#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "Plane.h"

class Player : public Plane {
public:
	double moveAmount = 0.8f;
    double movementInputX = 0.0f;
	std::vector<Plane> bullets;

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

        if (xPos > 1.5f) xPos = 1.5f;
        if (xPos < -1.5f) xPos = -1.5af;
    }
};
