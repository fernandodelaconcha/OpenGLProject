#pragma once

#include "libs.h"

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

class Game
{
private:
	GLFWwindow* window;
	const char* title;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int frameBufferWidth;
	int frameBufferHeight;

	int GL_VERSION_MAJOR;
	int GL_VERSION_MINOR;

	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
public:
	Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable);
	virtual ~Game();

	int getWindowShouldClose();

	void setWindowShouldClose();

	void update();
	void render();

	static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
};