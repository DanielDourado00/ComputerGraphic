// decoration.cpp

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "decoration.h"
#include "soil/include/SOIL/SOIL.h"
#include <iostream>

// Vetor global de objetos decorativos
static std::vector<Decoration> decorations;

Decoration::Decoration()
  : posX(0.0f), posY(0.0f), posZ(0.0f),
    scale(1.0f),
    rotX(0.0f), rotY(0.0f), rotZ(0.0f),  // agora zeramos os 3 eixos
    textureID(0), useTexture(false)
{
}

bool Decoration::loadOBJ(const std::string& filename) {
    tinyobj::attrib_t attrib; 
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    // Carrega o OBJ (triangulando automaticamente)
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                filename.c_str(), nullptr, true);
    if (!warn.empty()) {
        std::cerr << "TinyOBJ WARNING: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "TinyOBJ ERROR: " << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Falha ao carregar OBJ: " << filename << std::endl;
        return false;
    }

    vertices.clear();
    normals.clear();
    texcoords.clear();

    // Para cada shape (obj pode ter vários subobjetos)
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t indexOffset = 0;
        // Para cada face
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f]; // geralmente 3 (triângulos)

            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                // Posição
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                // Normal
                if (idx.normal_index >= 0 && 
                    (size_t)(3*idx.normal_index+2) < attrib.normals.size()) {
                    float nx = attrib.normals[3*idx.normal_index + 0];
                    float ny = attrib.normals[3*idx.normal_index + 1];
                    float nz = attrib.normals[3*idx.normal_index + 2];
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                } else {
                    // Se não tiver normal, coloque (0,0,0)
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                }

                // Coordenadas de textura (UV)
                if (idx.texcoord_index >= 0 &&
                    (size_t)(2*idx.texcoord_index+1) < attrib.texcoords.size()) {
                    float tu = attrib.texcoords[2*idx.texcoord_index + 0];
                    float tv = attrib.texcoords[2*idx.texcoord_index + 1];
                    texcoords.push_back(tu);
                    texcoords.push_back(tv);
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

bool Decoration::setTexture(const std::string& texturePath) {
    // Carrega a textura via SOIL
    GLuint texID = SOIL_load_OGL_texture(
        texturePath.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (!texID) {
        std::cerr << "Erro ao carregar textura: " << texturePath 
                  << " -> " << SOIL_last_result() << std::endl;
        return false;
    }
    // Configura parâmetros básicos
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Salva no objeto
    textureID = texID;
    useTexture = true;

    // Desvincula
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Decoration::draw() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    // Aplique a rotação em X, Y, Z nesta ordem:
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    glScalef(scale, scale, scale);

    if (useTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        // Normal
        if (i < normals.size()) {
            glNormal3f(normals[i], normals[i+1], normals[i+2]);
        }

        // TexCoords
        if (useTexture) {
            size_t texIndex = (i/3)*2;
            if (texIndex + 1 < texcoords.size()) {
                glTexCoord2f(texcoords[texIndex], texcoords[texIndex+1]);
            }
        }
        // Vértice
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
    glEnd();

    if (useTexture) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}


// -------------------------------------------------------------
// Funções globais (gerenciamento de várias decorações)

void initializeDecorations() {
    
    // Exemplo: criamos uma rocha, um baú, etc.
    {
        Decoration rock;
        rock.loadOBJ("models/seashell_obj.obj");
        rock.posX = 2.0f;
        rock.posY = -2.3f;
        rock.posZ = 0.0f;
        rock.scale = 0.5f;
    
        rock.rotX = 0.0f; // sem girar no X
        rock.rotY = 0.0f; // sem girar no Y
        rock.rotZ = 0.0f; // sem girar no Z
    
        decorations.push_back(rock);
    }
    

    // Exemplo: Mergulhador
    {
        Decoration diver;
        diver.loadOBJ("mergulhador/13018_Aquarium_Deep_Sea_Diver_v1_L1.obj");
        diver.posX = 2.0f;
        diver.posY = -1.5f; 
        diver.posZ = 0.0f;
        diver.scale = 0.1f;
        diver.rotX = -90.0f;
        diver.rotY  = 0.0f;
        

        // Carrega a textura do mergulhador (JPG)
        diver.setTexture("mergulhador/13018_Aquarium_Deep_Sea_Diver_diff.jpg");

        decorations.push_back(diver);
    }
    // ...
}

void updateDecorations(float /*deltaTime*/) {
    // Caso queiramos animar...
}

void drawDecorations() {
    for (auto &dec : decorations) {
        dec.draw();
    }
}
