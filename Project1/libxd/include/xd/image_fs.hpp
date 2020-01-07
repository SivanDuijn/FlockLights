#ifndef IMAGE_FS_HPP
#define IMAGE_FS_HPP

#include <string>

const std::string image_fs =
"#version 330 core\n"
"\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uTintColor;\n"
"uniform int uDoTint;\n"
"\n"
"in vec2 vUV;\n"
"out vec4 fragColor;\n"
"\n"
"void main(void) {\n"
"    fragColor = texture(uTexture, vUV);\n"
"    if (uDoTint == 1) {\n"
"        float grayness = 0.299 * fragColor.r + 0.587 * fragColor.g + 0.114 * fragColor.b; // (fragColor.r + fragColor.g + fragColor.b) / 3.0;\n"
"        vec4 grayscale = vec4(grayness, grayness, grayness, 1.0);\n"
"        fragColor = fragColor.a > 0 ? grayscale * uTintColor : fragColor;\n"
"    }\n"
"}\n"
;

#endif /* IMAGE_FS_HPP */
