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
 vec3 lightPosition = vec3(0, 0, 0);

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
glm::vec3 position = glm::vec3( 0, 10, 10 );
// Initial horizontal angle : toward -Z
float horizontalAngle = MATH_PI;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 20.0f; // 3 units / second
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
        glGenTextures(1, textureId);
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
    GLuint shaderOneTexture = createShaderLightOneTexture();
    GLuint shaderNoLight = createShaderNoLight();
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

    // Getting access to ShaderNoLight
    //attributes
    int posAttribLocationNoL = glGetAttribLocation(shaderNoLight, "aPos");
    int colorAttribLocationNoL = glGetAttribLocation(shaderNoLight, "aColor");
    int textureAttribLocationNoL = glGetAttribLocation(shaderNoLight, "aTexture");
    int normalAttribLocationNoL = glGetAttribLocation(shaderNoLight, "aNormal");
    //uniforms
    int modelViewProjMatrixLocationNoL = glGetUniformLocation(shaderNoLight, "uModelViewProjMat");
    int modelTextureLocationNoL = glGetUniformLocation (shaderNoLight, "uTexture");
    CHECK_GL_ERRORS();

    //Getting access to the ShaderOneTexture
    // аттрибуты вершин шейдера
    int posAttribLocationOneTexture = glGetAttribLocation(shaderOneTexture, "aPos");
    int colorAttribLocationOneTexture = glGetAttribLocation(shaderOneTexture, "aColor");
    int textureAttribLocationOneTexture = glGetAttribLocation(shaderOneTexture, "aTexture");
    int normalAttribLocationOneTexture = glGetAttribLocation(shaderOneTexture, "aNormal");
    CHECK_GL_ERRORS();

    // юниформы шейдера
    int modelViewProjMatrixLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uModelViewProjMat");
    int modelTextureLocationOneTexture = glGetUniformLocation (shaderOneTexture, "Texture");
    int lightColorLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uLightColor");
    int lightPowerLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uLightPower");
    int lightPositionLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uLightPosition");
    int VLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uV");
    int MLocationOneTexture = glGetUniformLocation(shaderOneTexture, "uM");
    CHECK_GL_ERRORS();

    //Read obj. file
    shared_ptr<Mesh> planet = make_shared<Mesh>("res/sphere.obj");

    // Get access to shaderLine
    // CirceDrawing
    shared_ptr<Circle> orbit = make_shared<Circle>(vec3(0.0f, 0.0f, 0.0f), 1.0, 300);
    // Attribute
    int linePosAttribLocation = glGetAttribLocation(shaderLine, "aPos");

    // Shader's uniforms
    int lineModelViewProjMatrixLocation = glGetUniformLocation(shaderLine, "uModelViewProjMat");
    int lineColorLocation = glGetUniformLocation(shaderLine, "uColor");
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

    // Textures for all planets
