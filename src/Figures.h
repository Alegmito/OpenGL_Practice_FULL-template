#ifndef FIGURES_H
#define FIGURES_H

#include <glm.hpp>
#include "Vertex.h"

using namespace glm;

vec3 positions[] = {
    //piramids
    // v0
    vec3 (0.0f, 1.0f, 0.0f),
    // v1
    vec3 (1.0f, -1.0f, 1.0f),
    // v2
    vec3 (-1.0f, -1.0f, 1.0f),
    // v3
    vec3 (1.0f, -1.0f, -1.0f),
    // v4
    vec3 (-1.0f, -1.0f, -1.0f),
    //2nd piramid
    // v5
    vec3 (-3.0f, 1.0f, 0.0f),
    // v6
    vec3 (-2.0f, -1.0f, 1.0f),
    // v7
    vec3 (-4.0f, -1.0f, 1.0f),
    // v8
    vec3 (-2.0f, -1.0f, -1.0f),
    // v9
    vec3 (-4.0f, -1.0f, -1.0f),
    //3rdpiramid
    // v10
    vec3 (3.0f, 1.0f, 0.0f),
    // v11
    vec3 (4.0f, -1.0f, 1.0f),
    // v12
    vec3 (2.0f, -1.0f, 1.0f),
    // v13
    vec3 (4.0f, -1.0f, -1.0f),
    // v14
    vec3 (2.0f, -1.0f, -1.0f),
    //cubes
    };
    Vertex TriangleVertecies[] = {
                // 1
        Vertex(positions[0],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[1],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

        Vertex(positions[2],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
                // 2
        Vertex(positions[0],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[3],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

        Vertex(positions[1],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
               // 3
        Vertex(positions[0],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[4],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

        Vertex(positions[3],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
               // 4
        Vertex(positions[0],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[2],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[4],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
             // 5
        Vertex(positions[4],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[2],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

        Vertex(positions[1],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
             // 6
        Vertex(positions[4],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[1],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

        Vertex(positions[3],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

        //2nd
        // 1
Vertex(positions[5],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

Vertex(positions[6],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

Vertex(positions[7],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
        // 2
Vertex(positions[5],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

Vertex(positions[8],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

Vertex(positions[6],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
       // 3
Vertex(positions[5],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

Vertex(positions[9],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

Vertex(positions[8],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
       // 4
Vertex(positions[5],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

Vertex(positions[7],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

Vertex(positions[9],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
     // 5
Vertex(positions[9],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

Vertex(positions[7],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

Vertex(positions[6],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
     // 6
Vertex(positions[9],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

Vertex(positions[6],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

Vertex(positions[8],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
        //3rd
        Vertex(positions[10],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[11],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

        Vertex(positions[12],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
                // 2
        Vertex(positions[10],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[13],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

        Vertex(positions[11],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
               // 3
        Vertex(positions[10],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[14],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

        Vertex(positions[13],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
               // 4
        Vertex(positions[10],   vec3(1.0f, 1.0f, 1.0f), vec2(0.5f, 0.5f)),

        Vertex(positions[12],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[14],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
             // 5
        Vertex(positions[14],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[12],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

        Vertex(positions[11],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
             // 6
        Vertex(positions[14],   vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

        Vertex(positions[11],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

        Vertex(positions[13],   vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
    };

    // данные о вершинах
    int triangleVerteciesCount = 18*3;
//    Vertex triangleVertexes[] = {
//        //                ВЕРШИНА                   ЦВЕТ
//        Vertex(vec3( 0.0f,  1.0f,  0.0f),   vec3(1.0f, 0.1f, 0.1f)),
//        Vertex(vec3(-1.0f, -1.0f,  0.0f),    vec3(0.1f, 1.1f, 0.1f)),
//        Vertex(vec3( 1.0f, -1.0f,  0.0f),   vec3(0.0f, 0.1f, 1.0f))
//    };


#endif
