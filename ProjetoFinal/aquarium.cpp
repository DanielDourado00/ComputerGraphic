#include <GL/glut.h>
#include <cstdio>
#include "soil/include/SOIL/SOIL.h"
#include "aquarium.h"

// Variável global para armazenar o ID da textura (fundo)
static GLuint sandTextureID;

void initializeAquarium() {
    // Carregando a textura de areia com SOIL
    sandTextureID = SOIL_load_OGL_texture(
        "image/areia.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y  // inverte a imagem verticalmente, se precisar
    );

    if (sandTextureID == 0) {
        // Caso haja erro no carregamento, exibir mensagem (opcional)
        printf("Erro ao carregar a textura areia.png: %s\n", SOIL_last_result());
    }

    // Configura parâmetros da textura
    glBindTexture(GL_TEXTURE_2D, sandTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ou GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Opcional: se quiser repetir a textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Se quiser mipmaps automáticos:
    // gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // Ou se estiver usando versões mais modernas do OpenGL: glGenerateMipmap(GL_TEXTURE_2D);

    // Desvincula a textura por segurança
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Dimensões do aquário, iguais às que você usa em drawAquarium()
static float width  = 10.0f;
static float height = 5.0f;
static float depth  = 8.0f;

void drawAquarium() {
    // Limpa e configura
    glPushMatrix();

    // Ativa blending/transparência (caso ainda queira paredes translúcidas)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Desenha as paredes (opcionalmente translúcidas)
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f);
    glBegin(GL_QUADS);
        // Face frontal
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);

        // Face traseira
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        glVertex3f(-width/2, height/2, -depth/2);

        // Face esquerda
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, -depth/2);

        // Face direita
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(width/2, height/2, -depth/2);
    glEnd();

    /* // Face inferior com textura de areia
    glDisable(GL_BLEND); // desativa blending para o chão texturizado
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTextureID);

    // Ajustar repetição da textura (exemplo):
    float repeatX = 2.0f; // repete 2 vezes
    float repeatZ = 2.0f;

    glBegin(GL_QUADS); // Face inferior
        // Coord. de textura (s, t) associadas ao fundo
        glColor3f(1.0f, 1.0f, 1.0f);  // branco para não alterar a textura
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-width/2, -height/2, -depth/2);

        glTexCoord2f(repeatX, 0.0f);
        glVertex3f(width/2, -height/2, -depth/2);

        glTexCoord2f(repeatX, repeatZ);
        glVertex3f(width/2, -height/2, depth/2);

        glTexCoord2f(0.0f, repeatZ);
        glVertex3f(-width/2, -height/2, depth/2);
    glEnd();

    glDisable(GL_TEXTURE_2D); */

    // Opcional: desenhar as arestas do aquário
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_LINES);
        // linhas da face frontal
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(width/2, -height/2, depth/2);

        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);

        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);

        glVertex3f(-width/2, height/2, depth/2);
        glVertex3f(-width/2, -height/2, depth/2);
        // etc. para as outras arestas, se quiser
    glEnd();

    glPopMatrix();
}
