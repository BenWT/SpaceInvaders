#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include "Vector3.h"

class GameObject {
private:
	glm::mat4 _trans, _rot, _scale;
	GLuint vertBuffer, vertArray, elementBuffer;
	bool shouldBuffer = true;
public:
	Vector3 position = Vector3(0.0f);
	Vector3 rotation = Vector3(0.0f);
	Vector3 scale = Vector3(1.0f);

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

		_trans = glm::translate(projectionMat * viewMat, glm::vec3(position.x, position.y, position.z));
		_rot = glm::rotate(_trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		_rot = glm::rotate(_rot, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		_rot = glm::rotate(_rot, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		_scale = glm::scale(_rot, glm::vec3(scale.x, scale.y, scale.z));

		GLint transLocation = glGetUniformLocation(shaderProgram, "trans");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(_scale));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vertArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // GL_TRIANGLES || GL_LINES
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
