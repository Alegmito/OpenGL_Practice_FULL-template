#include <GLFW/glfw3.h>
#include "Vertex.h"

//Camera class
//Camera class
//Camera class
//Camera class
//Camera class
//Camera class
//Camera class
//Camera class
#define MATH_PI 3.14159265
const static float StepScale = 0.1f;

Texture::Texture(char * path){
    textureId = textureCount;
    textureNumber = textureCount;
    textureCount += 1;
    loadImage(path, &textureId);
}

void Texture::loadImage(char* path, GLuint* textureId){
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

void Texture::genTexture(){

}

void DrawCircle(float x, float y, float z, float radius, int numberOfSides, vec3* circleVertices){

    float fraction = 2 * MATH_PI/numberOfSides;

    for (int i = 0; i <= numberOfSides; i++){
        circleVertices[i].x = x + (radius * cos( i * fraction));
        circleVertices[i].y = y;
        circleVertices[i].z = z + (radius * sin( i * fraction));
    }

//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, circleVertices);
//    glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices);
//    glDisableClientState( GL_VERTEX_ARRAY);
}

//Camera::Camera(){
//    m_pos = vec3(0.0f, 0.0f, 0.0f);
//    m_eye = vec3(0.0f, 0.0f, 1.0f);
//    m_up  = vec3(0.0f, 0.0f, 0.0f);
//}

//Camera::Camera(const vec3 Pos, const vec3 Eye, const vec3 Up){
//    m_pos = Pos;
//    m_eye = normalize(Eye);
//    m_up  = normalize(Up);
//}

//bool Camera::OnKeyBoard(int Key){
//    bool Ret = false;

//    switch (Key) {

//    case GLFW_KEY_UP:{
//        m_pos += (m_eye * StepScale);
//        Ret = true;
//    }break;

//    case GLFW_KEY_DOWN:{
//        m_pos -= (m_eye * StepScale);
//        Ret = true;
//    }break;

//    case GLFW_KEY_LEFT:{
//        m_pos += (m_eye * StepScale);
//        Ret = true;
//    }break;

//    case GLFW_KEY_RIGHT:{
//        m_pos += (m_eye * StepScale);
//        Ret = true;
//    }break;
//    }
//}
// Vertex Class
// Vertex Class
    // Vertex Class
    // Vertex Class
    // Vertex Class
    // Vertex Class
        // Vertex Class
        // Vertex Class
Vertex::Vertex(vec3 inPos, vec3 inColor, vec2 inTexture):
    pos(inPos),
    color(inColor),
    texture(inTexture){
}


// Pipeline Class
// Pipeline Class
// Pipeline Class
// Pipeline Class
//void Pipeline::InitScaleTransform(Matrix4f& m) const
//{
//    m.m[0][0] = m_scale.x; m.m[0][1] = 0.0f     ; m.m[0][2] = 0.0f     ; m.m[0][3] = 0.0f;
//    m.m[1][0] = 0.0f     ; m.m[1][1] = m_scale.y; m.m[1][2] = 0.0f     ; m.m[1][3] = 0.0f;
//    m.m[2][0] = 0.0f     ; m.m[2][1] = 0.0f     ; m.m[2][2] = m_scale.z; m.m[2][3] = 0.0f;
//    m.m[3][0] = 0.0f     ; m.m[3][1] = 0.0f     ; m.m[3][2] = 0.0f     ; m.m[3][3] = 1.0f;
//}

//void Pipeline::InitRotateTransform(Matrix4f& m) const
//{
//    Matrix4f rx, ry, rz;

//    const float x = ToRadian(m_rotateInfo.x);
//    const float y = ToRadian(m_rotateInfo.y);
//    const float z = ToRadian(m_rotateInfo.z);

//    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
//    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
//    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
//    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

//    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
//    ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
//    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
//    ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

//    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
//    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
//    rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
//    rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

//    m = rz * ry * rx;
//}


//void Pipeline::InitTranslationTransform(Matrix4f& m) const
//{
//    m.m[0][0] = 1.0f; m.m[0][1] = 0.0f; m.m[0][2] = 0.0f; m.m[0][3] = m_worldPos.x;
//    m.m[1][0] = 0.0f; m.m[1][1] = 1.0f; m.m[1][2] = 0.0f; m.m[1][3] = m_worldPos.y;
//    m.m[2][0] = 0.0f; m.m[2][1] = 0.0f; m.m[2][2] = 1.0f; m.m[2][3] = m_worldPos.z;
//    m.m[3][0] = 0.0f; m.m[3][1] = 0.0f; m.m[3][2] = 0.0f; m.m[3][3] = 1.0f;
//}

//void Pipeline::InitPerspectiveProj(Matrix4f& m) const
//{
//    const float ar         = m_persProj.Width / m_persProj.Height;
//    const float zNear      = m_persProj.zNear;
//    const float zFar       = m_persProj.zFar;
//    const float zRange     = zNear - zFar;
//    const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0f));

//    m.m[0][0] = 1.0f/(tanHalfFOV * ar); m.m[0][1] = 0.0f;            m.m[0][2] = 0.0f;                   m.m[0][3] = 0.0;
//    m.m[1][0] = 0.0f;                   m.m[1][1] = 1.0f/tanHalfFOV; m.m[1][2] = 0.0f;                   m.m[1][3] = 0.0;
//    m.m[2][0] = 0.0f;                   m.m[2][1] = 0.0f;            m.m[2][2] = (-zNear -zFar)/zRange ; m.m[2][3] = 2.0f * zFar*zNear/zRange;
//    m.m[3][0] = 0.0f;                   m.m[3][1] = 0.0f;            m.m[3][2] = 1.0f;                   m.m[3][3] = 0.0;
//}

//const Matrix4f* Pipeline::GetTrans()
//{
//    Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

//    InitScaleTransform(ScaleTrans);
//    InitRotateTransform(RotateTrans);
//    InitTranslationTransform(TranslationTrans);

//    m_transformation = TranslationTrans * RotateTrans * ScaleTrans;
//    return &m_transformation;
//}


