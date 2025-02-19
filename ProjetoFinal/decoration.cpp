#define TINYOBJLOADER_IMPLEMENTATION
#include "headers/tiny_obj_loader.h" //libs para carregar objetos 3D

#include "headers/decoration.h"
#include "grafica/soil/include/SOIL/SOIL.h"
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

    float minX = 999999.0f, maxX = -999999.0f;  // para cálculo de centro
    float minY = 999999.0f, maxY = -999999.0f;  
    float minZ = 999999.0f, maxZ = -999999.0f;

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
    
    {
        Decoration terrain;

        terrain.loadOBJ("grafica/terreno/map_697494cf-ea4b-4cf1-8341-dd55369a8c5e_densified_mesh_textured.obj");

        terrain.setTexture("grafica/terreno/map_697494cf-ea4b-4cf1-8341-dd55369a8c5e_densified_mesh_textured_4096.jpg");
        
        // Ajuste a posição para onde você quer exibir no aquário
        terrain.posX = 0.0f;
        terrain.posY = 0.75f;   // em ordem crescente: 0.0 0.25 0.5 0.75 1.0
        terrain.posZ = 0.0f;
        
        // Ajuste a escala conforme necessário
        terrain.scale = 2.3f;   // experimente valores maiores ou menores
        // Se o modelo vier deitado, rotacione no eixo X:
        terrain.rotX = 0.0f;  
        terrain.rotY = 0.0f;
        terrain.rotZ = 0.0f;

        decorations.push_back(terrain);
    }
    

    // Exemplo: Mergulhador
    {
        Decoration diver;
        diver.loadOBJ("grafica/mergulhador/13018_Aquarium_Deep_Sea_Diver_v1_L1.obj");
        diver.posX = 2.2f;
        diver.posY = -2.0f; 
        diver.posZ = 0.45f;
        diver.scale = 0.15f;
        diver.rotX = -79.0f;
        diver.rotY  = 0.95f; // gire conforme precise, para girar para esquerda é 
        diver.rotZ = -0.45f;
        

        // Carrega a textura do mergulhador (JPG)
        diver.setTexture("grafica/mergulhador/13018_Aquarium_Deep_Sea_Diver_diff.jpg");

        decorations.push_back(diver);
    }
    {
        Decoration boat;
        // Carrega o OBJ do navio
        boat.loadOBJ("grafica/navio/Marlow66.obj");
    
        // Posição, escala e rotação para que fique visível
        boat.posX = -2.0f;
        boat.posY = -2.0f; // se o fundo do aquário é -2.5
        boat.posZ = 1.0f;
        boat.scale = 0.3f;  // teste valores
        boat.rotY  = 120.0f; // gire conforme precise
        boat.rotX = 0.125f;
    
        // Força uma única textura (por exemplo, a da madeira)
        boat.setTexture("grafica/navio/FineWood.jpg");
    
        decorations.push_back(boat);
    }
    

    }



void drawDecorations() {
    for (auto &dec : decorations) {
        if (!dec.useTexture) {
            // Se o objeto não tem textura, garanta que a cor seja branca
            glColor3f(1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        }
        dec.draw();
    }
}

