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

GLuint createShaderWithLight(){
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
        uniform float uTextureOffset;
        // output will be interpolated for each fragment
        varying vec3 vColor;
        varying vec2 vTexture1;
        varying vec2 vTexture2;
        varying vec3 vPosWorldspace;
        varying vec3 vNormalCameraspace;
        varying vec3 vEyeDirCameraspace;
        varying vec3 vLightDirCameraspace;


        void main () {
            vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
            // вычисляем позицию точки в пространстве OpenGL
            gl_Position = uModelViewProjMat * vertexVec4;
            // Position of the vertex, in worldspace : M * position
            vPosWorldspace = (uM * vertexVec4).xyz;

            // Vector that goes from the vertex to the camera, in the camera space
            // In camera space, the camera is at the origin (0, 0, 0)
            vec3 vertexPosCameraspace = (uV * uM * vertexVec4).xyz;
            vEyeDirCameraspace = vec3(0,0,0) - vertexPosCameraspace;

            // Vector that goes from the vertex to the light, in camera space.
            vec3 LightPosCameraspace = (uV * vec4 (uLightPosition, 1.0)).xyz;
            vLightDirCameraspace = LightPosCameraspace + vEyeDirCameraspace;

            // Normal of the vertex, in camera space. only correct if model matrix doesn't scale object
            vNormalCameraspace = ( uV * uM * vec4(aNormal, 0)).xyz;
            // цвет и текстурные координаты просто пробрасываем для интерполяции
            vColor = aColor;

            vTexture1 = vec2(aTexture.x+uTextureOffset/5, aTexture.y);
            vTexture2 = vec2(aTexture.x+uTextureOffset/1, aTexture.y);
        }
    );

        // Fragment Shader
    const char* fragmentShader = STRINGIFY_SHADER(
        varying vec3 vColor;
        varying vec2 vTexture1;
        varying vec2 vTexture2;
        varying vec3 vPosWorldspace;
        varying vec3 vNormalCameraspace;
        varying vec3 vEyeDirCameraspace;
        varying vec3 vLightDirCameraspace;

        //  uniforms
        uniform sampler2D Texture0;
        uniform sampler2D Texture1;
        uniform vec3 uLightColor;
//        uniform mat4 uMV;
        uniform float uLightPower;
        uniform vec3 uLightPosition;

        void main () {

            // Material properties
            vec3 MaterialDiffuseColor = mix(texture2D(Texture0, vTexture1), texture2D(Texture1, vTexture2), 0.5).rgb;
            vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;

            // Distance to the light
            float distance = length(uLightPosition - vPosWorldspace);

            // Normal of the computed fragment, in camera space
            vec3 n = normalize(vNormalCameraspace);
            // Direction of the light ( from fragment to the light
            vec3 l = normalize(vLightDirCameraspace);
            // Cos of the angle betweev the normal and the light of direction
            // clamped above 0
            // - light if at the vertical of the triangle -> 1
            // - light is perpendicular to the triangle -> 0
            // - light is behind the triangle -> 0
            float cosTheta = clamp( dot(n, l), 0, 1);
//            vec4 color = texture2D(uTexture, vTexture1) * vec4 (vColor, 1.0);
//            if (color.a < 0.1){
//                discard;
//            }

            // eye vector (towards the camera)
            vec3 eye = normalize(vEyeDirCameraspace);
            // Direcrion in wich the triangle reflects the light
            vec3 reflected = reflect(-l, n);
            // cosine bestween eye vector and the reflect vector clamped to 0
            // - Looking into the reflection -> 1
            // - Looking elsewhere -> <1
            float cosAlpha = clamp(dot(eye, reflected), 0, 1);


            gl_FragColor = vec4(
                        // ambient
                        MaterialAmbientColor +
                        // diffuse
                        MaterialDiffuseColor * uLightColor * uLightPower * cosTheta
                    / (distance * distance) +
                        //specular
                        MaterialDiffuseColor * uLightColor * uLightPower * pow(cosAlpha, 10)/ (distance* distance)
                                , 1.0);
//            gl_FragColor = texture2D(uTexture0, vTexture1)+texture2D(uTexture1, vTexture1);
        }
    );




    GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
    CHECK_GL_ERRORS();
    return shader;
}

