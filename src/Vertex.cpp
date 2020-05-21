
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

void bindArrayBuffer(GLint attribLocation, GLint buffer, uint size){
    glEnableVertexAttribArray(attribLocation);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
     glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void bindElementBuffer(GLint buffer){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}


Circle::Circle(vec3 center, float radius, int numberOfSides){
    float fraction = 2 * MATH_PI/numberOfSides;

    for (int i = 0; i <= numberOfSides; i++){
        vertices.push_back(vec3(
                               center.x + (radius * cos( i * fraction)),
                               center.y,
                               center.z + (radius * sin( i * fraction))));
    }

    genBuffers();
}

void Circle::genBuffers(){
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}

void Circle::addBuffers(GLint posAttribLocation){
    bindArrayBuffer(posAttribLocation, VBO, 3);
}

void Circle::deleteBuffers(){
    glDeleteBuffers(1, &VBO);
}

void Circle::draw(){
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}



Mesh::Mesh(const char * path){
    loadAssImp(path, indices, vertices, uvs, normals);
    genBuffers();
}

void Mesh::genBuffers(){
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &UVBO);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    CHECK_GL_ERRORS();
}

void Mesh::addBuffers(GLint posAttribLocation, GLint normalAttribLocation, GLint textureAttribLocation){
    bindArrayBuffer(posAttribLocation, VBO, 3);
    if(normalAttribLocation >= 0)
    bindArrayBuffer(normalAttribLocation, NBO, 3);
    bindArrayBuffer(textureAttribLocation, UVBO, 2);
    bindElementBuffer(IBO);
}

void Mesh::deleteBuffers(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &UVBO);
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &NBO);
}

void Mesh::draw(){
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


Texture::Texture(const char * path, GLuint _texture){
    loadImage(path, &textureId);
    textureNumber = _texture;
}

void Texture::loadImage(const char* path, GLuint * textureId){
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

void Texture::UniformTexure(GLint textureLocation, GLuint tNumber){
    glUniform1i(textureLocation, tNumber);
}

void Texture::genTexture0(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::genTexture1(){
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);

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
