#pragma once
#include <vector>
#include <iterator>

class GameObject {
public:
	GLfloat x, y, z;
	GLuint vertBuffer, vertArray, elementBuffer;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	GameObject() {}
	GameObject(std::vector<GLfloat> vertices, std::vector<GLuint> indices) {
		this->vertices = vertices;
		this->indices = indices;
	}

	void BindBuffers() {
		glGenVertexArrays(1, &vertArray);
		glGenBuffers(1, &vertBuffer);
		glGenBuffers(1, &elementBuffer);

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
		BindBuffers();
		glBindVertexArray(vertArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // GL_TRIANGLES || GL_LINES
		glBindVertexArray(0);
		// unbind texture here
	}

	// TODO implement transformations
	void Move(GLfloat x, GLfloat y, GLfloat z) { }
	void Rotate(GLfloat x, GLfloat y, GLfloat z) { }
	void Scale(GLfloat x, GLfloat y, GLfloat z) { }
};
