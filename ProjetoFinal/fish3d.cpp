#include "fish3d.h"
#include "tiny_obj_loader.h"
#include "soil/include/SOIL/SOIL.h"
#include <iostream>

Fish3D::Fish3D()
    : textureID(0), hasTexture(false)
{
}

bool Fish3D::loadModel(const std::string& objPath, const std::string& basePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(
        &attrib, &shapes, &materials,
        &warn, &err,
        objPath.c_str(),
        basePath.empty() ? nullptr : basePath.c_str(),
        true // triangulate
    );
    if (!warn.empty()) {
        std::cerr << "TinyOBJ WARNING: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "TinyOBJ ERROR: " << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Falha ao carregar OBJ: " << objPath << std::endl;
        return false;
    }

    vertices.clear();
    normals.clear();
    texcoords.clear();

    // Aqui assumimos que o peixe usa 1 material principal (por ex. fish.jpg).
    // Se seu .mtl tiver 1 material com 'map_Kd fish.jpg', a string materials[0].diffuse_texname deve ter "fish.jpg"
    // Caso o modelo tenha múltiplos materiais, seria preciso agrupar ou separar submeshes.

    // Carrega a(s) textura(s) do material[0] se existir
    if (!materials.empty()) {
        std::string texName = materials[0].diffuse_texname; // ex: "fish.jpg"
        if (!texName.empty()) {
            std::string texPath = basePath + texName;
            GLuint texID = SOIL_load_OGL_texture(
                texPath.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_INVERT_Y
            );
            if (texID) {
                glBindTexture(GL_TEXTURE_2D, texID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                textureID = texID;
                hasTexture = true;
                std::cout << "Carregou textura do peixe: " << texPath << std::endl;
            } else {
                std::cerr << "Erro ao carregar textura do peixe: " << texPath 
                          << " -> " << SOIL_last_result() << std::endl;
            }
        }
    }

    // Agrupa todos os vértices/normais/UV de todos os shapes num único array
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t indexOffset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f]; // deve ser 3
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                float vx = attrib.vertices[3*idx.vertex_index + 0];
                float vy = attrib.vertices[3*idx.vertex_index + 1];
                float vz = attrib.vertices[3*idx.vertex_index + 2];
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                if (idx.normal_index >= 0 && (size_t)(3*idx.normal_index+2) < attrib.normals.size()) {
                    float nx = attrib.normals[3*idx.normal_index + 0];
                    float ny = attrib.normals[3*idx.normal_index + 1];
                    float nz = attrib.normals[3*idx.normal_index + 2];
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                } else {
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                }

                if (idx.texcoord_index >= 0 && (size_t)(2*idx.texcoord_index+1) < attrib.texcoords.size()) {
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

void Fish3D::draw() {
    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        // normal
        if (i < normals.size()) {
            glNormal3f(normals[i], normals[i+1], normals[i+2]);
        }
        // texcoord
        if (hasTexture) {
            size_t texIndex = (i/3)*2;
            if (texIndex + 1 < texcoords.size()) {
                glTexCoord2f(texcoords[texIndex], texcoords[texIndex+1]);
            }
        }
        // vertex
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
    glEnd();

    if (hasTexture) {
        glDisable(GL_TEXTURE_2D);
    }
}
