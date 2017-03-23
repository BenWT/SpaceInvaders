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
    Player(GLfloat x, GLfloat y, GLfloat size) {
		this->position.x = x;
		this->position.y = y;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	Player(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->position.x = x;
		this->position.y = y;
		this->w = w;
		this->h = h;

		SetPlane();
	}

    void DoMove(double deltaTime) {
        position.Move(moveAmount * deltaTime * movementInputX, 0.0f, 0.0f);

        if (position.x > 1.9f - (w / 2)) position.x = 1.9f - (w / 2);
        if (position.x < -1.9f + (w / 2)) position.x = -1.9f + (w / 2);
    }
};
