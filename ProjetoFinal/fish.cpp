#include "fish.h"
#include "bubbles.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

const float halfWidth  = 10.0f / 2.0f;
const float halfHeight = 5.0f  / 2.0f;
const float halfDepth  = 8.0f  / 2.0f;

#define NUM_FISH 5

static Fish fishArray[NUM_FISH];

Fish::Fish() {
    // Posição dentro do aquário
    posX = ((float)rand() / RAND_MAX) * (2.0f * halfWidth)  - halfWidth;
    posY = ((float)rand() / RAND_MAX) * (2.0f * halfHeight) - halfHeight;
    posZ = ((float)rand() / RAND_MAX) * (2.0f * halfDepth)  - halfDepth;

    // Velocidades
    velX = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velY = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velZ = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

    bubbleTimer = 0.0f;

    // Inicializa a animação da cauda:
    tailPhase = ((float)rand() / RAND_MAX) * 360.0f;   // fase inicial aleatória (em graus)
    tailSpeed = 80.0f + ((float)rand() / RAND_MAX) * 40.0f; 
    // ex.: velocidade entre 80 e 120 graus/segundo
}

void Fish::update(float deltaTime) {
    // Movimenta o peixe
    posX += velX * deltaTime;
    posY += velY * deltaTime;
    posZ += velZ * deltaTime;

    // Rebater nas paredes
    if (posX < -halfWidth || posX > halfWidth)   velX = -velX;
    if (posY < -halfHeight || posY > halfHeight) velY = -velY;
    if (posZ < -halfDepth || posZ > halfDepth)   velZ = -velZ;

    // Emissão de bolhas (ex.: a cada 10s, se quiser)
    bubbleTimer += deltaTime;
    if (bubbleTimer >= 10.0f) {
        //createFishBubble(posX, posY, posZ);
        bubbleTimer = 0.0f;
    }

    // Animação da cauda:
    tailPhase += tailSpeed * deltaTime;  // incrementa a fase
    if (tailPhase > 360.0f) {
        tailPhase -= 360.0f;  // mantém entre 0 e 360
    }
}

void Fish::draw(bool shadowMode) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    // Orienta o peixe na direção do movimento
    float angle = atan2(velZ, velX) * (180.0f / M_PI);
    glRotatef(-angle, 0.0f, 1.0f, 0.0f);

    drawGeometricFish(shadowMode);

    glPopMatrix();
}

void Fish::drawGeometricFish(bool shadowMode) {
    // Corpo principal
    
    if (!shadowMode) {
        glColor3f(1.0f, 0.7f, 0.0f);  // laranja
    }
    glPushMatrix();
        glScalef(1.5f, 0.8f, 0.4f);
        glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

 // Substitua este trecho na drawGeometricFish()
// (no lugar onde desenhava o cone da cauda)

// CAUDA ANIMADA
float rad = tailPhase * (M_PI / 180.0f);
float tailAngle = 20.0f * sin(rad);

if (!shadowMode) {
    glColor3f(1.0f, 0.0f, 0.0f);
    
}

glPushMatrix();
    // Translaciona para trás do corpo
    glTranslatef(-0.45f, 0.0f, 0.0f);

    // Primeiro, rotaciona o “eixo” do tail para alinhar-se a -X
    // (se o corpo está alongado no +X, a cauda é desenhada “atrás”, no -X)
    // Ajuste esses valores se notar que a rotação ficou invertida.
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // Agora, aplica a rotação da animação (balanço)
    glRotatef(tailAngle, 0.0f, 1.0f, 0.0f);

    // Desenhamos a cauda como um shape bidimensional no plano yz
    // (ou seja, x=0 no shape), usando alguns triângulos
    glBegin(GL_TRIANGLES);
        // Parte superior
        // Triângulo esquerdo
        glVertex3f(0.0f,  0.0f,   0.0f);   // vértice perto do corpo
        glVertex3f(0.0f,  0.25f, -0.15f);  // ponta superior esquerda
        glVertex3f(0.0f,  0.25f,  0.15f);  // ponta superior direita

        // Parte inferior
        // Triângulo
        glVertex3f(0.0f,  0.0f,   0.0f);   // vértice perto do corpo
        glVertex3f(0.0f, -0.25f,  0.15f);  // ponta inferior direita
        glVertex3f(0.0f, -0.25f, -0.15f);  // ponta inferior esquerda
    glEnd();
glPopMatrix();



    // Nadadeira superior
    if (!shadowMode) {
        glColor3f(1.0f, 0.7f, 0.0f);
    }
    glPushMatrix();
        glTranslatef(0.0f, 0.20f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeira inferior
    if (!shadowMode) {
        glColor3f(1.0f, 0.7f, 0.0f);
    }
    glPushMatrix();
        glTranslatef(0.0f, -0.18f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1, 0.2, 10, 2);
    glPopMatrix();

    // Nadadeiras laterais (opcional, podemos animar também)
    if (!shadowMode) {
        glColor3f(1.0f, 0.0f, 0.0f);
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
        glutSolidCone(0.08, 0.15, 10, 2);
    glPopMatrix();

    // Olhos (brancos)
    if (!shadowMode) {
        glColor3f(0.0f, 0.0f, 1.0f);
    }
    glPushMatrix();
        glTranslatef(0.35f, 0.05f, 0.12f);
        glutSolidSphere(0.04, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.35f, 0.05f, -0.12f);
        glutSolidSphere(0.04, 10, 10);
    glPopMatrix();

    // Pupilas
    if (!shadowMode) {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
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

void drawFish(bool shadowMode) {
    for (int i = 0; i < NUM_FISH; i++) {
        fishArray[i].draw(shadowMode);
    }
}
