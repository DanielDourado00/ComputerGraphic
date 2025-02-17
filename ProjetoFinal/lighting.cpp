// lighting.cpp
#include "lighting.h"

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configuração dos parâmetros da luz pontual
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

void setShadowMatrix(GLfloat shadowMat[4][4], const GLfloat groundplane[4], const GLfloat lightpos[4]) {
    // Calcula o produto escalar entre o plano e a posição da luz
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
