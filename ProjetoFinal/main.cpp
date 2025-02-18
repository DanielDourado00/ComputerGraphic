#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
/* Musica */
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
/* Musica */
#include "aquarium.h"
#include "camera.h"
#include "fish.h"
#include "algae.h"
#include "bubbles.h"
#include "lighting.h"
#include "decoration.h"

// Instância global da câmera
Camera camera;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.apply();

    // Desenha objetos internos
    drawDecorations();
    drawAlgae();
    drawFish();
    drawBubbles();

    // Se usar projeção de sombra planar, faça agora:
    // (Desenha sombras no plano y=-2.5)
    GLfloat groundPlane[4] = {0, 1, 0, 2.5f};
    GLfloat lightPos[4]    = {0, 2.5f, 0, 1};
    GLfloat shadowMat[4][4];
    setShadowMatrix(shadowMat, groundPlane, lightPos);

    glPushMatrix();
       glMultMatrixf((GLfloat*)shadowMat);
       glDisable(GL_LIGHTING);
       glColor4f(0,0,0,1);
       drawFish(true); // sombra dos peixes
       glEnable(GL_LIGHTING);
    glPopMatrix();

    // Desenha o aquário (paredes translúcidas) por cima de tudo
    drawAquarium();

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
    const float halfHeight = 5.0f  / 2.0f;
    const float halfDepth  = 8.0f  / 2.0f;

    switch (key) {
        case 27: // ESC para sair
            exit(0);
            break;

        // W -> Frente (ao longo do +Z quando yaw=0)
        case 'w':
        {
            rad = camera.yaw * M_PI / 180.0f;
            camera.posX -= sin(rad) * cameraSpeed;  // gira no X
            camera.posZ -= cos(rad) * cameraSpeed;  // gira no Z
        } break;

        // S -> Trás
        case 's':
        {
            rad = camera.yaw * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += cos(rad) * cameraSpeed;
        } break;

        // A -> Strafe para a esquerda (yaw-90)
        case 'a':
        {
            rad = (camera.yaw - 90.0f) * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += cos(rad) * cameraSpeed;
        } break;

        // D -> Strafe para a direita (yaw+90)
        case 'd':
        {
            rad = (camera.yaw + 90.0f) * M_PI / 180.0f;
            camera.posX += sin(rad) * cameraSpeed;
            camera.posZ += cos(rad) * cameraSpeed;
        } break;

        // Q -> subir
        case 'q':
            camera.posY += cameraSpeed;
            break;

        // E -> descer
        case 'e':
            camera.posY -= cameraSpeed;
            break;
    }

    // Garante que a câmera não saia dos limites do aquário
    if (camera.posX < -halfWidth)  camera.posX = -halfWidth;
    if (camera.posX >  halfWidth)  camera.posX =  halfWidth;
    if (camera.posY < -halfHeight) camera.posY = -halfHeight;
    if (camera.posY >  halfHeight) camera.posY =  halfHeight;
    if (camera.posZ < -halfDepth)  camera.posZ = -halfDepth;
    if (camera.posZ >  halfDepth)  camera.posZ =  halfDepth;

    glutPostRedisplay();
}



// Função de timer para atualizar o estado da cena (peixes, bolhas, algas)
void timer(int value) {
    updateFish(0.1f);
    updateBubbles(0.1f);
    updateAlgae(0.1f);
    updateDecorations(0.1f);
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // Aproximadamente 60 FPS
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            // girar para a esquerda (aumentar yaw ou diminuir?)
            camera.yaw += 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            // girar para a direita
            camera.yaw -= 2.0f;
            break;
        case GLUT_KEY_UP:
            // olhar para cima ou diminuir pitch
            camera.pitch += 2.0f;
            break;
        case GLUT_KEY_DOWN:
            // olhar para baixo ou aumentar pitch
            camera.pitch -= 2.0f;
            break;
    }
    glutPostRedisplay();
}



//==============================================MUSICA==============================================

// Variável global para a música
static Mix_Music* bgMusic = nullptr;

bool initAudio() {
    // Inicializa o subsistema de áudio do SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Inicializa SDL_mixer com suporte a MP3
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadAndPlayMusic(const char* mp3File) {
    std::cout << "Tentando carregar " << mp3File << std::endl;
    bgMusic = Mix_LoadMUS(mp3File);
    if (!bgMusic) {
        std::cerr << "Falha ao carregar música: " << mp3File 
                  << " -> " << Mix_GetError() << std::endl;
        return false;
    }
    // Toca a música em loop infinito ( -1 )
    if (Mix_PlayMusic(bgMusic, -1) == -1) {
        std::cerr << "Erro ao reproduzir música: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeAudio() {
    // Para a música
    Mix_HaltMusic();

    // Libera o recurso de música
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
    }

    // Fecha o audio
    Mix_CloseAudio();
    SDL_Quit(); // se você não usa SDL para mais nada, finalize o SDL
}

bool initAudio();
bool loadAndPlayMusic(const char* mp3File);
void closeAudio();


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Aquário 3D");

    glEnable(GL_DEPTH_TEST);

    // ---> ADICIONE ISTO <---
    if (!initAudio()) {
        std::cerr << "Falha ao inicializar áudio! Prosseguindo sem som...\n";
    } else {
        // Tente tocar a música
        if (!loadAndPlayMusic("music/musicjapao.mp3")) {
            std::cerr << "Falha ao carregar/tocar música. Prosseguindo sem som...\n";
        }
    }
  

    // Inicializa iluminação básica + Spotlight + Fog
    initLighting();
    initSpotlight(); // Adiciona o holofote (GL_LIGHT1)
    initFog();

    // Inicializa as texturas, algas, peixes, bolhas
    initializeAquarium();
    initializeAlgae();
    initializeFish();
    initializeBubbles();
    // Inicializa as decorações 3D
    initializeDecorations();

    // Registra os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

   //fecha ao sair
    closeAudio();

    return 0;
}
