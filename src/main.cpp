// It's important to include GLAD before GLFW.
// GLAD contains the correct OpenGL header includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

// GLFWWindow is fully defined, thus we need to create our own custom deleter
// When creating a smart pointer for the GLFWWindow type.

struct DestroyGlfwWindow {
	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}
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

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glClearColor(0.4f, 0.58f, 0.92f, 1.0f);

	while (!glfwWindowShouldClose(mainWindow.get())) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow.get());
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}