#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "..\shaders.h"

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "This is cool!", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(200, 150, 400, 300);

	GLuint vertexS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexS, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexS);
	GLuint fragmentS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentS, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentS);
	GLuint vfs_program = glCreateProgram();
	glAttachShader(vfs_program, fragmentS);
	glAttachShader(vfs_program, vertexS);
	glLinkProgram(vfs_program);
	glDeleteShader(fragmentS);
	glDeleteShader(vertexS);

	GLfloat triangle_v[] = {
		-1.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_v), triangle_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0); // ?
	glEnableVertexAttribArray(0);	// ?	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // moge zostawic pusty pierwszy arg?

	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(vfs_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(vfs_program);
	glfwDestroyWindow(window);
	glfwTerminate();	

	return 0;
}
