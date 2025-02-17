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

// Define o flag global (inicia como falso)
bool shadowMode = false;

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

    bubbleTimer = 0.0f;
}

void Fish::update(float deltaTime) {
    // Movimentação básica
    posX += velX * deltaTime;
    posY += velY * deltaTime;
    posZ += velZ * deltaTime;

    // Rebater nas paredes do aquário
    if (posX < -halfWidth || posX > halfWidth)   velX = -velX;
    if (posY < -halfHeight || posY > halfHeight) velY = -velY;
    if (posZ < -halfDepth || posZ > halfDepth)   velZ = -velZ;

    // Emissão de bolhas a cada X segundos (se quiser)
    bubbleTimer += deltaTime;
    if (bubbleTimer >= 10.0f) {
        // createFishBubble(posX, posY, posZ);
        bubbleTimer = 0.0f;
    }
}

void Fish::draw() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    float angle = atan2(velZ, velX) * (180.0f / M_PI);
    glRotatef(-angle, 0.0f, 1.0f, 0.0f);

    // Agora chamamos a função que desenha o peixe todo com primitivas
    drawGeometricFish();

    glPopMatrix();
}


void Fish::drawGeometricFish() {
    // Definindo uma cor laranja para o corpo
    glColor3f(1.0f, 0.7f, 0.0f);

    // Corpo: esfera escalonada (mais longa no eixo X e achatada no eixo Z)
    glPushMatrix();
        glScalef(1.5f, 0.8f, 0.4f); // Ajuste conforme desejar
        glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Cauda: cone na parte de trás
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
        // Mover-se para trás do corpo
        glTranslatef(-0.45f, 0.0f, 0.0f);
        // Girar para ficar perpendicular ao eixo X
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCone(0.15, 0.4, 10, 2);
    glPopMatrix();

    // Nadadeira Superior (dorsal)
    glPushMatrix();
        glTranslatef(-0.2f, 0.25f, 0.0f);
        // Deitar o cone
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeira Inferior (ventral)
    glPushMatrix();
        glTranslatef(-0.2f, -0.25f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeiras Laterais (peitorais)
    // Uma de cada lado do peixe
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.25f); // Ajuste para a lateral direita
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //
        glutSolidCone(0.08, 0.15, 10, 2);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -0.25f); // Ajuste para a lateral esquerda
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.08, 0.15, 10, 2);
    glPopMatrix();

    // Olhos: pequenas esferas brancas
    glColor3f(1.0f, 1.0f, 1.0f);
    // Olho esquerdo
    glPushMatrix();
        // Leve inclinação para frente e para o lado
        glTranslatef(0.35f, 0.05f, 0.12f);
        glutSolidSphere(0.04, 10, 10);
    glPopMatrix();
    // Olho direito
    glPushMatrix();
        glTranslatef(0.35f, 0.05f, -0.12f);
        glutSolidSphere(0.04, 10, 10);
    glPopMatrix();

    // Pupilas (opcional): esferas pretas dentro dos olhos
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

void drawFish() {
    for (int i = 0; i < NUM_FISH; i++) {
        fishArray[i].draw();
    }
}
