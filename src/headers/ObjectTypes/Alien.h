#include <string>
#include "Plane.h"

class GameObject : public Plane {
public:
	Plane(int itemIndex, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
		this.itemIndex = itemIndex;
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}

	bool DoMove() {
		// Transform here

		//if (x >= max || x <= -max) return true;
		return false;
	}

	void MoveDown() {
		
	}
};
