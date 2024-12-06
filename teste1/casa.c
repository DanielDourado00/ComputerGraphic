#include <GLFW/glfw3.h>
#include <math.h>

// Função para desenhar um quadrado
void desenhaQuadrado(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);  // Define a cor
    glBegin(GL_QUADS);
        glVertex2f(x, y);                // Canto inferior esquerdo
        glVertex2f(x + width, y);        // Canto inferior direito
        glVertex2f(x + width, y + height); // Canto superior direito
        glVertex2f(x, y + height);       // Canto superior esquerdo
    glEnd();
}

// Função para desenhar um triângulo
void desenhaTriangulo(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);  // Define a cor
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);                 // Ponto inferior esquerdo
        glVertex2f(x + width, y);         // Ponto inferior direito
        glVertex2f(x + width / 2, y + height); // Ponto superior (meio)
    glEnd();
}

// Função para desenhar um círculo
void desenhaCirculo(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy); // Centro do círculo
        int num_segments = 100; // Aumente para suavizar o círculo
        for (int i = 0; i <= num_segments; i++) {
            float angle = i * 2.0f * 3.14 / num_segments;
            float x = cx + (cos(angle) * radius);
            float y = cy + (sin(angle) * radius);
            glVertex2f(x, y);
        }
    glEnd();
}

int main(void) {
    // Inicializa o GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Cria uma janela e o contexto OpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "Desenho da Casa com Árvore", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela
        glClear(GL_COLOR_BUFFER_BIT);

        // Configura o sistema de coordenadas para o modo 2D
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);

        // Desenha a casa com as formas

        // Corpo da casa
        desenhaQuadrado(-0.3f, -0.5f, 0.65f, 0.65f, 0.5f, 0.5f, 0.5f);  // Cinza

        // Porta        
        desenhaQuadrado(-0.10f, -0.5f, 0.1f, 0.25f, 0.7f, 0.9f, 1.0f);  // Azul claro

        // Janela
        desenhaQuadrado(0.119f, -0.2f, 0.2f, 0.2f, 0.6f, 0.9f, 1.0f);  // Azul claro
       
        // Chaminé
        //                X     Y    Larg Altura R    G    B
        desenhaQuadrado(-0.25f, 0.15f, 0.1f, 0.5f, 0.4f, 0.2f, 0.0f);  // Marrom
                        
        // Telhado (ajustado para alinhar com a casa)
        desenhaTriangulo(-0.3f, 0.15f, 0.65f, 0.4f, 1.0f, 0.5f, 0.0f);  // Laranja


        // Desenha uma árvore ao lado da casa
        // Tronco
        desenhaQuadrado(-0.8f, -0.5f, 0.1f, 0.3f, 0.5f, 0.35f, 0.05f); // Marrom escuro
        // Copa (círculo)
        desenhaCirculo(-0.75f, -0.1f, 0.2f, 0.0f, 0.5f, 0.0f); // Verde escuro
        desenhaCirculo(-0.85f, -0.05f, 0.2f, 0.0f, 0.5f, 0.0f); // Verde escuro
        desenhaCirculo(-0.65f, -0.05f, 0.2f, 0.0f, 0.5f, 0.0f); // Verde escuro

        // Troca os buffers e processa eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//para rodar basta digitar no terminal: gcc casa.c -o casa -lglfw -lGL -lX11 -lm