#ifndef FONT_VS_HPP
#define FONT_VS_HPP

#include <string>

const std::string font_vs =
"#version 330 core\n"
"\n"
"uniform mat4 uMVP;\n"
"\n"
"layout(location = 0) in vec2 aPosition;\n"
"layout(location = 1) in vec2 aUV;\n"
"\n"
"out vec2 vUV;\n"
"\n"
"void main() {\n"
"    vUV = aUV;\n"
"    gl_Position = uMVP * vec4(aPosition, 0.0, 1.0);\n"
"}\n"
;

#endif /* FONT_VS_HPP */
