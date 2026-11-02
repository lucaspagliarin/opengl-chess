#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh {
private:
    unsigned int VAO, VBO;
    int vertexCount;

public:
    Mesh(float* vertices, int size);
    ~Mesh();

    void draw() const;
};

#endif
