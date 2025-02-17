// bubbles.h
#ifndef BUBBLES_H
#define BUBBLES_H

#include <GL/glut.h>
#include <vector>

class Bubble {
public:
    float posX, posY, posZ;  // Posição da bolha
    float velY;              // Velocidade vertical (ascendente)
    float radius;            // Tamanho da bolha

    Bubble(float x, float y, float z, float r, float vy);
    void update(float deltaTime);
    void draw();
};

// Funções para gerenciar as bolhas
void initializeBubbles();
void updateBubbles(float deltaTime);
void drawBubbles();

// Funções para criar bolhas:
// Uma para as bolhas naturais e outra para as emitidas pelos peixes.
void createNaturalBubble();
void createFishBubble(float x, float y, float z);

#endif // BUBBLES_H
