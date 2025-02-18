#ifndef DECORATION_H
#define DECORATION_H

#include <GL/glut.h>
#include <string>
#include <vector>

// Cada decoração 3D que carregamos terá:
class Decoration {
public:
    // Vetores para armazenar os dados do OBJ (em arrays "immediate mode" mesmo)
    std::vector<float> vertices; 
    std::vector<float> normals;
    std::vector<float> texcoords; // se quiser usar texturas
    // Você pode armazenar índices também, mas no immediate mode não é estritamente necessário

    // Para posicionar, escalar e rotacionar no aquário
    float posX, posY, posZ;
    float scale;
    float rotY;

    // Construtor básico
    Decoration();

    // Carrega o arquivo OBJ e preenche vertices, normals, etc.
    bool loadOBJ(const std::string& filename);

    // Desenha a decoração no aquário
    void draw();
};

// Função para inicializar alguns objetos de decoração (rochas, baús, etc.)
void initializeDecorations();

// Atualizar ou animar as decorações, se necessário
void updateDecorations(float deltaTime);

// Desenhar todas as decorações na cena
void drawDecorations();

#endif // DECORATION_H
