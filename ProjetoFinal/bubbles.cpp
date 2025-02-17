// bubbles.cpp
#include "bubbles.h"
#include <cstdlib>
#include <ctime>

// Vetor global que armazena todas as bolhas
static std::vector<Bubble> bubbles;

Bubble::Bubble(float x, float y, float z, float r, float vy)
    : posX(x), posY(y), posZ(z), radius(r), velY(vy) {}

void Bubble::update(float deltaTime) {
    posY += velY * deltaTime;
    // Opcional: a bolha pode aumentar ligeiramente de tamanho ao subir
    radius += 0.01f * deltaTime;
}

void Bubble::draw() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    // Cor branca com transparência para simular a aparência da bolha
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    glutSolidSphere(radius, 8, 8);
    glPopMatrix();
}

void initializeBubbles() {
    // Garante que a semente para números aleatórios esteja iniciada
    srand((unsigned int)time(0));
    bubbles.clear();
}

void createNaturalBubble() {
    // Limites do aquário (consistentes com o módulo do aquário)
    const float halfWidth  = 10.0f / 2.0f;
    const float halfDepth  = 8.0f / 2.0f;
    const float halfHeight = 5.0f / 2.0f;
    
    // Gera posição aleatória no fundo do aquário (Y mínimo)
    float x = ((float)rand() / RAND_MAX) * 10.0f - halfWidth;
    float z = ((float)rand() / RAND_MAX) * 8.0f - halfDepth;
    float y = -halfHeight;  // Fundo
    
    // Define tamanho e velocidade aleatória para a bolha
    float r  = 0.1f + ((float)rand() / RAND_MAX) * 0.1f;
    float vy = 0.1f + ((float)rand() / RAND_MAX) * 0.1f;
    
    bubbles.push_back(Bubble(x, y, z, r, vy));
}

void createFishBubble(float x, float y, float z) {
    // Bolha emitida pelo peixe: posicionado próximo à “boca” do peixe
    float offset = 0.3f;  // Ajusta o ponto de emissão (acima da posição do peixe)
    float r  = 0.05f + ((float)rand() / RAND_MAX) * 0.05f;  // Geralmente menor
    float vy = 0.15f + ((float)rand() / RAND_MAX) * 0.1f;
    
    bubbles.push_back(Bubble(x, y + offset, z, r, vy));
}

void updateBubbles(float deltaTime) {
    // Limite superior do aquário para remoção das bolhas
    const float halfHeight = 5.0f / 2.0f;
    
    // Atualiza cada bolha e remove as que ultrapassam o topo
    for (size_t i = 0; i < bubbles.size(); ) {
        bubbles[i].update(deltaTime);
        if (bubbles[i].posY > halfHeight) {
            bubbles.erase(bubbles.begin() + i);
        } else {
            i++;
        }
    }
    
    // Temporizador para bolhas naturais:
    // A cada 20 segundos, gera exatamente 2 bolhas naturais.
    static float naturalBubbleTimer = 0.0f;
    naturalBubbleTimer += deltaTime;
    if (naturalBubbleTimer >= 20.0f) {
        createNaturalBubble();
        createNaturalBubble();
        naturalBubbleTimer = 0.0f;
    }
}

void drawBubbles() {
    // Ativa blending para efeito de transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (size_t i = 0; i < bubbles.size(); i++) {
        bubbles[i].draw();
    }
    
    glDisable(GL_BLEND);
}
