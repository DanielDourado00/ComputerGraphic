#ifndef DECORATION_H
#define DECORATION_H

#include <GL/glut.h>
#include <vector>
#include <string>

class Decoration {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;

    // Posição e escala
    float posX, posY, posZ;
    float scale;
    

    // Agora teremos rotação em 3 eixos
    float rotX, rotY, rotZ;

    // Textura
    GLuint textureID;
    bool useTexture;

    Decoration();

    bool loadOBJ(const std::string& filename);
    bool setTexture(const std::string& texturePath);
    void draw();
};

void initializeDecorations();
void updateDecorations(float deltaTime);
void drawDecorations();

#endif
