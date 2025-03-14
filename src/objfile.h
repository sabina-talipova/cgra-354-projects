#pragma once

// std
#include <vector>

// glm
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

struct mesh_vertex {
	glm::vec3 position{ 0 };
	glm::vec3 normal{ 0 };
	glm::vec2 uv{ 0 };
};

class ObjFile
{
private:
	std::vector<mesh_vertex> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

public:
	GLuint shader = 0;
	glm::vec3 color{ 0.7 };

	ObjFile();
	~ObjFile();
	void loadOBJ(const std::string& filename);
	void build();
	void draw();
	void destroy();
	void printMeshData();
};

