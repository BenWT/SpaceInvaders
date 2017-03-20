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
    PlayerBullet(GLfloat x, GLfloat y, GLfloat size) {
		this->position.x = x;
		this->position.y= y;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	PlayerBullet(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->position.x = x;
		this->position.y= y;
		this->w = w;
		this->h = h;

		SetPlane();
	}

    void DoMove(double deltaTime) {
        position.Move(0.0f, moveAmount * deltaTime, 0.0f);

        if (position.y > 1.5f) shouldDestroy = true;
    }
};
