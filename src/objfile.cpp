#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "objfile.h"

using namespace std;
using namespace glm;

ObjFile::ObjFile(){}

ObjFile::~ObjFile(){}


void ObjFile::loadOBJ(const std::string& filename)
{
    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempUVs;

    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vt") {
            glm::vec2 tex;
            iss >> tex.x >> tex.y;
            tempUVs.push_back(tex);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "f") {
            int vIdx[3], tIdx[3], nIdx[3];
            char slash; // to read the `/` character

            for (int i = 0; i < 3; i++) {
                iss >> vIdx[i] >> slash >> tIdx[i] >> slash >> nIdx[i];
                mesh_vertex vertex;

                vertex.position = tempPositions[vIdx[i] - 1];
                vertex.normal = tempNormals[nIdx[i] - 1];
                vertex.uv = tempUVs[tIdx[i] - 1];

                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
        }
    }
    std::cout << "File id loaded" << std::endl;

    file.close();
}

void ObjFile::build()
{
    std::cout << "Build is running..." << std::endl;

    std::cout << "Vertices size: " << vertices.size() << std::endl;
    std::cout << "Indices size: " << indices.size() << std::endl;

    if (vertices.empty() || indices.empty()) {
        std::cerr << "Error: No vertex or index data to build!" << std::endl;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mesh_vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ObjFile::draw()
{
    if (VAO == 0) return;

    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ObjFile::destroy()
{
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }

    std::cout << "Destroy is finished" << std::endl;
}

void ObjFile::printMeshData()
{
    std::cout << "pos" << std::endl;
    for (mesh_vertex v : vertices) {
        std::cout << v.position.x << ", " << v.position.y << ", " << v.position.z << ", ";
        std::cout << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << ", ";
        std::cout << v.uv.x << ", " << v.uv.y << ", " << std::endl;
    }
    std::cout << "idx" << std::endl;
    for (int i : indices) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}
