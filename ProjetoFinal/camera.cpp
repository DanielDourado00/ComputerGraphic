// camera.cpp
#include <GL/glut.h>
#include "camera.h"

Camera::Camera() : posX(0.0f), posY(0.0f), posZ(2.0f), yaw(0.0f), pitch(0.0f) {}

void Camera::apply() {
    // Aplica as rotações (pitch e yaw) e a translação
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-posX, -posY, -posZ);
}

void Camera::processMouseMotion(int x, int y) {
    // Atualiza a orientação da câmera com base no movimento do mouse
    static int lastX = x, lastY = y;
    int dx = x - lastX;
    int dy = y - lastY;
    lastX = x;
    lastY = y;
    
    yaw   += dx * 0.1f;
    pitch += dy * 0.1f;
}
