#ifndef HELPERS
#define HELPERS

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>        // для поддержки расширений, шейдеров и так далее

// вычисление смещения в структуре/классе
#define OFFSETOF(TYPE, FIELD) ((void*)&(((TYPE*)NULL)->FIELD))
// Превращаем текущий текст в строку шейдера
#define STRINGIFY_SHADER(TEXT) ("#version 120\n "#TEXT)
// проверка ошибок
#define CHECK_GL_ERRORS() checkOpenGLerror(__FILE__, __LINE__)

typedef unsigned int uint;

int checkOpenGLerror(const char *file, int line);
void glDebugOut(uint source, uint type, uint id, uint severity, int length, const char* message, void* userParam);



#endif
