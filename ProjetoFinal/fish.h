// fish.h
#ifndef FISH_H
#define FISH_H

class Fish {
public:
    float posX, posY, posZ;
    float velX, velY, velZ;
    float bubbleTimer;

    // Construtor
    Fish();
    void update(float deltaTime);
    void draw();

private:
    // Nada aqui, pois o modelo 3D é global/estático
};

void initializeFish();  // Carrega o modelo e cria os peixes
void updateFish(float deltaTime);
void drawFish(bool shadowMode = false);

#endif