//    double time = glfwGetTime();
    //Sun
    shared_ptr<Texture> sunTexture = make_shared<Texture>("res/sun.png", 0);
    // Mercury
    shared_ptr<Texture> mercuryTexture = make_shared<Texture>("res/mercury.png", 0);
    // Venus
    shared_ptr<Texture> venusSurfTexture = make_shared<Texture>("res/venus_surface.png", 0);
    shared_ptr<Texture> venusAtmTexture = make_shared<Texture>("res/venus_atmosphere.png", 1);
    // Earth
    shared_ptr<Texture> earthSurfTexture = make_shared<Texture>("res/sphere.png", 0);
    shared_ptr<Texture> earthAtmTexture = make_shared<Texture>("res/Clouds.png", 1);
    // Moon
    shared_ptr<Texture> moonTexture = make_shared<Texture>("res/moon.png", 0);
    // Mars
    shared_ptr<Texture> marsTexture = make_shared<Texture>("res/mars.png", 0);
    // Jupiter
    shared_ptr<Texture> jupiterTexture = make_shared<Texture>("res/jupiter.png", 0);
    // Saturn
    shared_ptr<Texture> saturnTexture = make_shared<Texture>("res/saturn.png", 0);
    // Urans
    shared_ptr<Texture> uranusTexture = make_shared<Texture>("res/uranus.png", 0);
    // Neptune
    shared_ptr<Texture> neptuneTexture = make_shared<Texture>("res/neptune.png", 0);
    //skybox

    shared_ptr<Texture> sky = make_shared<Texture>("res/milky_way.png", 0);

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
        ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);
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



        glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
        glUniform3f(lightPositionLocation, lightPosition[0], lightPosition[1], lightPosition[2]);
        glUniform1f(lightPowerLocation, lightPower);

        CHECK_GL_ERRORS();

        // матрица модель-вид-проекция
        mat4 model = mat4(1.0);

        // 3 Earth 50

        model = translate(model, vec3(50.0*sinf(currentTime/100), 0, 50.0*cosf(currentTime/100)));
        model = rotate(model,0.5f , vec3(0, 0, 1));
        model = rotate(model,(float)currentTime/23.56f , vec3(0, 1, 0));
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

        mat4 ViewProjMatrix = ProjecationMatrix* ViewMatrix;
        mat4 modelViewProjMatrix =ViewProjMatrix * model;
        // выставляем матрицу трансформации в пространство OpenGL

        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

        // V matrix
        glUniformMatrix4fv(VLocation, 1, false, glm::value_ptr(ViewMatrix));

        // M matrix//
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));
//        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, (const GLfloat*)p.GetTrans());
        // 1st texture
        Texture::UniformTexure(modelTextureLocation0, 0);
        earthSurfTexture->genTexture0();

        // 2nd texture
        Texture::UniformTexure(modelTextureLocation1, 1);
        earthAtmTexture->genTexture1();

        glUniform1f(textureOffsetLocation, currentTime/15);

        planet -> addBuffers(posAttribLocation, normalAttribLocation, textureAttribLocation);

        // рисуем
        planet -> draw(); // draw points 0-3 from the currently bound VAO with current in-use shader

        // 2 Venus 30
        // trajectory
        model = translate(mat4(1.0), vec3(30.0f*sinf(currentTime/60+0.5), 0, 30.0f*cosf(currentTime/60+0.5)));
        //size
        model = scale(model, vec3(0.95f, 0.95f, 0.95f));

        // наклон и вращение
        model = rotate(model,(float)currentTime/500.3f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));
        glUniform1f(textureOffsetLocation, currentTime/10);

        venusAtmTexture ->  genTexture0();

        venusSurfTexture -> genTexture1();

        planet -> draw();


        // Shader for planets with one texture
        glUseProgram(shaderOneTexture);
        CHECK_GL_ERRORS();

        // 5 Jupiter
        // trajectory
        model = translate(mat4(1.0), vec3(100*sinf(currentTime/500 + 5), 0, 100*cosf(currentTime/500 + 5)));
        //size
        model = scale(model, vec3(5.0, 5.0, 5.0));

        // наклон и вращение
        model = rotate(model, (float)currentTime/10.0f, vec3(0.0, 1.0, 0.0));
        modelViewProjMatrix = ViewProjMatrix * model;

        // V matrix
        glUniformMatrix4fv(VLocationOneTexture, 1, false, glm::value_ptr(ViewMatrix));

        // M matrix
        glUniformMatrix4fv(MLocationOneTexture, 1, false, glm::value_ptr(model));

        glUniformMatrix4fv(modelViewProjMatrixLocationOneTexture, 1, false, glm::value_ptr(modelViewProjMatrix));
        CHECK_GL_ERRORS();
        // texture
        Texture::UniformTexure(modelTextureLocationOneTexture, 0);
        jupiterTexture->genTexture0();
        CHECK_GL_ERRORS();
        glUniform3f(lightColorLocationOneTexture, lightColor[0], lightColor[1], lightColor[2]);
        glUniform3f(lightPositionLocationOneTexture, lightPosition[0], lightPosition[1], lightPosition[2]);
        glUniform1f(lightPowerLocationOneTexture, lightPower);
        CHECK_GL_ERRORS();

        planet->addBuffers(posAttribLocationOneTexture, normalAttribLocationOneTexture, textureAttribLocationOneTexture);

        planet->draw();

        // 1 Mercury
        // trajectory
        model = translate(mat4(1.0), vec3(20.0f*sinf(currentTime/40+4), 0, 20.0f*cosf(currentTime/40+4)));
        //size
        model = scale(model, vec3(0.3f, 0.3f, 0.3f));
        // наклон
        model = rotate(model,(float)currentTime/300.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

         mercuryTexture ->  genTexture0();

        planet -> draw();

        // 3.1 Moon
        // trajectory
        model = translate(mat4(1.0), vec3(50.0*sinf(currentTime/100), 0, 50.0*cosf(currentTime/100)));
        model = translate(model, vec3(3*sinf(currentTime/100), 0, 3*cosf(currentTime/100)));
        //size
        model = scale(model, vec3(0.2f, 0.2f, 0.2f));

        // наклон и вращение
        model = rotate(model,(float)currentTime/100.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

        moonTexture ->  genTexture0();

        planet -> draw();


        // 4 Mars
        // trajectory
        model = translate(mat4(1.0), vec3(70.0f*sinf(currentTime/40+2.1), 0, 70.0f*cosf(currentTime/40+2.1)));
        //size
        model = scale(model, vec3(0.5f, 0.5f, 0.5f));

        // наклон
        model = rotate(model,0.5f , vec3(0, 0, 1));
        model = rotate(model,(float)currentTime/24.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

        marsTexture ->  genTexture0();

        planet -> draw();

        // 6 Saturn 130
        // trajectory
        model = translate(mat4(1.0), vec3(130.0f*sinf(currentTime/600+4.9), 0, 130.0f*cosf(currentTime/600+4.9)));
        //size
        model = scale(model, vec3(4.5f, 4.5f, 4.5f));

        // наклон и вращение
        model = rotate(model,0.5f , vec3(0, 0, 1));
        model = rotate(model,(float)currentTime/9.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

        saturnTexture ->  genTexture0();

        planet -> draw();

        // 7 Uranus 150
        // trajectory
        model = translate(mat4(1.0), vec3(150.0f*sinf(currentTime/640+3.5), 0, 150.0f*cosf(currentTime/640+3.5)));
        //size
        model = scale(model, vec3(3.0f, 3.0f, 3.0f));
        // наклон
        model = rotate(model,4.95f , vec3(0, 0, 1));
        model = rotate(model,(float)currentTime/17.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

        saturnTexture ->  genTexture0();

        planet -> draw();

        // 8 Neptune 180
        // trajectory
        model = translate(mat4(1.0), vec3(180.0f*sinf(currentTime/670+1.7), 0, 180.0f*cosf(currentTime/670+1.7)));
        //size
        model = scale(model, vec3(3.0f, 3.0f, 3.0f));
        // наклон и вращение
        model = rotate(model, 0.5f , vec3(0, 0, 1));
        model = rotate(model,(float)currentTime/16.0f , vec3(0, 1, 0));
        modelViewProjMatrix =ViewProjMatrix * model;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniformMatrix4fv(MLocation, 1, false, glm::value_ptr(model));

        neptuneTexture ->  genTexture0();

        planet -> draw();
        //Shader for Sun

        glUseProgram(shaderNoLight);

        model = scale(mat4(1.0), vec3(9.0, 9.0, 9.0));
        model = rotate(model, (float)currentTime/50, vec3(0.0, 1.0, 0.0));
        modelViewProjMatrix = ViewProjMatrix * model;

        // выставляем матрицу трансформации в пространство OpenGL
        glUniformMatrix4fv(modelViewProjMatrixLocationNoL, 1, false, glm::value_ptr(modelViewProjMatrix));

        Texture::UniformTexure(modelTextureLocationNoL, 0);
        sunTexture->genTexture0();

        planet->addBuffers(posAttribLocationNoL, normalAttribLocationNoL, textureAttribLocationNoL);

        planet->draw();
        CHECK_GL_ERRORS();
        // Включаем отображение задней части полигонов для рисования фона
        glDisable(GL_CULL_FACE);
        // смещаем фон в соответствии с положением камеры
        model = translate(mat4(1.0), position);
        model = scale(model, vec3(500, 500, 500));
        model = rotate(model, (float)currentTime/700, vec3(0.0, 1.0, 0.0));
        modelViewProjMatrix = ViewProjMatrix * model;

        glUniformMatrix4fv(modelViewProjMatrixLocationNoL, 1, false, glm::value_ptr(modelViewProjMatrix));

        Texture::UniformTexure(modelTextureLocationNoL, 0);
        sky->genTexture0();

        planet->addBuffers(posAttribLocationNoL, normalAttribLocationNoL, textureAttribLocationNoL);

        planet->draw();
        //shader for Orbits
        glUseProgram (shaderLine);
        CHECK_GL_ERRORS();

        glEnable(GL_CULL_FACE);
        // 1 Mercury 20
        model = scale(mat4(1.0), vec3(20.0f, 20.0f, 20.0f));
        modelViewProjMatrix =ViewProjMatrix * model;;
        glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));
        glUniform3f(lineColorLocation, 1.0f, 1.0f, 1.0f);

         orbit -> addBuffers(linePosAttribLocation);
         orbit -> draw();

         // 2 Venus 30
         model = scale(mat4(1.0), vec3(30.0f, 30.0f, 30.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 3 Earth 50

         model = scale(mat4(1.0), vec3(50.0f, 50.0f, 50.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 3.1 Moon 50
         model = translate(mat4(1.0), vec3(50.0*sinf(currentTime/100), 0, 50.0*cosf(currentTime/100)));
         model = scale(model, vec3(3.0f, 3.0f, 3.0f));

         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 4 Mars 70

         model = scale(mat4(1.0), vec3(70.0f, 70.0f, 70.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 5 Jupiter 100

         model = scale(mat4(1.0), vec3(100.0f, 100.0f, 100.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 6 Saturn 130

         model = scale(mat4(1.0), vec3(130.0f, 130.0f, 130.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 6.1 Saturn rings
         model = translate(mat4(1.0), vec3(130.0f*sinf(currentTime/600+4.9), 0, 130.0f*cosf(currentTime/600+4.9)));
         model = scale(model, vec3(6.0f, 6.0f, 6.0f));
         model = rotate(model, 0.5f, vec3(0.0, 0.0, 1.0));

         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         model = translate(mat4(1.0), vec3(130.0f*sinf(currentTime/600+4.9), 0, 130.0f*cosf(currentTime/600+4.9)));
         model = scale(model, vec3(9.0f, 9.0f, 9.0f));
         model = rotate(model, 0.5f, vec3(0.0, 0.0, 1.0));

         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 7 Uranus 150

         model = scale(mat4(1.0), vec3(150.0f, 150.0f, 150.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

         orbit -> draw();

         // 8 Uranus 180

         model = scale(mat4(1.0), vec3(180.0f, 180.0f, 180.0f));
         modelViewProjMatrix =ViewProjMatrix * model;
         glUniformMatrix4fv(lineModelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

        orbit -> draw();

        // VBO off
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderLine);
    orbit -> deleteBuffers();
    planet -> deleteBuffers();

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
