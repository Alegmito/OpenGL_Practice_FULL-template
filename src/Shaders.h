#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>        // для поддержки расширений, шейдеров и так далее

GLuint createShaderWithLight();

GLuint createShaderNoLight();

GLuint createShaderFromSources(const char* vertexShader, const char* fragmentShader);

GLuint createShaderNoTexture();

GLuint createShaderLightOneTexture();

#endif
