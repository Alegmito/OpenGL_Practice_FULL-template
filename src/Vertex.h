#ifndef VERTEX_H
#define VERTEX_H

#include "PngLoader.h"
#include "ObjLoader.h"
#include "Helpers.h"
#include <GL/glew.h>
#include <glm.hpp>          // библиотека графической математики
#include <memory>


using namespace glm;
using namespace std;

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)


//class Circle{
//public:
//    Circle();
//    Circle(float x, float y, float z, float radius);
//    void Draw();
//    vec3 circleVertices[200];
//private:
//    vec3 center;
//    float radius;


//};

void bindArrayBuffer(GLint attribLocation, GLint buffer, uint size);
void bindElementBuffer(GLint buffer);


class Mesh{
public:
    Mesh(const char * path);
    void genBuffers();
    void addBuffers(GLint posAttribLocation, GLint normalAttribLocation, GLint textureAttribLocation);
    void deleteBuffers();
    void draw();
protected:
    vector<vec3> vertices;
    vector<unsigned short> indices;
    vector<vec2> uvs;
    vector<vec3> normals;
    GLuint VBO;
    GLuint IBO;
    GLuint UVBO;
    GLuint NBO;
};

//class SkySphere {
//public:

//private:
//    shared_ptr<mesh> planet;
//    shared_ptr<Circle> orbit;

//};

class Circle{
public:
    // make circle
    Circle(vec3 center, float radius, int numberOfSides);

    void draw();

    void addBuffers(GLint posAttribLocation);

    void genBuffers();

    void deleteBuffers();

private:
    vector<vec3> vertices;
    GLuint VBO;
};




class Texture{
public:
    Texture(const char * path, GLuint _textureNumber);
    void loadImage (const char * path, GLuint * textureId);
    static void UniformTexure(GLint textureLocation, GLuint tNumber);
    void genTexture0();
    void genTexture1();
private:
    GLuint textureNumber;
    GLuint textureId;
};


class Matrix4f
{
public:
    float m[4][4];

    Matrix4f()
    {
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }
};


struct Vertex{
    vec3 pos;
    vec3 color;
    vec2 texture;

    // constructor
    Vertex(vec3 inPos, vec3 inColor, vec2 inTexture);
};

//class Pipeline
//{
//public:
//    Pipeline()
//    {
//        m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
//        m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
//        m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
//    }

//    void Scale(float ScaleX, float ScaleY, float ScaleZ)
//    {
//        m_scale.x = ScaleX;
//        m_scale.y = ScaleY;
//        m_scale.z = ScaleZ;
//    }

//    void WorldPos(float x, float y, float z)
//    {
//        m_worldPos.x = x;
//        m_worldPos.y = y;
//        m_worldPos.z = z;
//    }

//    void Rotate(float RotateX, float RotateY, float RotateZ)
//    {
//        m_rotateInfo.x = RotateX;
//        m_rotateInfo.y = RotateY;
//        m_rotateInfo.z = RotateZ;
//    }

//    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
//       {
//           m_persProj.FOV    = FOV;
//           m_persProj.Width  = Width;
//           m_persProj.Height = Height;
//           m_persProj.zNear  = zNear;
//           m_persProj.zFar   = zFar;
//       }

//       const Matrix4f* GetTrans();

//   private:
//       void InitScaleTransform(Matrix4f& m) const;
//       void InitRotateTransform(Matrix4f& m) const;
//       void InitTranslationTransform(Matrix4f& m) const;
//       void InitPerspectiveProj(Matrix4f& m) const;

//       Vector3f m_scale;
//       Vector3f m_worldPos;
//       Vector3f m_rotateInfo;

//       struct {
//           float FOV;
//           float Width;
//           float Height;
//           float zNear;
//           float zFar;
//       } m_persProj;

//       Matrix4f m_transformation;
//   };


//class Camera
//{
//public:
//    Camera();
//    Camera(const vec3 Position, const vec3 Eye, const vec3 Up);
//    bool OnKeyBoard (int Key);
//    const vec3 GetPos() const{
//        return m_pos;
//    }
//    const vec3 GetEye() const{
//        return m_eye;
//    }
//    const vec3 GetUp() const{
//        return m_up;
//    }

//private:
//    vec3 m_pos;
//    vec3 m_eye;
//    vec3 m_up;
//};
#endif
