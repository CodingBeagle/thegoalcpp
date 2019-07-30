// It's important to include GLAD before GLFW.
// GLAD contains the correct OpenGL header includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

#include "Shader.hpp"

// OpenGL Debug Output Message callback
void APIENTRY openGlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

// GLFW key callback
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// GLFWWindow is fully defined, thus we need to create our own custom deleter
// When creating a smart pointer for the GLFWWindow type.;
struct DestroyGlfwWindow {
	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}
};

// Vertex Data
float vertices[] = {
	// Positions        // Colors
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom left
	-0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f // top left
};

// Cube Vertex Data
float verticesCube[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.2f,
	-0.5f,  0.5f, -0.5f, 0.7f, 0.9f, 0.5f,
	-0.5f, -0.5f, -0.5f, 0.2f, 0.3f, 0.1f,

	-0.5f, -0.5f,  0.5f, 0.1f, 0.3f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.4f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.3f, 0.3f, 0.3f,
	-0.5f,  0.5f,  0.5f, 0.1f, 0.1f, 0.1f,
	-0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f,

	-0.5f,  0.5f,  0.5f, 0.1f, 1.0f, 0.1f,
	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.2f,
	-0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 0.1f, 0.2f, 0.9f,
	 0.5f,  0.5f, -0.5f, 0.9f, 0.8f, 0.7f,
	 0.5f, -0.5f, -0.5f, 0.1f, 1.0f, 0.1f,
	 0.5f, -0.5f, -0.5f, 0.4f, 0.2f, 0.1f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.9f, 0.8f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.1f, 1.0f, 0.1f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.2f, 0.8f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.1f, 0.2f, 0.3f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.1f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.1f, 0.1f, 1.0f,

	-0.5f,  0.5f, -0.5f, 0.1f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.1f, 0.5f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.2f,
	-0.5f,  0.5f,  0.5f, 0.5f, 0.2f, 0.1f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.5f
};

int indices[] = {
	0, 1, 3, // First triangle
	1, 2, 3  // Second triangle
};

int main()
{
	// Initialize GLFW
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable an OpenGL Debugging context so that we can use the
	// OpenGL Debug Output feature
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	std::unique_ptr<GLFWwindow, DestroyGlfwWindow> 
		mainWindow(glfwCreateWindow(800, 600, "The Goal.", nullptr, nullptr));

	if (mainWindow == nullptr) {
		std::cout << "Failed to create application window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(mainWindow.get(), glfwKeyCallback);

	glfwMakeContextCurrent(mainWindow.get());

	// Center window
	int windowWidth = 0;
	int windowHeight = 0;
	glfwGetWindowSize(mainWindow.get(), &windowWidth, &windowHeight);

	const std::unique_ptr<const GLFWvidmode> 
		glfwVideoMode(glfwGetVideoMode(glfwGetPrimaryMonitor()));

	glfwSetWindowPos(
		mainWindow.get(),
		(glfwVideoMode->width - windowWidth) / 2,
		(glfwVideoMode->height - windowHeight) / 2
	);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}

	// The Z-buffer of OpenGL allows OpenGL to decide when to draw over a pixel
	// and when not to, based on depth testing.
	// OpenGL stores all depth information in a z-buffer, known as the "depth buffer".
	// GLFW automatically creates such a buffer for you.
	// However, we have to explicitly enable depth testing.
	// We do that here.
	glEnable(GL_DEPTH_TEST);

	// We register the OpenGL Debug Message callback
	// This feature has been in the OpenGL core profile since 4.3
	// Before that, it's an extension by the name of "KHR_Debug"
	glDebugMessageCallback(openGlErrorCallback, nullptr);

	// OpenGl Core REQUIRES us to use Vertex Array Objects (VAOs)
	// VAOs are OpenGL objects which will save state related to these calls:
	// -- Calls to glEnableVertexAttribArray or glDisableVertexAttribArray
	// -- Vertex attribute configurations via glVertexAttribPointer
	// -- Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// We generate an OpenGL buffer object
	// OpenGL buffers can be used for many things. They are simply allocated memory which can be used
	// to store whatever you want
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Now we bind our generated buffer to the GL_ARRAY_BUFFER target. This essentially means that we will
	// be using it is a vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Now that we have bound our buffer object to a target, we can start to make OpenGL calls to functions
	// That affect the state relevant for that object
	// Here we copy our vertice data to the GPU, to our newly created buffer object.
	// We also hint to OpenGL that the date most likely won't change. This means that OpenGL can make some assumptions
	// about the data which can be used to optimize it.
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	// With Element Buffer Objects, we can give OpenGL a list of indices, describing the order
	// In which triangles should be rendered from the vertices array.
	unsigned int EBO;
	// glGenBuffers(1, &EBO);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// In the vertex shader we specified that location 0 accepted a 3D vector as input
	// OpenGL is very flexible when it comes to how to feed input into that location
	// But that also means we have to describe how the buffer is structured
	// So that OpenGL knows how to take the x, y and z number of each vertex described
	// in our array
	// Parameter 1: The index of the location we want to input to
	// Parameter 2: The number of components per generic vertex attribute
	// -- We have 3 components, since our input is a Vec3 in the vertex shader
	// Parameter 3: The data type of each component.
	// -- They are 32-bit floats
	// Parameter 4: Should data be normalized. Should be FALSE for floats.
	// Parameter 5: The byte offset between each consecutive generic vertex attribute.
	// -- Our array is tightly packed, so 0 byte offset between them
	// Parameter 6: The byte offset of the first component of the first generic vertex
	// Attribute.
	// -- This is 0 for us. It begins at the start of the array.
	// NOTICE: glVertexAttribPointer reads the currently bound buffer in GL_ARRAY_BUFFER
	// and stores it in the VAO, so unbinding the buffer in GL_ARRAY_BUFFER will not affect
	// The currently bound VAO

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// Cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	// Vertex programming
	Shader myShader("shaders/transvertex.glsl", "shaders/fragment.glsl");
	myShader.activate();

	glBindVertexArray(VAO);

	float rotation = 0;

	float radius = 5.0f;

	glClearColor(0.4f, 0.58f, 0.92f, 1.0f);
	while (!glfwWindowShouldClose(mainWindow.get())) {
		// Besides clearing the color buffer, we also want to clear the
		// depth buffer, otherwise depth information from the previous frame stays in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Our model matrix
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));

		// Our view matrix
		float camX = (sin(glfwGetTime()) * radius) + sin(glfwGetTime()) * 3;
		float camZ = (cos(glfwGetTime()) * radius) + sin(glfwGetTime()) * 3;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(
		        glm::vec3(camX, 2.0f, camZ),
		        glm::vec3(0.0f, 0.0f, 0.0f),
		        glm::vec3(0.0f, 1.0f, 0.0f)
		        );

		// Our projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		myShader.setMatrix("model", trans);
		myShader.setMatrix("view", view);
		myShader.setMatrix("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// rotation += 0.5;

		glfwSwapBuffers(mainWindow.get());

		// Processes input events which has already been received and then returns immediately
		// This is the best choice for applications that render continously, like games.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

bool wireframeToggle = false;
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!wireframeToggle)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframeToggle = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframeToggle = false;
		}
	}
}

void APIENTRY openGlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "**** OpenGL Debug Message ****" << std::endl;
	std::cout << "Debug Message: " << message << std::endl;
	std::cout << "Source: " << source << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "******************************" << std::endl;
}