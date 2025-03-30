
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"


// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and/or modified for adding in extra information for drawing
// including colors for diffuse/specular, and textures for texture mapping etc.
struct basic_model {
	GLuint shader = 0;
	cgra::gl_mesh mesh;
	glm::vec3 color{1, 0, 0};

	// Assignment_2

	glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 ambient_color{ 1, 0, 0 };
	glm::vec3 specular_color{ 1, 0, 0 };
	float specular_strength = 0.5;
	float shininess = 0.5;

	glm::mat4 modelTransform{1.0};

	void draw(const glm::mat4 &view, const glm::mat4 proj) {
		using namespace glm;

		// cacluate the modelview transform
		mat4 modelview = view * modelTransform;

		// load shader and variables
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

		glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(light_pos));
		glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(view_pos));
		glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(light_color));

		glUniform3fv(glGetUniformLocation(shader, "uAmbiantColor"), 1, value_ptr(ambient_color));
		glUniform3fv(glGetUniformLocation(shader, "uSpecularColor"), 1, value_ptr(specular_color));
		glUniform1f(glGetUniformLocation(shader, "uSpecularStrength"), specular_strength);
		glUniform1f(glGetUniformLocation(shader, "uShininess"), shininess);

		// draw the mesh
		mesh.draw(); 
	}
};