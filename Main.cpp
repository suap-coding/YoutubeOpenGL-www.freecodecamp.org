#include <iostream>
#include <glad/glad.h>	// library we use to allow the use of OpenGL
#include <GLFW/glfw3.h>	// library we use to create window

int main()
{
	glfwInit();

	// hints for GLFW - it doesn't know what we're doing :)
	// we're saying GLFW that we're going to be using OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// below we chose the OpenGL Profile - core or compatibility
	// and we load the set of functions of the chosen type
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window (window object)
	GLFWwindow* window = glfwCreateWindow(
		800,	// height
		800,	// width
		"YoutubeOpenGL",	// name
		NULL,	// which screen? = want fullscreen?; null = not fullscreen
		NULL	// not important (for now?)
	);

	// check if the object was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// we're making our window a part of the CURRENT CONTEXT
	// (context is SORT OF object that holds whole of OpenGL, but not really.
	//  we're leave it at that for now.)
	glfwMakeContextCurrent(window);

	// tell GLAD to load all required configurations for OpenGL
	gladLoadGL();

	// tell OpenGL where to render
	glViewport(
		0,   0,		// bottom left corner
		800, 800	// top right corner
	);

	// there are 2 buffers - front and back
	// front - what's currently shown on the screen
	// back - what's being prepared to be shown
	// while front is shown, back is rewritten,
	// then back is shown - becomes front and
	// front becomes back and is rewritten

	// prepare to clear a (back) buffer and give it a nice color
	glClearColor(
		0.07f, 0.13f, 0.17f,	// color - navy blue :)
		1.0f					// opacity - 1 full opaque, 0 full transparent
	);
	// execute the command we prepared on a current (back) buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// swap the buffers!
	glfwSwapBuffers(window);	

	// main while loop - where all rendering will happen
	// while the close button have NOT been pressed
	while (!glfwWindowShouldClose(window))
	{
		// we need to tell GLFW to process the events
		// or the window freezes in nonresponsive state
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
