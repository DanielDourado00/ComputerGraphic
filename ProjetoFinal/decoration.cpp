#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include "decoration.h"

// Vetor global de objetos decorativos
static std::vector<Decoration> decorations;

Decoration::Decoration()
    : posX(0.0f), posY(0.0f), posZ(0.0f),
      scale(1.0f), rotY(0.0f) { }

bool Decoration::loadOBJ(const std::string& filename) {
    tinyobj::attrib_t attrib; 
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; // não usaremos agora, mas se quiser usar materiais, eles ficam aqui

    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        std::cerr << "TinyObj WARNING: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "TinyObj ERROR: " << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Falha ao carregar OBJ: " << filename << std::endl;
        return false;
    }

    // Limpa dados prévios
    vertices.clear();
    normals.clear();
    texcoords.clear();

    // Para cada shape (obj pode ter vários "shapes" ou subobjetos)
    for (size_t s = 0; s < shapes.size(); s++) {
        // Para cada face (índices)
        size_t indexOffset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f]; // geralmente 3 para triângulos

            // Para cada vértice da face
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                // Positions
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                // Normals (se existir)
                if (idx.normal_index >= 0 && (size_t)(3*idx.normal_index + 2) < attrib.normals.size()) {
                    float nx = attrib.normals[3*idx.normal_index + 0];
                    float ny = attrib.normals[3*idx.normal_index + 1];
                    float nz = attrib.normals[3*idx.normal_index + 2];
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                } else {
                    // Se não existir, use 0,0,0 ou calcule depois
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                }

                // Texcoords (se quiser)
                if (idx.texcoord_index >= 0 && (size_t)(2*idx.texcoord_index + 1) < attrib.texcoords.size()) {
                    float tx = attrib.texcoords[2*idx.texcoord_index + 0];
                    float ty = attrib.texcoords[2*idx.texcoord_index + 1];
                    texcoords.push_back(tx);
                    texcoords.push_back(ty);
                } else {
                    texcoords.push_back(0.0f);
                    texcoords.push_back(0.0f);
                }
            }
            indexOffset += fv;
        }
    }

    return true;
}

void Decoration::draw() {
    glPushMatrix();
        // Aplica posição, rotação e escala
        glTranslatef(posX, posY, posZ);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        glScalef(scale, scale, scale);

        // Se os normais estiverem válidos, habilite lighting
        // e use GL_SMOOTH shading para aproveitar
        // ou já está habilitado globalmente

        // Modo simples: immediate
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < vertices.size(); i += 3) {
            // Normais
            if (i < normals.size()) {
                glNormal3f(normals[i], normals[i+1], normals[i+2]);
            }
            // Se quiser textura, chamaria: glTexCoord2f(texcoords[...], texcoords[...]);
            glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
        }
        glEnd();
    glPopMatrix();
}

// --------------- Funções globais ---------------
void initializeDecorations() {
    // Exemplo: criamos uma rocha, um baú, etc.
    {
        Decoration rock;
        // Carrega o arquivo OBJ de uma rocha
        rock.loadOBJ("models/seashell_obj.obj");  // Ajuste o caminho conforme necessário
        rock.posX = 2.0f;  // posicionado em (2, -2.5, 0) no fundo
        rock.posY = -2.5f;
        rock.posZ = 0.0f;
        rock.scale = 0.5f;
        rock.rotY  = 0.0f;
        decorations.push_back(rock);
    }

    {
        Decoration chest;
        chest.loadOBJ("models/chest.obj");
        chest.posX = -3.0f;
        chest.posY = -2.5f;
        chest.posZ = 1.5f;
        chest.scale = 0.2f;
        chest.rotY  = 45.0f;
        decorations.push_back(chest);
    }

    // E assim por diante para cada objeto
}

void updateDecorations(float /*deltaTime*/) {
    // Se quiser animar algo, rotacionar, etc., faça aqui
    // Exemplo: girar uma estátua
    // decorations[1].rotY += 10.0f * deltaTime;
}

void drawDecorations() {
    for (size_t i = 0; i < decorations.size(); i++) {
        decorations[i].draw();
    }
}
