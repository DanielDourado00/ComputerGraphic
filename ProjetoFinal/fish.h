// fish.h
#ifndef FISH_H
#define FISH_H

// Classe Fish representa cada peixe no aquário
class Fish {
public:
    float posX, posY, posZ;  // Posição do peixe
    float velX, velY, velZ;  // Velocidade do peixe
    float bubbleTimer;       // Temporizador para emissão de bolhas

    // Construtor
    Fish();

    // Atualiza a posição e estado do peixe
    void update(float deltaTime);

    // Desenha o peixe. Se shadowMode == true, desenha em modo sombra (sem cores).
    void draw(bool shadowMode = false);

private:
    // Desenha as diferentes partes (corpo, nadadeiras, etc.)
    // Se shadowMode == true, ignora as chamadas de cor para cada parte.
    void drawGeometricFish(bool shadowMode = false);
};

// Funções para gerenciar o conjunto global de peixes
void initializeFish();
void updateFish(float deltaTime);

// Desenha todos os peixes. Se shadowMode == true, desenha em modo sombra.
void drawFish(bool shadowMode = false);

#endif // FISH_H
