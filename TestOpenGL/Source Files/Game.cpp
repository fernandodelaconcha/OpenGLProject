#include "../Header Files/Game.h"


void Game::initGLFW()
{
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "ERROR::GAME::GLFW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Game::initWindow(const char* title, const bool resizable)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, nullptr, nullptr);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GAME::GLFW_WINDOW_INIT_ERROR" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::frameBufferResizeCallback);
	glfwMakeContextCurrent(this->window);
}

void Game::initGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR::GAME::GLEW_INIT_FAILED" << std::endl;
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane

	);
}

Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
	:WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR)
{
	this->window = nullptr;
	this->frameBufferWidth = this->WINDOW_WIDTH;
	this->frameBufferHeight = this->WINDOW_HEIGHT;

	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);
	this->camPosition = glm::vec3(0.f, 0.f, 1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

}
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}
void Game::update()
{
	glfwPollEvents();
}
void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Game::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}