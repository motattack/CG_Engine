#ifndef CG_ENGINE_VERTEX_H
#define CG_ENGINE_VERTEX_H

#include <CG_Engine/math/common.h>

struct Vertex {
    Vertex() = default;

    Vertex(float x, float y, float z) : Position(Vec3(x, y, z)) {}

    Vertex(Vec3 position) : Position(position), Normal(0), TexCoords(0) {}

    Vertex(Vec3 position, Vec3 normal) : Position(position), Normal(normal), TexCoords(0) {}

    Vertex(Vec3 position, Vec3 normal, Vec2 texcoord) : Position(position), Normal(normal), TexCoords(texcoord) {}

    Vec3 Position = Vec3(0.0f);
    Vec3 Normal = Vec3(0.0f);
    Vec2 TexCoords = Vec2(0.0f);
};

#endif //CG_ENGINE_VERTEX_H
