// main.cpp
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include "aquarium.h"
#include "camera.h"
#include "fish.h"
#include "algae.h"
#include "bubbles.h"
#include "lighting.h"

// Instância global da câmera
Camera camera;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Aplica as transformações da câmera
    camera.apply();

    // Desenha o aquário e os elementos
    drawAquarium();
    drawAlgae();
    drawFish();
    drawBubbles();
    

    // --- Renderização de sombras ---
    // Definindo o plano para a projeção da sombra:
    // Nosso aquário tem altura 5.0, portanto o fundo está em y = -2.5.
    // A equação do plano é: 0*x + 1*y + 0*z + 2.5 = 0  (ou seja, y + 2.5 = 0)
    GLfloat groundPlane[4] = { 0.0f, 1.0f, 0.0f, 2.5f }; // Plano do fundo do aquário
    // A mesma posição de luz definida em initLighting
    GLfloat lightPos[4]   = { 0.0f, 2.0f, 2.0f, 1.0f }; // Posição da luz
    GLfloat shadowMat[4][4];
    setShadowMatrix(shadowMat, groundPlane, lightPos);

    // Salva a matriz atual e multiplica pela matriz de sombra
    glPushMatrix();
    glMultMatrixf((GLfloat*)shadowMat);

    // Desabilita a iluminação para desenhar as sombras com cor escura
    glDisable(GL_LIGHTING);
// Desabilita o blending para garantir que a sombra seja preta sem mistura
glDisable(GL_BLEND);
glColor4f(0.0f, 0.0f, 0.0f, 1.0f); // sombra preta 
drawFish();
glEnable(GL_LIGHTING);

    // Restaura o estado original
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    const float cameraSpeed = 0.2f;
    float rad;
    
    // Limites do aquário
    const float halfWidth  = 10.0f / 2.0f;
    const float halfHeight = 5.0f / 2.0f;
    const float halfDepth  = 8.0f / 2.0f;
    
    switch (key) {
        case 27: // ESC para sair
            exit(0);
            break;
        case 'w': // mover para frente
            rad = camera.yaw * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += -cos(rad) * cameraSpeed;
            break;
        case 's': // mover para trás
            rad = camera.yaw * M_PI / 180.0f;
            camera.posX -= sin(rad) * cameraSpeed;
            camera.posZ -= -cos(rad) * cameraSpeed;
            break;
        case 'a': // mover para a esquerda (strafe)
            rad = (camera.yaw - 90.0f) * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += -cos(rad) * cameraSpeed;
            break;
        case 'd': // mover para a direita (strafe)
            rad = (camera.yaw + 90.0f) * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += -cos(rad) * cameraSpeed;
            break;
        case 'q': // subir
            camera.posY += cameraSpeed;
            break;
        case 'e': // descer
            camera.posY -= cameraSpeed;
            break;
    }
    
    // Garante que a câmera não saia dos limites do aquário
    if (camera.posX < -halfWidth)  camera.posX = -halfWidth;
    if (camera.posX > halfWidth)   camera.posX = halfWidth;
    if (camera.posY < -halfHeight) camera.posY = -halfHeight;
    if (camera.posY > halfHeight)  camera.posY = halfHeight;
    if (camera.posZ < -halfDepth)  camera.posZ = -halfDepth;
    if (camera.posZ > halfDepth)   camera.posZ = halfDepth;
    
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    camera.processMouseMotion(x, y);
    glutPostRedisplay();
}

// Função de timer para atualizar o estado da cena (peixes e bolhas)
void timer(int value) {
    updateFish(0.1f);
    updateBubbles(0.1f);
    updateAlgae(0.1f);  // Atualiza o movimento das algas
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // Aproximadamente 60 FPS
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Aquário 3D");

    glEnable(GL_DEPTH_TEST);

    // Inicializa os módulos
    initLighting();
    initializeAlgae();
    initializeFish();
    initializeBubbles();

    // Registra os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}

