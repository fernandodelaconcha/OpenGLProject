#pragma once

#include <iostream>
#include <vector>
#include "../Header Files/Shader.h"
#include "../Header Files/Texture.h"
#include "../Header Files/Material.h"
#include "../Header Files/Vertex.h"
#include "../Header Files/Primitives.h"
#include <glm/ext/matrix_float4x4.hpp>

class Mesh
{
private:
	unsigned numberOfVertices;
	unsigned numberOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO(Primitive primitive)
	{
		this->numberOfVertices = primitive.getNumberOfVertices();
		this->numberOfIndices = primitive.getNumberOfIndices();

		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->numberOfVertices * sizeof(Vertex), primitive.getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numberOfIndices * sizeof(GLuint), primitive.getIndices(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
	};
	void initVAO(Vertex* vertexArray, const unsigned& numberOfVertices, GLuint* indexArray, const unsigned& numberOfIndices)
	{
		this->numberOfVertices = numberOfVertices;
		this->numberOfIndices = numberOfIndices;

		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->numberOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numberOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
	};
	void updateUniforms(Shader* shader)
	{
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	};
	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	};
public:
	Mesh(Primitive primitive, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(primitive);
		this->updateModelMatrix();
	};
	Mesh(Vertex* vertexArray, const unsigned& numberOfVertices, GLuint* indexArray, const unsigned& numberOfIndices, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(vertexArray, numberOfVertices, indexArray, numberOfIndices);
		this->updateModelMatrix();
	};
	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	};

	void setPosition(glm::vec3 position)
	{
		this->position = position;
	};
	void setRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
	};
	void setScale(glm::vec3 scale)
	{
		this->scale = scale;
	};

	void move(glm::vec3 position)
	{
		this->position += position;
	};
	void rotate(glm::vec3 rotation)
	{
		this->rotation += rotation;
	};
	void scaleUp(glm::vec3 scale)
	{
		this->scale += scale;
	};
	
	void update()
	{

	};
	void render(Shader* shader)
	{
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		glBindVertexArray(this->VAO);

		if (this->numberOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, this->numberOfVertices);
		else
			glDrawElements(GL_TRIANGLES, this->numberOfIndices, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	};
};