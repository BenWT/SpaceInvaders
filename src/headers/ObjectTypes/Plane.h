#pragma once
#include <string>
#include "../GameObject.h"

class Plane : public GameObject {
private:
	void AddCoord(GLfloat x, GLfloat y, GLfloat uvx, GLfloat uvy) {
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);
		vertices.push_back(uvx);
		vertices.push_back(uvy);
	}
public:
	GLfloat w, h;

	Plane() {}
	Plane(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = w;
		this->h = h;

		SetPlane();
	}

	void SetPlane() {
		AddCoord(0.0f - (w / 2), 0.0f + (h / 2), 0.0f, 1.0f); // Top Left
		AddCoord(0.0f + (w / 2), 0.0f + (h / 2), 1.0f, 1.0f); // Top Right
		AddCoord(0.0f - (w / 2), 0.0f - (h / 2), 0.0f, 0.0f); // Bottom Left
		AddCoord(0.0f + (w / 2), 0.0f - (h / 2), 1.0f, 0.0f); // Bottom Right

		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
	}

	bool CheckCollision(GLfloat x, GLfloat y) {
		if (x >= xPos - (w / 2) && x <= xPos + (w / 2)) {
			if (y >= yPos - (h / 2) && y <= yPos + (h / 2)) {
				return true;
			}
		}
		return false;
	}
};
