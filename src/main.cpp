// TODO: надо ли?
// #define GLFW_INCLUDE_GLCOREARB 1 // Tell GLFW to include the OpenGL core profile header
#define GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GL3
#define GLFW_INCLUDE_GLEXT
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <GL/glew.h>        // для поддержки расширений, шейдеров и так далее
#include <GLFW/glfw3.h>     // Непосредственно сам GLFW
#include <glm.hpp>          // библиотека графической математики
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "PngLoader.h"
#include "Helpers.h"
#include "Vertex.h"
#include "Figures.h"
#include "Shaders.h"
#include "ObjLoader.h"

// Документация
// https://www.opengl.org/sdk/docs/man/html/

using namespace std;
using namespace glm;

#define MATH_PI 3.14159265

// Parameters for circle
int numberOfSides = 200;

// Текущие переменные для модели
bool leftButtonPressed = false;
bool rightPressed = false;
bool middlePressed = false;
double lastCursorPosX = 0.0;
double lastCursorPosY = 0.0;
double cursorPosX = 0.0;
double cursorPosY = 0.0;
vec3 lightColor = vec3(1.0, 1.0, 1.0);

float lightPower = 40.0f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 5, 0 );
// Initial horizontal angle : toward -Z
float horizontalAngle = MATH_PI;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;




void glfwErrorCallback(int error, const char* description) {
    printf("OpenGL error = %d\n description = %s\n\n", error, description);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Выходим по нажатию Escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    }
    // по пробелу включаем или выключаем вращение автоматом
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
    }
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
    // обработка левой кнопки
    if(button == GLFW_MOUSE_BUTTON_1){
        if(state == GLFW_PRESS){
            leftButtonPressed = true;
        }else{
            leftButtonPressed = false;
        }
    }
    // обработка правой кнопки
    if(button == GLFW_MOUSE_BUTTON_2){
        if(state == GLFW_PRESS){
            rightPressed = true;
        }else{
            rightPressed = false;
        }
    }
}




void glfwCursorCallback(GLFWwindow* window, double x, double y) {
    // при нажатой левой кнопки - вращаем по X и Y
    if(leftButtonPressed){
        cursorPosX = y * 0.01;
        cursorPosY = x * 0.01;
    }


    // при нажатой левой кнопки - перемещаем по X Y
    if(rightPressed){
    }

    lastCursorPosX = x;
    lastCursorPosY = y;
}

void glfwScrollCallback(GLFWwindow* window, double scrollByX, double scrollByY) {
}

void loadImage (char * path, GLuint* textureId){
    ImageData info = loadPngImage(path);
    if(info.loaded){
        glGenTextures(1, *&textureId);
        glBindTexture(GL_TEXTURE_2D, *textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,              // формат внутри OpenGL
                     info.width, info.height, 0,            // ширинна, высота, границы
                     info.withAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.data); // формат входных данных
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECK_GL_ERRORS();
    }
}

int main(int argc, char *argv[]) {

    // окно
    GLFWwindow* window = 0;

    // обработчик ошибок
    glfwSetErrorCallback(glfwErrorCallback);

    // инициализация GLFW
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }

    // создание окна
