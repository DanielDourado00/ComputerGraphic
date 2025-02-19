#include <GL/glut.h>
#include "headers/camera.h"

Camera::Camera() : posX(0.0f), posY(0.0f), posZ(2.0f), yaw(0.0f), pitch(0.0f) {}

void Camera::apply() {
    // Aplica as rotações (pitch e yaw) e a translação
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-posX, -posY, -posZ);
}

