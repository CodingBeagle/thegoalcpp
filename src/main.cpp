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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::unique_ptr<GLFWwindow, DestroyGlfwWindow> 
		mainWindow(glfwCreateWindow(800, 600, "The Goal.", nullptr, nullptr));

	if (mainWindow == nullptr) {
		std::cout << "Failed to create application window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mainWindow.get());

	// Center window
	int windowWidth = 0;
	int windowHeight = 0;
	glfwGetWindowSize(mainWindow.get(), &windowWidth, &windowHeight);

	std::unique_ptr<const GLFWvidmode> 
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

	glViewport(0, 0, 800, 600);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// With Element Buffer Objects, we can give OpenGL a list of indices, describing the order
	// In which triangles should be rendered from the vertices array.
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	myShader.setMatrix("transform", trans);

	glClearColor(0.4f, 0.58f, 0.92f, 1.0f);
	while (!glfwWindowShouldClose(mainWindow.get())) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(mainWindow.get());
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}