GLuint createShaderLightOneTexture(){
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
        uniform float uTextureOffset;
        // output will be interpolated for each fragment
        varying vec3 vColor;
        varying vec2 vTexture1;
        varying vec2 vTexture2;
        varying vec3 vPosWorldspace;
        varying vec3 vNormalCameraspace;
        varying vec3 vEyeDirCameraspace;
        varying vec3 vLightDirCameraspace;


        void main () {
            vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
            // вычисляем позицию точки в пространстве OpenGL
            gl_Position = uModelViewProjMat * vertexVec4;
            // Position of the vertex, in worldspace : M * position
            vPosWorldspace = (uM * vertexVec4).xyz;

            // Vector that goes from the vertex to the camera, in the camera space
            // In camera space, the camera is at the origin (0, 0, 0)
            vec3 vertexPosCameraspace = (uV * uM * vertexVec4).xyz;
            vEyeDirCameraspace = vec3(0,0,0) - vertexPosCameraspace;

            // Vector that goes from the vertex to the light, in camera space.
            vec3 LightPosCameraspace = (uV * vec4 (uLightPosition, 1.0)).xyz;
            vLightDirCameraspace = LightPosCameraspace + vEyeDirCameraspace;

            // Normal of the vertex, in camera space. only correct if model matrix doesn't scale object
            vNormalCameraspace = ( uV * uM * vec4(aNormal, 0)).xyz;
            // цвет и текстурные координаты просто пробрасываем для интерполяции
            vColor = aColor;

            vTexture1 = vec2(aTexture.x+uTextureOffset/5, aTexture.y);
            vTexture2 = vec2(aTexture.x+uTextureOffset/1, aTexture.y);
        }
    );

        // Fragment Shader
    const char* fragmentShader = STRINGIFY_SHADER(
        varying vec3 vColor;
        varying vec2 vTexture1;
        varying vec2 vTexture2;
        varying vec3 vPosWorldspace;
        varying vec3 vNormalCameraspace;
        varying vec3 vEyeDirCameraspace;
        varying vec3 vLightDirCameraspace;

        //  uniforms
        uniform sampler2D Texture0;
        uniform sampler2D Texture1;
        uniform vec3 uLightColor;
//        uniform mat4 uMV;
        uniform float uLightPower;
        uniform vec3 uLightPosition;

        void main () {

            // Material properties
            vec3 MaterialDiffuseColor = mix(texture2D(Texture0, vTexture1), texture2D(Texture1, vTexture2), 0.5).rgb;
            vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;

            // Distance to the light
            float distance = length(uLightPosition - vPosWorldspace);

            // Normal of the computed fragment, in camera space
            vec3 n = normalize(vNormalCameraspace);
            // Direction of the light ( from fragment to the light
            vec3 l = normalize(vLightDirCameraspace);
            // Cos of the angle betweev the normal and the light of direction
            // clamped above 0
            // - light if at the vertical of the triangle -> 1
            // - light is perpendicular to the triangle -> 0
            // - light is behind the triangle -> 0
            float cosTheta = clamp( dot(n, l), 0, 1);
//            vec4 color = texture2D(uTexture, vTexture1) * vec4 (vColor, 1.0);
//            if (color.a < 0.1){
//                discard;
//            }

            // eye vector (towards the camera)
            vec3 eye = normalize(vEyeDirCameraspace);
            // Direcrion in wich the triangle reflects the light
            vec3 reflected = reflect(-l, n);
            // cosine bestween eye vector and the reflect vector clamped to 0
            // - Looking into the reflection -> 1
            // - Looking elsewhere -> <1
            float cosAlpha = clamp(dot(eye, reflected), 0, 1);


            gl_FragColor = vec4(
                        // ambient
                        MaterialAmbientColor +
                        // diffuse
                        MaterialDiffuseColor * uLightColor * uLightPower * cosTheta
                    / (distance * distance) +
                        //specular
                        MaterialDiffuseColor * uLightColor * uLightPower * pow(cosAlpha, 10)/ (distance* distance)
                                , 1.0);
//            gl_FragColor = texture2D(uTexture0, vTexture1)+texture2D(uTexture1, vTexture1);
        }
    );




    GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
    CHECK_GL_ERRORS();
    return shader;
}

GLuint createShaderNoLight(){
//         shader code for Sun surface
        // Шейдер вершин
        const char* vertexShader = STRINGIFY_SHADER(
            // vertex attribute
            attribute vec3 aPos;
            attribute vec3 aColor;
            attribute vec2 aTexture;
            // uniforms
            uniform mat4 uModelViewProjMat;
            // output
            varying vec3 vColor;
            varying vec2 vTexture;

            void main () {
                vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
                // вычисляем позицию точки в пространстве OpenGL
                gl_Position = uModelViewProjMat * vertexVec4;
                // цвет и текстурные координаты просто пробрасываем для интерполяции
                vColor = aColor;
                vTexture = vec2(aTexture.x, aTexture.y);
            }
        );
        const char* fragmentShader = STRINGIFY_SHADER(
            varying vec3 vColor;
            varying vec2 vTexture;

            uniform sampler2D uTexture;

            void main () {

    //            vec4 color = texture2D(uTexture, vTexture) * vec4 (vColor, 1.0);
    //            if (color.a < 0.1){
    //                discard;
    //            }
                gl_FragColor = texture2D(uTexture, vTexture);        }
        );
    GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
    CHECK_GL_ERRORS();
    return shader;
}

GLuint createShaderNoTexture(){
    // Шейдер вершин
    const char* vertexShader = STRINGIFY_SHADER(
        // vertex attribute
        attribute vec3 aPos;
        // uniforms
        uniform mat4 uModelViewProjMat;
        // output
        varying vec3 vColor;

        void main () {
            vec4 vertexVec4 = vec4(aPos, 1.0);      // последняя компонента 1, тк это точка
            // вычисляем позицию точки в пространстве OpenGL
            gl_Position = uModelViewProjMat * vertexVec4;
            // цвет и текстурные координаты просто пробрасываем для интерполяции
        }
    );
    const char* fragmentShader = STRINGIFY_SHADER(
        uniform vec3 uColor;

        void main () {

//            vec4 color = texture2D(uTexture, vTexture) * vec4 (vColor, 1.0);
//            if (color.a < 0.1){
//                discard;
//            }
            gl_FragColor = vec4(uColor, 1.0);        }
    );
GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
CHECK_GL_ERRORS();
return shader;
}

