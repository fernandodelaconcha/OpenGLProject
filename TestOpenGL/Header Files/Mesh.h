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
	Vertex* vertexArray;
	unsigned numberOfVertices;
	GLuint* indexArray;
	unsigned numberOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 origin;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO()
	{
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->numberOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		if (this->numberOfIndices > 0)
		{
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numberOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

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
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	};
public:
	Mesh(Primitive primitive, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->rotation = rotation;
		this->origin = origin;
		this->scale = scale;

		this->numberOfVertices = primitive.getNumberOfVertices();
		this->numberOfIndices = primitive.getNumberOfIndices();
		this->vertexArray = new Vertex[this->numberOfVertices];
		for (size_t i = 0; i < this->numberOfVertices; i++)
		{
			this->vertexArray[i] = primitive.getVertices()[i];
		}

		this->indexArray = new GLuint[this->numberOfIndices];
		for (size_t i = 0; i < this->numberOfIndices; i++)
		{
			this->indexArray[i] = primitive.getIndices()[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	};
	Mesh(Vertex* vertexArray, const unsigned& numberOfVertices, GLuint* indexArray, const unsigned& numberOfIndices, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->rotation = rotation;
		this->origin = origin;
		this->scale = scale;

		this->numberOfVertices = numberOfVertices;
		this->numberOfIndices = numberOfIndices;
		this->vertexArray = new Vertex[this->numberOfVertices];
		for (size_t i = 0; i < numberOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->numberOfIndices];
		for (size_t i = 0; i < numberOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	};
	Mesh(const Mesh& obj)
	{
		this->position = obj.position;
		this->rotation = obj.rotation;
		this->origin = obj.origin;
		this->scale = obj.scale;

		this->numberOfVertices = obj.numberOfVertices;
		this->numberOfIndices = obj.numberOfIndices;
		this->vertexArray = new Vertex[this->numberOfVertices];
		for (size_t i = 0; i < this->numberOfVertices; i++)
		{
			this->vertexArray[i] = obj.vertexArray[i];
		}

		this->indexArray = new GLuint[this->numberOfIndices];
		for (size_t i = 0; i < this->numberOfIndices; i++)
		{
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}
	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);

		if (this->numberOfIndices > 0)
		{
			glDeleteBuffers(1, &this->EBO);
		}

		delete[] this->vertexArray;
		delete[] this->indexArray;
	};

	void setPosition(glm::vec3 position)
	{
		this->position = position;
	};
	void setRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
	};
	void setOrigin(glm::vec3 origin)
	{
		this->origin = origin;
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