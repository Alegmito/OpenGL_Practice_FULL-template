#include "Shaders.h"
#include "Helpers.h"
#include <stdio.h>
#include <stdlib.h>


GLuint createShaderFromSources(const char* vertexShader, const char* fragmentShader){
    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    CHECK_GL_ERRORS();

    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    CHECK_GL_ERRORS();

    GLuint shaderProgram = glCreateProgram ();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);
    CHECK_GL_ERRORS();

    /*int infologLength = 0;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0){
        GLchar* infoLog = (GLchar*)malloc(static_cast<size_t>(infologLength));
        if (infoLog == nullptr){
            printf("ERROR: Could not allocate InfoLog buffer");
            exit(1);
        }
        int charsWritten = 0;
        glGetShaderInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
        printf("Shader InfoLog:\n%s", infoLog );
        free(infoLog);
    }
    CHECK_GL_ERRORS();*/
    
    return shaderProgram;
}

    // Shader code for Planets

GLuint createShader(){
    // Шейдер вершин
    const char* vertexShader = STRINGIFY_SHADER(
        // vertex attribute
        attribute vec3 aPos;
        attribute vec3 aColor;
        attribute vec2 aTexture;
        attribute vec3 aNormal;
        // uniforms
        uniform mat4 uModelViewProjMat;
        uniform vec3 uLightPosition;
        uniform mat4 uV;
        uniform mat4 uM;
        uniform vec3 uLightPosWorldspace;
        // output will be interpolated for each fragment
        varying vec3 vColor;
        varying vec2 vTexture;
        varying vec3 vPosWorldspace;
        varying vec3 vNormalCameraspace;
        verying vec3 vEyeDirCameraspace;
        varying vec3 vLightDirCameraspace;


        void main () {
            vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
            // вычисляем позицию точки в пространстве OpenGL
            gl_Position = uModelViewProjMat * vertexVec4;
            // Position of the vertex, in worldspace : M * position
            vPosWorldspace = (M * vec4(aPos, 1.0)).xyz;
            // цвет и текстурные координаты просто пробрасываем для интерполяции
            vColor = aColor;
            vTexture = vec2(aTexture.x, aTexture.y);
        }
    );
    const char* fragmentShader = STRINGIFY_SHADER(
        varying vec3 vColor;
        varying vec2 vTexture;

        //  uniforms
        uniform sampler2D uTexture;
        uniform vec3 uLightColor;
        uniform floar uLightPower;

        void main () {

            // light emission properties


//            vec4 color = texture2D(uTexture, vTexture) * vec4 (vColor, 1.0);
//            if (color.a < 0.1){
//                discard;
//            }
            gl_FragColor = texture2D(uTexture, vTexture);        }
    );


    // shader code for Sun surface
//        GLuint createShader(){
//            // Шейдер вершин
//            const char* vertexShader = STRINGIFY_SHADER(
//                // vertex attribute
//                attribute vec3 aPos;
//                attribute vec3 aColor;
//                attribute vec2 aTexture;
//                // uniforms
//                uniform mat4 uModelViewProjMat;
//                // output
//                varying vec3 vColor;
//                varying vec2 vTexture;

//                void main () {
//                    vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
//                    // вычисляем позицию точки в пространстве OpenGL
//                    gl_Position = uModelViewProjMat * vertexVec4;
//                    // цвет и текстурные координаты просто пробрасываем для интерполяции
//                    vColor = aColor;
//                    vTexture = vec2(aTexture.x, aTexture.y);
//                }
//            );
//            const char* fragmentShader = STRINGIFY_SHADER(
//                varying vec3 vColor;
//                varying vec2 vTexture;

//                uniform sampler2D uTexture;

//                void main () {

//        //            vec4 color = texture2D(uTexture, vTexture) * vec4 (vColor, 1.0);
//        //            if (color.a < 0.1){
//        //                discard;
//        //            }
//                    gl_FragColor = texture2D(uTexture, vTexture);        }
//            );

    GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
    CHECK_GL_ERRORS();
    return shader;
}

