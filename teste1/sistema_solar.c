#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// IDs para as texturas dos planetas e do fundo estrelado
GLuint texSol, texMercurio, texVenus, texTerra, texMarte, texStars;

// Ângulos de órbita (ajustados para serem mais lentos)
float angleMercury = 0.0f;
float angleVenus = 0.0f;
float angleEarth = 0.0f;
float angleMars = 0.0f;

// Função para carregar as texturas e verificar se foram carregadas corretamente
void carregarTexturas() {
    texSol = SOIL_load_OGL_texture("textures/sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texMercurio = SOIL_load_OGL_texture("textures/mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texVenus = SOIL_load_OGL_texture("textures/venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texTerra = SOIL_load_OGL_texture("textures/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texMarte = SOIL_load_OGL_texture("textures/mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texStars = SOIL_load_OGL_texture("textures/stars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (!texSol || !texMercurio || !texVenus || !texTerra || !texMarte || !texStars) {
        printf("Erro ao carregar as texturas.\n");
        exit(1);
    }
}

// Função para desenhar o fundo estrelado (Via Láctea)
void desenhaFundoEstrelado() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texStars);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, -90.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -100.0f, -90.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, -90.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 100.0f, -90.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Função para desenhar órbitas circulares
void desenhaOrbita(float distancia) {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * 3.14 * (float)i / (float)100;
        float x = distancia * cosf(theta);
        float z = distancia * sinf(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

// Função para desenhar um planeta com textura e órbita
void desenhaPlaneta(float distancia, float tamanho, float angulo, GLuint textura) {
    glPushMatrix();
        glRotatef(angulo, 0.0f, 1.0f, 0.0f);
        glTranslatef(distancia, 0.0f, 0.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura);

        glColor3f(1.0f, 1.0f, 1.0f); // Branco para permitir que a textura tenha cor total
        glutSolidSphere(tamanho, 30, 30);  // Aumentando a resolução da esfera

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Função principal para desenhar o sistema solar
void desenhaSistemaSolar() {
    // Desenha o Sol
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texSol);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(2.0f, 50, 50);
    glDisable(GL_TEXTURE_2D);

    // Desenha as órbitas
    desenhaOrbita(5.0f); // Órbita de Mercúrio
    desenhaOrbita(7.0f); // Órbita de Vênus
    desenhaOrbita(10.0f); // Órbita da Terra
    desenhaOrbita(13.0f); // Órbita de Marte

    // Desenha os planetas com suas texturas e órbitas
    desenhaPlaneta(5.0f, 0.2f, angleMercury, texMercurio); // Mercúrio
    desenhaPlaneta(7.0f, 0.4f, angleVenus, texVenus);      // Vênus
    desenhaPlaneta(10.0f, 0.5f, angleEarth, texTerra);     // Terra
    desenhaPlaneta(13.0f, 0.3f, angleMars, texMarte);      // Marte
}

// Função para atualizar os ângulos de órbita dos planetas
void atualizaAngulos() {
    angleMercury += 0.2f;  // Velocidade de rotação mais lenta
    angleVenus += 0.1f;
    angleEarth += 0.05f;
    angleMars += 0.03f;

    if (angleMercury > 360) angleMercury -= 360;
    if (angleVenus > 360) angleVenus -= 360;
    if (angleEarth > 360) angleEarth -= 360;
    if (angleMars > 360) angleMars -= 360;
}

// Função de exibição para desenhar a cena completa
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 10.0f, 30.0f,  // Posição da câmera
              0.0f, 0.0f, 0.0f,    // Ponto para onde a câmera aponta
              0.0f, 1.0f, 0.0f);   // Vetor "up" da câmera

    desenhaFundoEstrelado();  // Desenha o fundo estrelado
    desenhaSistemaSolar();    // Desenha o sistema solar
    atualizaAngulos();        // Atualiza as órbitas dos planetas

    glutSwapBuffers();
}

// Função de inicialização do ambiente
void inicializa() {
    glEnable(GL_DEPTH_TEST);  // Habilita teste de profundidade
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Cor de fundo (preto para o espaço)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);  // Configura perspectiva
    glMatrixMode(GL_MODELVIEW);

    carregarTexturas(); // Carrega as texturas dos planetas e do Sol
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sistema Solar 3D Detalhado");

    inicializa();

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}