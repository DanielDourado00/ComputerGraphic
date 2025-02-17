// algae.cpp
#include "algae.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

// Vetor global para armazenar as algas
static std::vector<Algae> algaeList;

Algae::Algae(float x, float z, float height, float amplitude, float swaySpeed)
    : baseX(x), baseZ(z), height(height), amplitude(amplitude), swaySpeed(swaySpeed), swayPhase(0.0f) {}

void Algae::update(float deltaTime) {
    // Atualiza a fase do balanço
    swayPhase += swaySpeed * deltaTime;
}

void Algae::draw() {
    // Desenha a alga como um polyline com 5 segmentos (quanto mais segmentos, mais suave a curva)
    const int segments = 5;
    // Supondo que o fundo do aquário esteja em y = -2.5 (para um aquário de altura 5.0)
    float baseY = -2.5f;
    
    glColor3f(0.0f, 0.8f, 0.0f);  // Verde vibrante
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = static_cast<float>(i) / segments;
        float y = baseY + t * height;
        // O deslocamento horizontal aumenta com a altura (a base permanece fixa)
        float offset = amplitude * t * sin(swayPhase + t * 3.1415f);
        glVertex3f(baseX + offset, y, baseZ);
    }
    glEnd();
}

void initializeAlgae() {
    algaeList.clear();
    srand(static_cast<unsigned int>(time(0)));
    
    const float halfWidth = 10.0f / 2.0f;
    const float halfDepth = 8.0f / 2.0f;
    // Cria, por exemplo, 10 algas espalhadas aleatoriamente pelo fundo do aquário
    for (int i = 0; i < 10; i++) {
        float x = ((float)rand() / RAND_MAX) * 10.0f - halfWidth;
        float z = ((float)rand() / RAND_MAX) * 8.0f - halfDepth;
        float height = 1.5f + ((float)rand() / RAND_MAX) * 1.0f;    // Altura entre 1.5 e 2.5
        float amplitude = 0.1f + ((float)rand() / RAND_MAX) * 0.2f; // Amplitude entre 0.1 e 0.3
        float swaySpeed = 0.5f + ((float)rand() / RAND_MAX) * 0.5f; // Velocidade entre 0.5 e 1.0
        algaeList.push_back(Algae(x, z, height, amplitude, swaySpeed));
    }
}

void updateAlgae(float deltaTime) {
    for (size_t i = 0; i < algaeList.size(); i++) {
        algaeList[i].update(deltaTime);
    }
}

void drawAlgae() {
    for (size_t i = 0; i < algaeList.size(); i++) {
        algaeList[i].draw();
    }
}
