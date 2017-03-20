#pragma once
#include <iostream>
#include "Plane.h"
#include "Player.h"

class PlayerBullet : public Plane {
public:
	double moveAmount = 1.0f;
    double movementInputX = 0.0f;
    bool shouldDestroy = false;

    PlayerBullet() {}
    PlayerBullet(GLfloat xPos, GLfloat yPos, GLfloat size) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	PlayerBullet(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = w;
		this->h = h;

		SetPlane();
	}

    void DoMove(double deltaTime) {
        position.Move(0.0f, moveAmount * deltaTime, 0.0f);

        if (yPos > 1.5f) shouldDestroy = true;
    }
};
