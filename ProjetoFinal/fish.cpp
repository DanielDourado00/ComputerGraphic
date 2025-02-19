// fish.cpp
#include "headers/fish.h"
#include "headers/fish3d.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

// Carregaremos o modelo 3D do peixe apenas uma vez
static Fish3D fishModel; 
static bool fishModelLoaded = false; // flag para saber se já carregou

const float halfWidth  = 10.0f / 2.0f;
const float halfHeight = 5.0f  / 2.0f;
const float halfDepth  = 8.0f  / 2.0f;

#define NUM_FISH 5
static Fish fishArray[NUM_FISH];

Fish::Fish() {
    posX = ((float)rand() / RAND_MAX) * (2.0f*halfWidth)  - halfWidth;
    posY = ((float)rand() / RAND_MAX) * (2.0f*halfHeight) - halfHeight;
    posZ = ((float)rand() / RAND_MAX) * (2.0f*halfDepth)  - halfDepth;

    velX = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velY = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
    velZ = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

    bubbleTimer = 0.0f;
}

void Fish::update(float deltaTime) {
    posX += velX * deltaTime;
    posY += velY * deltaTime;
    posZ += velZ * deltaTime;

    // Rebater nas paredes
    if (posX < -halfWidth || posX > halfWidth)   velX = -velX;
    if (posY < -halfHeight || posY > halfHeight) velY = -velY;
    if (posZ < -halfDepth || posZ > halfDepth)   velZ = -velZ;

    // Emissão de bolhas
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
    // Se antes fazia: glRotatef(-angle, 0, 1, 0);
    // Adicione + 180.0f para virar o peixe:
    glRotatef(-angle + 180.0f, 0.0f, 1.0f, 0.0f);
    

    // Se o peixe estiver deitado, rotacione no eixo X
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // Agora 5x menor que 0.125 => 0.025
    glScalef(0.025f, 0.025f, 0.025f);

    fishModel.draw();
    glPopMatrix();
}


// Funções globais
void initializeFish() {
    srand((unsigned int)time(0));

    // Carrega o modelo OBJ do peixe se ainda não foi carregado
    if (!fishModelLoaded) {
        fishModel.loadModel("grafica/fishModel/12265_Fish_v1_L2.obj", "grafica/fishModel/");
        fishModelLoaded = true;
    }

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
        fishArray[i].draw();
    }
}
