#pragma once
#include <iostream>
#include <string>
#include "Plane.h"

class Alien : public Plane {
public:
	GLfloat speed = 200;

	Alien(GLfloat x, GLfloat y, GLfloat size) {
		this->x = x;
		this->y = y;
		this->w = size;
		this->h = size;

		SetPlane();
	}
	Alien(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		SetPlane();
	}

	bool DoMove(double deltaTime) {
		Move(speed * deltaTime, 0, 0);

		//if (x >= max || x <= -max) return true;
		// if (y > min y) return true;
		return false;
	}

	void MoveDown() {
		// if (y > miny)
		Move(0, h, 0);

		speed = -speed;
	}
};
