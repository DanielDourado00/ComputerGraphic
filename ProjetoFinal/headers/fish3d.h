#ifndef FISH3D_H
#define FISH3D_H

#include <GL/glut.h>
#include <string>
#include <vector>

// Se o seu peixe tiver apenas 1 material/1 textura, você pode simplificar.
// Se tiver múltiplos materiais, use a mesma abordagem do "Boat" com submeshes.

class Fish3D {
public:
    // Vetores para armazenar a geometria do OBJ
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;

    // ID da textura (se o modelo tiver apenas 1 material)
    GLuint textureID;
    bool hasTexture;

    Fish3D();
    bool loadModel(const std::string& objPath, const std::string& basePath = "");
    void draw();

};

#endif
