// camera.h
#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    float posX, posY, posZ;
    float yaw, pitch;
    
    Camera();
    void apply();
    void processMouseMotion(int x, int y);
};

#endif // CAMERA_H
