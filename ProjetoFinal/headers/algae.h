// algae.h
#ifndef ALGAE_H
#define ALGAE_H

#include <GL/glut.h>
#include <vector>

class Algae {
public:
    float baseX, baseZ;    // Posição na base do aquário (plano do fundo)
    float height;          // Altura da alga
    float amplitude;       // Amplitude do balanço
    float swayPhase;       // Fase atual do balanço
    float swaySpeed;       // Velocidade de oscilação

    Algae(float x, float z, float height, float amplitude, float swaySpeed);
    void update(float deltaTime);
    void draw();
};

void initializeAlgae();
void updateAlgae(float deltaTime);
void drawAlgae();

#endif // ALGAE_H
