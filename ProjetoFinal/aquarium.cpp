// aquarium.cpp
#include <GL/glut.h>
#include "aquarium.h"

void drawAquarium() {
    // Dimensões do aquário
    float width = 10.0f;
    float height = 5.0f;
    float depth = 8.0f;
    
    glPushMatrix();

    // Ativa blending para transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Desenha as faces do aquário (sem a tampa superior, para simular a abertura)
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f);  // Azul com transparência

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
    
    // Face inferior (fundo)
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(width/2, -height/2, -depth/2);
    glVertex3f(width/2, -height/2, depth/2);
    glVertex3f(-width/2, -height/2, depth/2);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Opcional: desenhar as arestas do aquário para dar um efeito de "estrutura"
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
      // Exemplo: linhas da face frontal
      glVertex3f(-width/2, -height/2, depth/2);
      glVertex3f(width/2, -height/2, depth/2);
      
      glVertex3f(width/2, -height/2, depth/2);
      glVertex3f(width/2, height/2, depth/2);
      
      glVertex3f(width/2, height/2, depth/2);
      glVertex3f(-width/2, height/2, depth/2);
      
      glVertex3f(-width/2, height/2, depth/2);
      glVertex3f(-width/2, -height/2, depth/2);
      // Você pode adicionar as demais arestas conforme necessário
    glEnd();
    
    glPopMatrix();
}
