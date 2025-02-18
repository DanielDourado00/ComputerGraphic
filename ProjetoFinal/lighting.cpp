// lighting.cpp
#include "lighting.h"
#include <cstdio>

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configuração dos parâmetros da luz pontual (já existente)
    GLfloat ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 0.0f, 2.0f, 2.0f, 1.0f }; // luz pontual

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Permite que a cor dos objetos defina suas propriedades de material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT, GL_SHININESS, 64);
}

void initSpotlight() {
    glEnable(GL_LIGHT1);

    // Cor da luz do holofote
    GLfloat ambient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat diffuse[]  = { 0.9f, 0.9f, 0.8f, 1.0f };
    GLfloat specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    // Posição do holofote (acima do aquário). (x=0, y=2.5, z=0), w=1.0 => luz pontual
    GLfloat position[] = { 0.0f, 2.5f, 0.0f, 1.0f };

    // Direção do cone de luz (apontando para baixo)
    GLfloat direction[] = { 0.0f, -1.0f, 0.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

    // Posição e direção
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);

    // Abertura do cone (ângulo em graus: 0 a 90)
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);

    // Concentração do holofote (0 = espalhado, >100 = muito concentrado)
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f);

    // Atenuação (quanto maior, mais rápido decai a luz)
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,   0.1f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,0.05f);
}

void setShadowMatrix(GLfloat shadowMat[4][4], const GLfloat groundplane[4], const GLfloat lightpos[4]) {
    GLfloat dot = groundplane[0]*lightpos[0] + groundplane[1]*lightpos[1] +
                  groundplane[2]*lightpos[2] + groundplane[3]*lightpos[3];

    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.0f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.0f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.0f - lightpos[0] * groundplane[3];

    shadowMat[0][1] = 0.0f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.0f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.0f - lightpos[1] * groundplane[3];

    shadowMat[0][2] = 0.0f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.0f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.0f - lightpos[2] * groundplane[3];

    shadowMat[0][3] = 0.0f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.0f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.0f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
}

// NOVA FUNÇÃO: configura a neblina
void initFog() {
    glEnable(GL_FOG);

    // Ajuste a cor do fog para um azul que combine com o aquário
    GLfloat fogColor[4] = {0.0f, 0.5f, 0.7f, 1.0f}; 
    // (você pode mudar o tom conforme desejar)

    glFogfv(GL_FOG_COLOR, fogColor);
    // Ajusta a cor de limpeza de tela para harmonizar com a neblina
    glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);

    // Definindo o modo da neblina como linear:
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.15f);
    glFogf(GL_FOG_START, 5.0f);   // onde a neblina começa a afetar os objetos
    glFogf(GL_FOG_END, 20.0f);    // onde a neblina é total
    // Se quiser mais densidade, diminua o FOG_END ou use GL_EXP2/GL_EXP e glFogf(GL_FOG_DENSITY, 0.02f);

    glHint(GL_FOG_HINT, GL_NICEST);
}

