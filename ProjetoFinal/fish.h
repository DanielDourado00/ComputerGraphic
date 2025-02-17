#ifndef FISH_H
#define FISH_H

class Fish {
public:
    float posX, posY, posZ;
    float velX, velY, velZ;
    float bubbleTimer;

    Fish();
    void update(float deltaTime);
    void draw();

private:
    // Adicione esta linha:
    void drawGeometricFish();
};

void initializeFish();
void updateFish(float deltaTime);
void drawFish();

#endif // FISH_H
