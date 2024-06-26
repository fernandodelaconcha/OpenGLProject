#include "../Header Files/Game.h"

enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_CAT, TEX_CAT_SPECULAR, TEX_CHEST, TEX_CHEST_SPECULAR };
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Resource Files/vertex_core.glsl", "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Resource Files/fragment_core.glsl"));
}

void Game::initTextures()
{
	this->textures.push_back(new Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/cat1.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/cat1_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/chest.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/Images/chest_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void Game::initModels()
{
	std::vector<Mesh*> meshes;
	meshes.push_back(new Mesh(Quad(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-90.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(100.f)));
	this->models.push_back(new Model(glm::vec3(2.f, -5.f, 2.f), this->materials[material_enum::MAT_1], this->textures[TEX_CHEST], this->textures[TEX_CHEST_SPECULAR], meshes));
	this->models.push_back(new Model(glm::vec3(0.f, 1.f, 0.f), this->materials[material_enum::MAT_1], this->textures[TEX_CAT], this->textures[TEX_CAT_SPECULAR], "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/OBJ Files/axe.obj"));
	this->models.push_back(new Model(glm::vec3(2.f, 0.f, 2.f), this->materials[material_enum::MAT_1], this->textures[TEX_CAT], this->textures[TEX_CAT_SPECULAR], "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/OBJ Files/shield.obj"));
	this->models.push_back(new Model(glm::vec3(4.f, 0.f, 4.f), this->materials[material_enum::MAT_1], this->textures[TEX_CAT], this->textures[TEX_CAT_SPECULAR], "C:/Users/ferna/OneDrive/Desktop/TestOpenGL/TestOpenGL/TestOpenGL/OBJ Files/bomb.obj"));

	for (auto*& i : meshes)
	{
		delete i;
	}
}

void Game::initPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f)));
}

void Game::initLights()
{
	this->initPointLights();
}

void Game::initUniforms()
{
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	for (PointLight * pl: this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
}

void Game::updateUniforms()
{
	this->ViewMatrix = this->camera.getViewMatrix();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	for (PointLight* pl : this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane
	);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
	:WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR), camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
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

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
	{
		delete this->shaders[i];
	}
	for (size_t i = 0; i < this->textures.size(); i++)
	{
		delete this->textures[i];
	}
	for (size_t i = 0; i < this->materials.size(); i++)
	{
		delete this->materials[i];
	}
	for (auto*& i : this->models)
	{
		delete i;
	}
	for (size_t i = 0; i < this->pointLights.size(); i++)
	{
		delete this->pointLights[i];
	}
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		this->pointLights[0]->setPosition(this->camera.getPosition());
	}
}

void Game::updateKeyboardInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->setWindowShouldClose();
	}
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.001f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.001f;
	}
}

void Game::updateInput()
{
	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
	this->updateDt();
	this->updateInput();

	//this->models[0]->rotate(glm::vec3(0.f, 0.05f, 0.f));
	//this->models[1]->rotate(glm::vec3(0.f, 0.05f, 0.05f));
	//this->models[2]->rotate(glm::vec3(0.f, 0.05f, 0.05f));
}

void Game::render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->updateUniforms();

	for (auto* i : this->models)
	{
		i->render(this->shaders[SHADER_CORE_PROGRAM]);
	}

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