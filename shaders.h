#pragma once
#include <string>

// shaders declarations
const char* const vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* const fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

//char* the_string;
//I can change which char the_string points to,
//and I can modify the char to which it points.

//const char* the_string;
//I can change which char the_string points to,
//but I cannot modify the char to which it points.

//char* const the_string;
//I cannot change which char the_string points to,
//but I can modify the char to which it points.

//const char* const the_string;
//I cannot change which char the_string points to,
//nor can I modify the char to which it points.