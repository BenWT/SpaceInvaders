#include <vector>
#include <iterator>

class GameObject {
public:
	int itemIndex;
	GLfloat x, y, z;
	GLuint vertBuffer, vertArray, elementBuffer;
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	GameObject() {
		this.itemIndex = 0;
	}
	GameObject(int itemIndex, vector<GLfloat> vertices, vector<GLuint> indices) {
		this.itemIndex = itemIndex;
		this.vertices = vertices;
		this.indices = indices;
	}

	void CreateBuffers() {
		glGenVertexArrays(itemIndex, &vertArray);
		glGenBuffers(itemIndex, &vertBuffer);
		glGenBuffers(itemIndex, &elementBuffer);

		glBindVertexArray(vertArray);

		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // TODO add texture coords
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Render() {
		// bind texture here
		glBindVertexArray(vertArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // GL_TRIANGLES || GL_LINES
		glBindVertexArray(0);
		// unbind texture here
	}

	// TODO implement transformations
	void Move() { }
	void Rotate() { }
	void Scale() { }
};
