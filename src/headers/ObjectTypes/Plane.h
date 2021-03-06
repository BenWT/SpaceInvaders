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
	Plane(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this->position.x = x;
		this->position.y = y;
		this->w = w;
		this->h = h;

		SetPlane(1.0f);
	}
	Plane(GLfloat x, GLfloat y, GLfloat w, GLfloat h, float uvScale) {
		this->position.x = x;
		this->position.y = y;
		this->w = w;
		this->h = h;

		SetPlane(uvScale);
	}

	void SetPlane(float scale) {
		AddCoord(0.0f - (w / 2), 0.0f + (h / 2), 0.0f * scale, 1.0f * scale); // Top Left
		AddCoord(0.0f + (w / 2), 0.0f + (h / 2), 1.0f * scale, 1.0f * scale); // Top Right
		AddCoord(0.0f - (w / 2), 0.0f - (h / 2), 0.0f * scale, 0.0f * scale); // Bottom Left
		AddCoord(0.0f + (w / 2), 0.0f - (h / 2), 1.0f * scale, 0.0f * scale); // Bottom Right

		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
	}

	bool CheckCollision(GLfloat x, GLfloat y) {
		if (x >= position.x - (w / 2) && x <= position.x + (w / 2)) {
			if (y >= position.y - (h / 2) && y <= position.y + (h / 2)) {
				return true;
			}
		}
		return false;
	}
};
