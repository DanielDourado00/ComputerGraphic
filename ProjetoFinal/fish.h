// fish.h
#ifndef FISH_H
#define FISH_H

class Fish {
public:
    float posX, posY, posZ;     // Posição do peixe
    float velX, velY, velZ;     // Velocidade do peixe
    float bubbleTimer;          // Temporizador para emissão de bolhas

    // NOVAS VARIÁVEIS para animação da cauda
    float tailPhase;            // controla o ângulo ou fase do movimento
    float tailSpeed;            // controla a velocidade de oscilação

    Fish();
    void update(float deltaTime);
    void draw(bool shadowMode = false);

private:
    void drawGeometricFish(bool shadowMode = false);
};

void initializeFish();
void updateFish(float deltaTime);
void drawFish(bool shadowMode = false);

#endif // FISH_H
