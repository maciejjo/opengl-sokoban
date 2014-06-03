#ifndef SHADERS_UTILS_H
#define SHADERS_UTILS_H

#include "opengl_includes.h"

char *load_file(const char *filename);
void print_log(GLuint object);
GLuint create_shader(const char *filename, GLenum type);

#endif
