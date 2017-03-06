#pragma once
#include <string>
#include "Plane.h"

class Alien : public Plane {
public:
	Alien(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		SetPlane();
	}

	bool DoMove() {
		// Transform here

		//if (x >= max || x <= -max) return true;
		return false;
	}

	void MoveDown() {

	}
};
