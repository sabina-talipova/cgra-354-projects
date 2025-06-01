
#define GLM_ENABLE_EXPERIMENTAL
// glm
#include <glm/gtc/constants.hpp>

// project
#include "light.hpp"
#include <glm/gtx/norm.hpp>

using namespace glm;


bool DirectionalLight::occluded(Scene *scene, const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Determine whether the given point is being occluded from
	// this directional light by an object in the scene.
	// Remember that directional lights are "infinitely" far away
	// so any object in the way would cause an occlusion.
	//-------------------------------------------------------------

	Ray shadowRay(point + 1e-4f * -m_direction, -m_direction);
	RayIntersection hit = scene->intersect(shadowRay);

	return hit.m_valid;
}


vec3 DirectionalLight::incidentDirection(const vec3 &) const {
	return m_direction;
}


vec3 DirectionalLight::irradiance(const vec3 &) const {
	return m_irradiance;
}


bool PointLight::occluded(Scene *scene, const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Determine whether the given point is being occluded from
	// this directional light by an object in the scene.
	// Remember that point lights are somewhere in the scene and
	// an occulsion has to occur somewhere between the light and 
	// the given point.
	//-------------------------------------------------------------
	glm::vec3 dirToLight = m_position - point;
	float maxDist = glm::length(dirToLight);
	glm::vec3 dir = glm::normalize(dirToLight);

	Ray shadowRay(point + 1e-4f * dir, dir);
	RayIntersection hit = scene->intersect(shadowRay);

	return hit.m_valid && hit.m_distance < maxDist;
}


vec3 PointLight::incidentDirection(const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the direction of the incoming light (light to point)
	//-------------------------------------------------------------

	return glm::normalize(m_position - point);
}


vec3 PointLight::irradiance(const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the total irradiance on the given point.
	// This requires you to convert the flux of the light into
	// irradiance by dividing it by the surface of the sphere
	// it illuminates. Remember that the surface area increases
	// as the sphere gets bigger, ie. the point is further away.
	//-------------------------------------------------------------
	float distance2 = glm::distance2(m_position, point);
	if (distance2 == 0) return glm::vec3(0);

	return m_flux / (4.0f * glm::pi<float>() * distance2);
}
