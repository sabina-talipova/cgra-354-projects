
#pragma once

//std
#include <vector>

// glm
#include <glm/glm.hpp>

// project
#include "cgra/cgra_mesh.hpp"
#include "cgra/cgra_shader.hpp"


// boid class (forward declaration)
class Boid;

struct Obstacle {
	glm::vec3 position;
	float radius;
};


class Scene {
private:
	// opengl draw data
	GLuint m_color_shader = 0;
	GLuint m_aabb_shader = 0;
	GLuint m_skymap_shader = 0;
	cgra::gl_mesh m_simple_boid_mesh;
	cgra::gl_mesh m_boid_mesh;
	cgra::gl_mesh m_predator_mesh;
	cgra::gl_mesh m_sphere_mesh;

	// draw status
	bool m_show_aabb = true;
	bool m_show_axis = false;
	bool m_show_skymap = false;

	// scene data
	glm::vec3 m_bound_hsize = glm::vec3(20);
	std::vector<Boid> m_boids;


	//-------------------------------------------------------------
	// [Assignment 3] :
	// Create variables for keeping track of the boid parameters
	// such as min and max speed etc. These paramters can either be
	// public, or private with getter functions.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	float m_minSpeed = 2.0f;
	float m_maxSpeed = 10.0f;
	float m_localRadius = 5.0f;

	float m_avoidanceWeight = 1.5f;
	float m_alignmentWeight = 1.0f;
	float m_cohesionWeight = 1.0f;

	std::vector<Obstacle> m_obstacles;

public:

	Scene();

	// functions that load the scene
	void loadCore();
	void loadCompletion();
	void loadChallenge();

	// called every frame, with timestep in seconds
	void update(float timestep);

	// called every frame, with the given projection and view matrix
	void draw(const glm::mat4 &proj, const glm::mat4 &view);

	// called every frame (to fill out a ImGui::TreeNode)
	void renderGUI();

	// returns a const reference to the boids vector
	const std::vector<Boid> & boids() const { return m_boids; }

	// returns the half-size of the bounding box (centered around the origin)
	glm::vec3 bound() const { return m_bound_hsize; }

	// YOUR CODE GOES HERE
	// ...

	float minSpeed() const { return m_minSpeed; }
	float maxSpeed() const { return m_maxSpeed; }
	float localRadius() const { return m_localRadius; }

	float avoidanceWeight() const { return m_avoidanceWeight; }
	float alignmentWeight() const { return m_alignmentWeight; }
	float cohesionWeight() const { return m_cohesionWeight; }

	void setMinSpeed(float speed) { m_minSpeed = speed; }
	void setMaxSpeed(float speed) { m_maxSpeed = speed; }
	void setLocalRadius(float radius) { m_localRadius = radius; }

	void setAvoidanceWeight(float w) { m_avoidanceWeight = w; }
	void setAlignmentWeight(float w) { m_alignmentWeight = w; }
	void setCohesionWeight(float w) { m_cohesionWeight = w; }

	void addObstacle(const glm::vec3& pos, float radius) {
		m_obstacles.push_back({ pos, radius });
	}

	const std::vector<Obstacle>& obstacles() const { return m_obstacles; }
};