
// std
#include <stdexcept>

// project
#include "cgra_mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace cgra {

	void gl_mesh::draw() {
		if (vao == 0) return;
		// bind our VAO which sets up all our buffers and data for us
		glBindVertexArray(vao);
		// tell opengl to draw our VAO using the draw mode and how many verticies to render
		//glDrawElements(mode, index_count, GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(
			mode,               // e.g. GL_TRIANGLES
			index_count,        // number of indices per instance
			GL_UNSIGNED_INT,    // assuming your indices are unsigned int
			0,                  // pointer to the starting offset (usually 0)
			100                 // number of instances
		);
	}

	void gl_mesh::destroy() {
		// delete the data buffers
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

	gl_mesh mesh_builder::multibuild() const {
		glm::mat4 modelMatrices[100];
		srand(static_cast<unsigned int>(glfwGetTime()));

		for (int i = 0; i < 100; i++) {
			float angle = glm::radians(static_cast<float>(rand() % 360));
			float scale = 0.5f + static_cast<float>(rand() % 100) / 200.0f;

			float x = (rand() % 81) - 15.0f;
			float y = (rand() % 51) - 5.0f;
			float z = (rand() % 81) - 15.0f;

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x, y, z));
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(scale));

			modelMatrices[i] = model;
		}

		glm::vec3 instanceColors[100];
		for (int i = 0; i < 100; i++) {
			instanceColors[i] = glm::vec3(
				static_cast<float>(rand() % 100) / 100.0f,
				static_cast<float>(rand() % 100) / 100.0f,
				static_cast<float>(rand() % 100) / 100.0f
			);
		}

		gl_mesh m;
		glGenVertexArrays(1, &m.vao);
		glGenBuffers(1, &m.vbo);
		glGenBuffers(1, &m.ibo);

		GLuint instanceVBO;
		glGenBuffers(1, &instanceVBO);

		GLuint colorVBO;
		glGenBuffers(1, &colorVBO);


		glBindVertexArray(m.vao);

		glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mesh_vertex), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, norm));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)offsetof(mesh_vertex, uv));

		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(modelMatrices), modelMatrices, GL_STATIC_DRAW);

		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(3 + i);
			glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
			glVertexAttribDivisor(3 + i, 1);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(instanceColors), instanceColors, GL_STATIC_DRAW);

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(7, 1);

		m.index_count = indices.size();
		m.mode = mode;

		glBindVertexArray(0);

		return m;

	}


	gl_mesh mesh_builder::build() const {

		gl_mesh m;
		glGenVertexArrays(1, &m.vao); // VAO stores information about how the buffers are set up
		glGenBuffers(1, &m.vbo); // VBO stores the vertex data
		glGenBuffers(1, &m.ibo); // IBO stores the indices that make up primitives


		// VAO
		//
		glBindVertexArray(m.vao);

		
		// VBO (single buffer, interleaved)
		//
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
		// upload ALL the vertex data in one buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mesh_vertex), &vertices[0], GL_STATIC_DRAW);

		// this buffer will use location=0 when we use our VAO
		glEnableVertexAttribArray(0);
		// tell opengl how to treat data in location=0 - the data is treated in lots of 3 (3 floats = vec3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void *)(offsetof(mesh_vertex, pos)));

		// do the same thing for Normals but bind it to location=1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void *)(offsetof(mesh_vertex, norm)));

		// do the same thing for UVs but bind it to location=2 - the data is treated in lots of 2 (2 floats = vec2)
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void *)(offsetof(mesh_vertex, uv)));


		// IBO
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo);
		// upload the indices for drawing primitives
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


		// set the index count and draw modes
		m.index_count = indices.size();
		m.mode = mode;

		// clean up by binding VAO 0 (good practice)
		glBindVertexArray(0);

		return m;
	}
}