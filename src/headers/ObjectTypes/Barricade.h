#pragma once
#include <iostream>
#include "Plane.h"

class Barricade : public Plane {
public:
    int lives = 2;

    Barricade() {}
    Barricade(GLfloat x, GLfloat y, GLfloat size) {
		this->position.x = x;
		this->position.y= y;
		this->w = size;
		this->h = size;

		SetPlane(1.0f);
	}
	Barricade(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->position.x = x;
		this->position.y= y;
		this->w = w;
		this->h = h;

		SetPlane(1.0f);
	}
};