#ifdef __APPLE__
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);        // вертикальная синхронизация
    CHECK_GL_ERRORS();

    // Обработка клавиш и прочего
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, glfwCursorCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);

    // инициализация расширений
    glewExperimental = GL_TRUE;
    glewInit();
    CHECK_GL_ERRORS();

    // инициализация расширений
    glewExperimental = GL_TRUE;
    glewInit();
    CHECK_GL_ERRORS();

    // Инициализация отладки
    if(glDebugMessageCallback){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        // Коллбек ошибок OpenGL
        glDebugMessageCallback((GLDEBUGPROC)glDebugOut, 0);

        // Более высокий уровень отладки
        // GLuint unusedIds = 0;
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
    }
    CHECK_GL_ERRORS();

    const unsigned char* version = glGetString(GL_VERSION);
    printf("OpenGL version = %s\n", version);

    // оотношение сторон
    int width = 1024;
    int height = 768;
    // Размер буффера кадра
    glfwGetFramebufferSize(window, &width, &height);
    CHECK_GL_ERRORS();

    // задаем отображение
    glViewport(0, 0, width, height);
    CHECK_GL_ERRORS();

    // Шейдеры
    GLuint shaderProgram = createShaderWithLight();
    GLuint shaderLine = createShaderNoTexture();
    CHECK_GL_ERRORS();

    // Getting access to shaderProgram
    // аттрибуты вершин шейдера
    int posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    int colorAttribLocation = glGetAttribLocation(shaderProgram, "aColor");
    int textureAttribLocation = glGetAttribLocation(shaderProgram, "aTexture");
    int normalAttribLocation = glGetAttribLocation(shaderProgram, "aNormal");
    CHECK_GL_ERRORS();

    // юниформы шейдера
    int modelViewProjMatrixLocation = glGetUniformLocation(shaderProgram, "uModelViewProjMat");
    int modelTextureLocation0 = glGetUniformLocation (shaderProgram, "Texture0");
    int modelTextureLocation1 = glGetUniformLocation (shaderProgram, "Texture1");
    int lightColorLocation = glGetUniformLocation(shaderProgram, "uLightColor");
    int lightPowerLocation = glGetUniformLocation(shaderProgram, "uLightPower");
    int lightPositionLocation = glGetUniformLocation(shaderProgram, "uLightPosition");
    int VLocation = glGetUniformLocation(shaderProgram, "uV");
    int MLocation = glGetUniformLocation(shaderProgram, "uM");
    int textureOffsetLocation = glGetUniformLocation(shaderProgram, "uTextureOffset");
    CHECK_GL_ERRORS();

    //Read obj. file
    std::vector<unsigned short> indices;
    vector <glm::vec3> vertices;
    vector <glm::vec2> uvs;
    std::vector <glm::vec3> normals;



    bool res = loadAssImp ("res/sphere.obj", indices, vertices, uvs, normals);

    // VBO, данные о вершинах
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    CHECK_GL_ERRORS();

    //Texture Buffer
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    //buffer for indices
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);


    // NormalBuffer
    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Get access to shaderLine
    // CirceDrawing
    glm::vec3 orbit[numberOfSides];
    DrawCircle(0, 0, 0, 5, numberOfSides, orbit);
    // Attribute
    int linePosAttribLocation = glGetAttribLocation(shaderLine, "aPos");

    // Shader's uniforms
    int lineModelViewProjMatrixLocation = glGetUniformLocation(shaderLine, "uModelViewProjMat");
    int lineColorLocation = glGetUniformLocation(shaderLine, "uColor");
    CHECK_GL_ERRORS();

    // VBO, данные о вершинах
    GLuint VBOLines = 0;
    glGenBuffers(1, &VBOLines);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLines);
    glBufferData(GL_ARRAY_BUFFER, numberOfSides * sizeof(vec3), &orbit[0], GL_STATIC_DRAW);
    CHECK_GL_ERRORS();


    // отключаем отображение задней части полигонов
    glEnable(GL_CULL_FACE);
    // отбрасываться будут задние грани
    glCullFace(GL_BACK);
    // Определяем, в каком направлении должный обходиться вершины, для передней части (против часовой стрелки?)
    // задняя часть будет отбрасываться
    glFrontFace(GL_CCW);
    CHECK_GL_ERRORS();

    // проверка глубины
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    CHECK_GL_ERRORS();

    // текущее время
//    double time = glfwGetTime();
    GLuint textureID0 = 0;
    GLuint textureID1 = 1;
    // Загрузка текстуры
    loadImage("res/venus_atmosphere.png", &textureID0);

    loadImage("res/moon.png",&textureID1);
    while (!glfwWindowShouldClose(window)){
        // приращение времени
//        double newTime = glfwGetTime();
        //double timeDelta = newTime - time;
//        time = newTime;

        // wipe the drawing surface clear
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram (shaderProgram);
        CHECK_GL_ERRORS();

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, 1024/2, 768/2);

        // Compute new orientation
        horizontalAngle += mouseSpeed * float(1024/2 - xpos );
        verticalAngle   += mouseSpeed * float( 768/2 - ypos );

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        // Up vector
        glm::vec3 up = glm::cross( right, direction );

        // Move forward
        if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }

        // Move backward
        if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
            position.y += deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
            position.y -= deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

        // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        ViewMatrix       = glm::lookAt(
                                    position,           // Camera is here
                                    position + glm::normalize(direction), // and looks here : at the same position, plus "direction"
                                    up                  // Head is up (set to 0,-1,0 to look upside-down)
                               );

        // For the next frame, the "last time" will be "now"
        lastTime = currentTime;

        // change Scale
        static float Scale = 0.0f;
        Scale += 0.01f;
