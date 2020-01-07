#ifndef IMAGE_VS_HPP
#define IMAGE_VS_HPP

#include <string>

const std::string image_vs =
"#version 330 core\n"
"\n"
"uniform mat4 uMVP;\n"
"uniform mat4 uTextureMatrix;\n"
"\n"
"layout(location = 0) in vec2 aPosition;\n"
"layout(location = 1) in vec2 aUV;\n"
"\n"
"out vec2 vUV;\n"
"\n"
"void main() {\n"
"    vUV = vec2(uTextureMatrix * vec4(aUV, 0.0, 1.0));\n"
"    gl_Position = uMVP * vec4(aPosition, 0.0, 1.0);\n"
"}\n"
;

#endif /* IMAGE_VS_HPP */
