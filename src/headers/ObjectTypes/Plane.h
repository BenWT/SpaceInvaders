#pragma once
#include <string>
#include "../GameObject.h"

class Plane : public GameObject {
private:
	void AddCoord(GLfloat x, GLfloat y) {
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);
	}
public:
	GLfloat x, y, w, h;

	Plane() {}
	Plane(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void SetPlane() {
		AddCoord(x - (w / 2), y + (h / 2)); // Top Left
		AddCoord(x + (w / 2), y + (h / 2)); // Top Right
		AddCoord(x - (w / 2), y - (h / 2)); // Bottom Left
		AddCoord(x + (w / 2), y - (h / 2)); // Bottom Right

		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
	}
};