//        glUniform1f(gScaleLocation, sinf(Scale));

        // матрица модель-вид-проекция
        mat4 model = mat4(1.0);
        model = rotate(model, sinf(1/2), vec3(0, 0, 1));
        model = translate(model, vec3(5*sinf(currentTime/100), 0, 5*cosf(currentTime/100)));
//        computeMatricesFromInputs();
//        mat4 proj = getProjectionMatrix();
//        mat4 view = getViewMatrix();
        // Initially, coordinates are between [-1; 1]. But with the perspective matrix we can expand it to much greateter numbers and configure our model as we want
//        model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
//        model = rotate(model, sinf(Scale)/3, vec3(0.0f, 1.0f, 0.0f));
//        model = translate(model, vec3(0.0f, 0.0f, 0.0f));
//        model = rotate(model, Scale, vec3(0.0f, 2.0f, 0.0f));
//        model = rotate(model, Scale, vec3(1.0f, 0.0f, 0.0f));
//        mat4 view = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.1f, 0.0f));
//        mat4 persp = perspective(60.0f, float(1024/768), 1.0f, 150.0f );
//        model = rotate(model, Scale/5, vec3(0.0f, 0.0f, 0.5f));
//        model = translate(model, vec3(sinf(Scale)*5, 0.0f, 0.0f));
        mat4 ProjecationMatrix = getProjectionMatrix();
        mat4 ViewMatrix = getViewMatrix();

        mat4 modelViewProjMatrix = ProjecationMatrix* ViewMatrix* model;
        vec3 lightPosition = vec3(0, 0, 0);
//        Pipeline p;
//        p.Rotate(0.0f, Scale*10, Scale);
//        p.WorldPos(0.0f, 0.0f, 5.0f);
//        p.SetPerspectiveProj(30.0f, width, height, 1.0f, 100.0f);


        // выставляем матрицу трансформации в пространство OpenGL

        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

        // V matrix
        glUniformMatrix4fv(VLocation, 1, false, glm::value_ptr(ViewMatrix));

        // M matrix
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));
//        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, (const GLfloat*)p.GetTrans());
        // 1st texture
        glUniform1i(modelTextureLocation0, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID0);

        // 2nd texture
        glUniform1i(modelTextureLocation1, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID1);

        glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
        glUniform3f(lightPositionLocation, lightPosition[0], lightPosition[1], lightPosition[2]);
        glUniform1f(lightPowerLocation, lightPower);
        glUniform1f(textureOffsetLocation, currentTime*0.01);
        CHECK_GL_ERRORS();

        // sizeof(Vertex) - размер блока данных о вершине
        // OFFSETOF(Vertex, color) - смещение от начала

        // 1st attribute buffer: Position
        glEnableVertexAttribArray(posAttribLocation);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // Цвет вершин
//        glEnableVertexAttribArray(colorAttribLocation);
//        glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, color));

        // 2nd attribute buffer: Texture pixels
        glEnableVertexAttribArray(textureAttribLocation);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(textureAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        CHECK_GL_ERRORS();

        // 3rd attribute buffer: normals
        glEnableVertexAttribArray(normalAttribLocation);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // рисуем
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // draw points 0-3 from the currently bound VAO with current in-use shader

        //shader for circle
        glUseProgram (shaderLine);
        CHECK_GL_ERRORS();

        model = mat4(1.0);
        modelViewProjMatrix = ProjecationMatrix* ViewMatrix* model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniform3f(lineColorLocation, 1.0f, 1.0f, 1.0f);

        // position for circle vertices
        glEnableVertexAttribArray(linePosAttribLocation);
        glBindBuffer(GL_ARRAY_BUFFER, VBOLines);
        glVertexAttribPointer(linePosAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        CHECK_GL_ERRORS();

         glDrawArrays(GL_LINE_LOOP, 0, numberOfSides);
         CHECK_GL_ERRORS();
//        model = translate(model, vec3(1.0f, -1.0f, 0.0f));

//        modelViewProj = proj * model;
//        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, value_ptr(modelViewProj));


        // VBO off
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderLine);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &VBOLines);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
