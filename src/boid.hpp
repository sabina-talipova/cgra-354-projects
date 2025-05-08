
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "scene.hpp"


class Boid {
private:

	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;

	// Assignment 3
	int m_flockId = 0;
	bool m_isPredator = false;

	glm::vec3 obstacleAvoidanceForce(const Scene* scene);

public:
	Boid(glm::vec3 pos, glm::vec3 dir, int flock = 0, bool predator = false)
		: m_position(pos), m_velocity(dir), m_flockId(flock), m_isPredator(predator) { }

	glm::vec3 position() const { return m_position; }
	glm::vec3 velocity() const { return m_velocity; }
	glm::vec3 acceleration() const { return m_acceleration; }

	glm::vec3 color() const;

	void calculateForces(Scene *scene);
	void update(float timestep, Scene *scene);


	int flockId() const { return m_flockId; }
	bool isPredator() const { return m_isPredator; }
};