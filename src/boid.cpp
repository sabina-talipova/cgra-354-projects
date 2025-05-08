
// glm
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

// project
#include "boid.hpp"
#include "scene.hpp"
#include "cgra/cgra_mesh.hpp"


using namespace glm;
using namespace std;


vec3 Boid::color() const {
	//return vec3(0, 1, 0);

	glm::vec3 dir = glm::normalize(m_velocity);
	return glm::vec3(
		0.5f + 0.5f * dir.x,
		0.5f + 0.5f * dir.y,
		0.5f + 0.5f * dir.z
	);
}


void Boid::calculateForces(Scene *scene) {
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Calculate the forces affecting the boid and update the
	// acceleration (assuming mass = 1).
	// Do NOT update velocity or position in this function.
	// Core : 
	//  - Cohesion
	//  - Alignment
	//  - Avoidance
	//  - Soft Bound (optional)
	// Completion : 
	//  - Cohesion and Alignment with only boids in the same flock
	//  - Predator Avoidance (boids only)
	//  - Predator Chase (predator only)
	// Challenge : 
	//  - Obstacle avoidance
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	const auto& boids = scene->boids();

	float localRadius = scene->localRadius();
	float avoidanceWeight = scene->avoidanceWeight();
	float alignmentWeight = scene->alignmentWeight();
	float cohesionWeight = scene->cohesionWeight();

	glm::vec3 obstacleAvoid = obstacleAvoidanceForce(scene);

	glm::vec3 alignment(0.0f), cohesion(0.0f), avoidance(0.0f);
	glm::vec3 predatorAvoidance(0.0f);
	glm::vec3 chaseForce(0.0f);

	int neighborCount = 0;

	float predatorRadius = localRadius * 1.5f;
	float predatorAvoidWeight = 3.0f;
	float chaseWeight = 2.0f;

	float minDistToPrey = 99999.0f;
	glm::vec3 closestPrey;

	for (const Boid& other : boids) {
		if (&other == this) continue;

		float dist2 = glm::distance2(m_position, other.position());

		if (!m_isPredator) {
			if (!other.isPredator() && other.flockId() == m_flockId && dist2 < localRadius * localRadius) {
				alignment += other.velocity();
				cohesion += other.position();

				glm::vec3 offset = m_position - other.position();
				if (glm::length2(offset) > 0.0001f)
					avoidance += glm::normalize(offset) / dist2;

				++neighborCount;
			}

			if (other.isPredator() && dist2 < predatorRadius * predatorRadius) {
				glm::vec3 away = m_position - other.position();
				if (glm::length2(away) > 0.0001f)
					predatorAvoidance += glm::normalize(away) / dist2;
			}
		}

		else if (m_isPredator && !other.isPredator()) {
			if (dist2 < minDistToPrey) {
				minDistToPrey = dist2;
				closestPrey = other.position();
			}
		}
	}

	if (neighborCount > 0) {
		alignment = glm::normalize(alignment / float(neighborCount)) - m_velocity;
		cohesion = ((cohesion / float(neighborCount)) - m_position);
	}

	if (m_isPredator && minDistToPrey < 99999.0f) {
		chaseForce = glm::normalize(closestPrey - m_position);
	}

	glm::vec3 boundForce(0.0f);
	glm::vec3 bounds = scene->bound();
	for (int i = 0; i < 3; ++i) {
		if (m_position[i] < -bounds[i]) boundForce[i] = 1.0f;
		if (m_position[i] > bounds[i]) boundForce[i] = -1.0f;
	}

	m_acceleration =
		(avoidance * avoidanceWeight) +
		(alignment * alignmentWeight) +
		(cohesion * cohesionWeight) +
		(predatorAvoidance * predatorAvoidWeight) +
		(chaseForce * chaseWeight) +
		(boundForce * 5.0f) +
		+ obstacleAvoid;

}


void Boid::update(float timestep, Scene *scene) {
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Integrate the velocity of the boid using the timestep.
	// Update the position of the boid using the new velocity.
	// Take into account the bounds of the scene which may
	// require you to change the velocity (if bouncing) or
	// change the position (if wrapping).
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...


	//m_position += m_velocity * timestep;

	calculateForces(scene);

	m_velocity += m_acceleration * timestep;

	float speed = glm::length(m_velocity);
	float minSpeed = scene->minSpeed();
	float maxSpeed = scene->maxSpeed();

	if (speed > maxSpeed) {
		m_velocity = glm::normalize(m_velocity) * maxSpeed;
	}
	else if (speed < minSpeed && speed > 0.001f) {
		m_velocity = glm::normalize(m_velocity) * minSpeed;
	}

	m_position += m_velocity * timestep;

}

glm::vec3 Boid::obstacleAvoidanceForce(const Scene* scene) {
	glm::vec3 force(0.0f);
	for (const auto& obs : scene->obstacles()) {
		glm::vec3 toBoid = m_position - obs.position;
		float dist = glm::length(toBoid);
		float safeDist = obs.radius + scene->localRadius();

		if (dist < safeDist && dist > 0.01f) {
			glm::vec3 away = glm::normalize(toBoid);
			force += away * (1.0f / dist);
		}
	}
	return force * 5.0f;
}