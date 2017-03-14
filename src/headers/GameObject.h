#pragma once
#include <iostream>
#include <vector>
#include <iterator>

class GameObject {
public:
	glm::mat4 translation, rotation, scale;
	GLfloat xPos = 0.0f, yPos = 0.0f, zPos = 0.0f;
	GLfloat xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
	GLfloat xScale = 1.0f, yScale = 1.0f, zScale = 1.0f;
	GLuint vertBuffer, vertArray, elementBuffer;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	bool shouldBuffer = true;

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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); // TODO add texture coords
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // TODO add texture coords
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		shouldBuffer = false;
	}

	void Render(GLuint &shaderProgram, glm::mat4 &projectionMat, glm::mat4 &viewMat, GLuint &texture) {
		if (shouldBuffer) BindBuffers();

		translation = glm::translate(projectionMat * viewMat, glm::vec3(xPos, yPos, zPos));
		rotation = glm::rotate(translation, glm::radians(xRot), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(yRot), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(zRot), glm::vec3(0.0f, 0.0f, 1.0f));
		scale = glm::scale(rotation, glm::vec3(xScale, yScale, zScale));

		GLint transLocation = glGetUniformLocation(shaderProgram, "trans");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(scale));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vertArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // GL_TRIANGLES || GL_LINES
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// TODO implement transformations
	void Move(GLfloat x, GLfloat y, GLfloat z) {
		xPos += x;
		yPos += y;
		zPos += z;
	}

	void Rotate(GLfloat x, GLfloat y, GLfloat z) {
		xRot += x;
		yRot += y;
		zRot += z;
	}

	void Scale(GLfloat x, GLfloat y, GLfloat z) {
		xScale += x;
		yScale += y;
		zScale += z;
	}
};
