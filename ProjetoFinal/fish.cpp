// fish.cpp
#include "fish.h"
#include "bubbles.h"  // Necessário para chamar createFishBubble()
#include <cstdlib>
#include <cmath>
#include <ctime>

// Limites do aquário (metade das dimensões definidas em aquarium.cpp)
const float halfWidth  = 10.0f / 2.0f;
const float halfHeight = 5.0f / 2.0f;
const float halfDepth  = 8.0f / 2.0f;

#define NUM_FISH 5  // Número de peixes

static Fish fishArray[NUM_FISH];

Fish::Fish() {
    // Inicializa posição dentro do aquário
    posX = ((float)rand() / RAND_MAX) * (2.0f * halfWidth)  - halfWidth;
    posY = ((float)rand() / RAND_MAX) * (2.0f * halfHeight) - halfHeight;
    posZ = ((float)rand() / RAND_MAX) * (2.0f * halfDepth)  - halfDepth;

    // Inicializa velocidades
    velX = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velY = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velZ = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

    // Inicializa temporizador para bolhas
    bubbleTimer = 0.0f;
}

void Fish::update(float deltaTime) {
    // Movimentação
    posX += velX * deltaTime;
    posY += velY * deltaTime;
    posZ += velZ * deltaTime;

    // Rebater nas paredes do aquário
    if (posX < -halfWidth || posX > halfWidth)   velX = -velX;
    if (posY < -halfHeight || posY > halfHeight) velY = -velY;
    if (posZ < -halfDepth || posZ > halfDepth)   velZ = -velZ;

    // Exemplo de emissão de bolhas a cada 10s:
    bubbleTimer += deltaTime;
    if (bubbleTimer >= 10.0f) {
        // createFishBubble(posX, posY, posZ); 
        bubbleTimer = 0.0f;
    }
}

// Recebe um parâmetro "shadowMode" (padrão = false). Se shadowMode for true,
// desenhamos o peixe somente em preto (sem chamar glColor3f(...) para partes).
void Fish::draw(bool shadowMode) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    // Ajusta rotação do peixe com base na direção do movimento
    float angle = atan2(velZ, velX) * (180.0f / M_PI);
    glRotatef(-angle, 0.0f, 1.0f, 0.0f);

    drawGeometricFish(shadowMode);

    glPopMatrix();
}

// Desenha as partes do peixe. Se shadowMode for true, não chamamos glColor.
void Fish::drawGeometricFish(bool shadowMode) {
    // Corpo
    if (!shadowMode) {
        glColor3f(1.0f, 0.7f, 0.0f);  // cor laranja para o corpo
    }
    glPushMatrix();
        // Esfera escalonada (mais longa no eixo X, achatada no Z)
        glScalef(1.5f, 0.8f, 0.4f);
        glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Cauda
    if (!shadowMode) {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    glPushMatrix();
        glTranslatef(-0.65f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCone(0.15, 0.4, 10, 2);
    glPopMatrix();

    // Nadadeira superior
    if (!shadowMode) {
        glColor3f(1.0f, 0.6f, 0.0f);
    }
    glPushMatrix();
        glTranslatef(0.0f, 0.20f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeira inferior
    if (!shadowMode) {
        glColor3f(1.0f, 0.6f, 0.0f);
    }
    glPushMatrix();
        glTranslatef(0.0f, -0.18f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeiras laterais
    if (!shadowMode) {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    // Lateral direita
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.25f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCone(0.08, 0.15, 10, 2);
    glPopMatrix();
    // Lateral esquerda
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -0.25f);
        // Sem rotação extra aqui, se preferir
        glutSolidCone(0.08, 0.15, 10, 2);
    glPopMatrix();

    // Olhos e Pupilas (se não estiver em modo sombra)
    if (!shadowMode) {
        // Olhos (brancos)
        glColor3f(1.0f, 1.0f, 1.0f);
        // Olho esquerdo
        glPushMatrix();
            glTranslatef(0.35f, 0.05f, 0.12f);
            glutSolidSphere(0.04, 10, 10);
        glPopMatrix();
        // Olho direito
        glPushMatrix();
            glTranslatef(0.35f, 0.05f, -0.12f);
            glutSolidSphere(0.04, 10, 10);
        glPopMatrix();

        // Pupilas (pretas)
        glColor3f(0.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.35f, 0.05f, 0.12f);
            glutSolidSphere(0.015, 10, 10);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.35f, 0.05f, -0.12f);
            glutSolidSphere(0.015, 10, 10);
        glPopMatrix();
    }
}

// Funções globais para gerenciar o array de peixes

void initializeFish() {
    srand((unsigned int)time(0));
    for (int i = 0; i < NUM_FISH; i++) {
        fishArray[i] = Fish();
    }
}

void updateFish(float deltaTime) {
    for (int i = 0; i < NUM_FISH; i++) {
        fishArray[i].update(deltaTime);
    }
}

// Desenha todos os peixes. Se shadowMode for true, não usamos cores internas.
void drawFish(bool shadowMode) {
    for (int i = 0; i < NUM_FISH; i++) {
        fishArray[i].draw(shadowMode);
    }
}
