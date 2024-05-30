#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/vector_relational.hpp>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include<glm/ext/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Vertex.h"

static std::vector<Vertex> loadOBJ(const char* file_name)
{
	std::vector<glm::vec3> vertex_positions;
	std::vector<glm::vec2> vertex_texcoords;
	std::vector<glm::vec3> vertex_normals;

	std::vector<GLint> vertex_positions_indices;
	std::vector<GLint> vertex_texcoords_indices;
	std::vector<GLint> vertex_normals_indices;

	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	if (!in_file.is_open())
	{
		std::cerr << "ERROR::OBJLOADER::Could not open file" << "\n";
	}

	while (std::getline(in_file, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{

		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_glint)
			{
				if (counter == 0)
					vertex_positions_indices.push_back(temp_glint);
				else if (counter == 1)
					vertex_texcoords_indices.push_back(temp_glint);
				else if (counter == 2)
					vertex_normals_indices.push_back(temp_glint);

				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}
	}

	vertices.resize(vertex_positions_indices.size(), Vertex());

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].position = vertex_positions[vertex_positions_indices[i] - 1];
		vertices[i].texcoord = vertex_texcoords[vertex_texcoords_indices[i] - 1];
		vertices[i].normal = vertex_normals[vertex_normals_indices[i] - 1];
		vertices[i].color = glm::vec3(1.f);
	}
	std::cout << "OBJ File Loaded with vertices: " << vertices.size() << "\n";
	return vertices;
}