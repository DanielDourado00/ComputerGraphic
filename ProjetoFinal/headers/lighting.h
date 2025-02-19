// lighting.h
#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glut.h>

void initLighting();
void initSpotlight(); // NOVA FUNÇÃO
void setShadowMatrix(GLfloat shadowMat[4][4], const GLfloat groundplane[4], const GLfloat lightpos[4]);
void initFog();

#endif // LIGHTING_H
