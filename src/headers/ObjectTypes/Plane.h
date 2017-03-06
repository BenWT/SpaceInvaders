#include <string>
#include "../GameObject.h"

class Plane : public GameObject {
private:
	void AddCoord(GLfloat x, GLfloat y) {
		this.vertices.push_back(x);
		this.vertices.push_back(y);
		this.vertices.push_back(0);
	}
public:
	GLfloat x, y, w, h;

	Plane(int itemIndex, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this.itemIndex = itemIndex;
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}

	void SetPlane() {
		AddCoord(x - (w / 2), y + (h / 2)); // Top Left
		AddCoord(x + (w / 2), y + (h / 2)); // Top Right
		AddCoord(x - (w / 2), y - (h / 2)); // Bottom Left
		AddCoord(x + (w / 2), y - (h / 2)); // Bottom Right

		this.indices.push_back(2);
		this.indices.push_back(0);
		this.indices.push_back(1);

		this.indices.push_back(1);
		this.indices.push_back(3);
		this.indices.push_back(2);
	}
};
