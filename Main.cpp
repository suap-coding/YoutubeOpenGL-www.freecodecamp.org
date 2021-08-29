// some important stuff to remember:
// - all open glf objects (including shader objects) - are accessible by reference

#include <iostream>
#include <glad/glad.h>	// library we use to allow the use of OpenGL
#include <GLFW/glfw3.h>	// library we use to create window

// Vertex Shader source code
// it prepares mathematicaly perfect triangle
const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// Fragment Shader source code
// it pixelises the triangle
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
	"}\n\0";

int main()
{
	glfwInit();

	// hints for GLFW - it doesn't know what we're doing :)
	// we're saying GLFW that we're going to be using OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// below we chose the OpenGL Profile - core or compatibility
	// and we load the set of functions of the chosen type
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// we make sure the float is actualy the size we need
	// and we create a triangle as an array of floats
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3 , 0.0f
	};

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


	// create REFERENCE VALUE for a Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(
		vertexShader,		// connect the REFERENCE VALUE
		1,					// using 1 string
		&vertexShaderSource,// with the reference of our string
		NULL				// something...
	);
	// compile the shader referenced by the given value
	// into MACHINE CODE, because GPU doesn't understand the source code
	glCompileShader(vertexShader);

	// same for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// wrapper around the shaders - make a shader program program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link the program we created to opengl? unsure.
	glLinkProgram(shaderProgram);

	// clear stuff
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// time to send stuff over from CPU to GPU - the triangle.
	// it's a slow thing, so we need to send it in
	// BIIIG BATCH, to do it once and be done with
	// that's probably why we get the loading screens... :P
	// u call this kind of batch "A BUFFER", a buffer (object).
	// VBO is actualy (normaly?) an array of references,
	// but we need to send only 1 reference - to the program.
	// (so this time the batch is not that big.)

	GLuint VAO, VBO;	// ArrayObject, BufferObject
	// VAO stores pointers to one ore more VBO's
	// we need VAO's to tell OpenGL about our VBO
	// or change between VBO's

	// ACHTUNG !!! The order of CREATING VAO and VBO
	// MATTERS! is VERY important!
	// make sure u generate VAO before VBO!
	glGenVertexArrays(1, &VAO); // 1 object, point to VAO reff

	// we now CREATE the buffer object
	glGenBuffers(
		1,		// we send 1 3D object.
		&VBO	// point to the refference we created
	);

	// BINDING IN OpenGL
	// we make a chosen, certain object
	// become THE CURRENT OBJECT.
	// whenever we run a function that modifies THAT TYPE of object,
	// it straight away modify THE CURRENT OBJECT (of that type)!
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// there are other buffers than GL_ARRAY_BUFFER
	// - u can read about them in the documentation

	// now let's STORE our vertices in the VBO.
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW // STATIC, STREAM, DYNAMIC _DRAW, _READ, _COPY
	);
	// STREAM -  modified once, used a few times
	// STATIC -  modified once, used MANY MANY times
	// DYNAMIC - modiefied multiple times, used MANY MANY times
	// _DRAW - modified and used to draw an image

	// configure VAO, so OpenGL knows how to read VBO, with...
	glVertexAttribPointer(
		0,						// position of Vertex Attribute
		3,						// how many values PER vertex
		GL_FLOAT,				// value type
		GL_FALSE,				// this only matters if we use integers
		3 * sizeof(GL_FLOAT),	// stride - distance between vertices
		(void*)0				// offset - how far from the begining our data (vertices) begin.
	);
	glEnableVertexAttribArray(0);

	// not mandatory, but
	// UNBIND the VertexBuffer and VertexArray
	// to not accidently fuck them up.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// !! the buffers we're talking about below
	// are NOT the batches we described above !!
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
		// the actual fuckin redrawing
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	// navy blue, full opacity
		glClear(GL_COLOR_BUFFER_BIT);				// execute program on back buffer
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);		// we have only one VAO, but we need to get used to this
		glDrawArrays(
			GL_TRIANGLES,	// primitive to use
			0,				// starting index of vertices
			3				// # of vertices
		);
		glfwSwapBuffers(window);

		// we need to tell GLFW to process the events
		// or the window freezes in nonresponsive state
		glfwPollEvents();
	}


	// cleanup before closing
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
