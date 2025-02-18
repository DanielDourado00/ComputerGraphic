#ifndef DECORATION_H
#define DECORATION_H

#include <GL/glut.h>
#include <string>
#include <vector>

// Cada decoração 3D que carregamos terá:
// decoration.h
class Decoration {
    public:
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texcoords;
    
        float posX, posY, posZ;
        float scale;
    
        // Em vez de só rotY, podemos ter rotX, rotY, rotZ.
        float rotX, rotY, rotZ;
    
        Decoration();
        bool loadOBJ(const std::string& filename);
        void draw();
    };
    
// Função para inicializar alguns objetos de decoração (rochas, baús, etc.)
void initializeDecorations();

// Atualizar ou animar as decorações, se necessário
void updateDecorations(float deltaTime);

// Desenhar todas as decorações na cena
void drawDecorations();

#endif // DECORATION_H
