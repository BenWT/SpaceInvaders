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
	GLfloat w, h;

	Plane() {}
	Plane(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->w = w;
		this->h = h;
	}

	void SetPlane() {
		AddCoord(0.0f - (w / 2), 0.0f + (h / 2)); // Top Left
		AddCoord(0.0f + (w / 2), 0.0f + (h / 2)); // Top Right
		AddCoord(0.0f - (w / 2), 0.0f - (h / 2)); // Bottom Left
		AddCoord(0.0f + (w / 2), 0.0f - (h / 2)); // Bottom Right

		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);

		//translation = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, zPos));
	}
};
