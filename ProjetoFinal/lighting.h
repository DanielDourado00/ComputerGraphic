// lighting.h
#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glut.h>

// Inicializa a iluminação com uma luz pontual
void initLighting();

// Calcula uma matriz de projeção de sombras dada uma equação de plano e uma posição de luz.
// groundplane: [A, B, C, D] que define o plano (Ax + By + Cz + D = 0)
// lightpos: posição da luz (x, y, z, w)
void setShadowMatrix(GLfloat shadowMat[4][4], const GLfloat groundplane[4], const GLfloat lightpos[4]);

#endif // LIGHTING_H
