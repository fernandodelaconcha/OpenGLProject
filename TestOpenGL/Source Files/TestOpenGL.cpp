// TestOpenGL.cpp : Defines the entry point for the application.
//

#include "../Header Files/TestOpenGL.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <vector>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <SOIL2.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Header Files/Mesh.h"
#include "../Header Files/Primitives.h"

static bool initializeGLFW()
{
	return glfwInit();
}

static bool initializeGLEW()
{
	return glewInit() == GLEW_OK;
}

void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void handleExit(GLFWwindow* window, int key, int status, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void updateInput(GLFWwindow* window, Mesh &mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.001f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.001f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(-0.001f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.001f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, -0.01f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, 0.01f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(0.001f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(-0.001f));
	}
}

GLFWwindow* createWindow(const char * title, const int width, const int height, int& fbWidth, int& fbHeight, int GLMajorVersion, int GLMinorVersion, bool resizable)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glfwMakeContextCurrent(window);

	return window;
}

int main()
{
	if (!initializeGLFW()) {
		std::cerr << "glfw initialization failed." << std::endl;
		return -1;
	}
	const int GLMajorVersion = 4;
	const int GLMinorVersion = 5;
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	int frameBufferWidth = WINDOW_WIDTH;
	int frameBufferHeight = WINDOW_HEIGHT;

	GLFWwindow* window = createWindow("Super videogame", WINDOW_WIDTH, WINDOW_HEIGHT, frameBufferWidth, frameBufferHeight, GLMajorVersion, GLMinorVersion, true);

	if (window == nullptr) {
		std::cerr << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return -2;
	}
	glfwSetKeyCallback(window, handleExit);
	glewExperimental = GL_TRUE;
	if (!initializeGLEW()) {
		std::cerr << "Glew initialization failed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	// OPEN GL OPTIONS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader core_program(GLMajorVersion, GLMinorVersion, "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Resource Files/vertex_core.glsl", "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Resource Files/fragment_core.glsl");
	

	//textures
	Texture texture1 = Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/cat1.png", GL_TEXTURE_2D, 0);
	Texture texture2 = Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/chest.png", GL_TEXTURE_2D, 1);

	//material
	Material material1(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture1.getTextureUnit(), texture2.getTextureUnit());

	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 camPosition = glm::vec3(0.f, 0.f, 1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(frameBufferWidth) / frameBufferHeight,
		nearPlane,
		farPlane
	);

	glm::vec3 lightPos0 = glm::vec3(0.f, 0.f, 1.f);

	Quad tempQuad = Quad();
	Mesh test(tempQuad, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f));
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		updateInput(window, test);

		core_program.setMat4fv(ViewMatrix, "ViewMatrix");
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		core_program.setVec3f(lightPos0, "lightPos");
		core_program.setVec3f(camPosition, "cameraPos");

		material1.sendToShader(core_program);

		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(frameBufferWidth) / frameBufferHeight,
			nearPlane,
			farPlane
		);

		core_program.use();

		texture1.bind();
		texture2.bind();

		test.render(&core_program);

		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
