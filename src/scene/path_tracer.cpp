
// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

// std
#include <random>

// project
#include "scene.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "material.hpp"
#include "path_tracer.hpp"


using namespace std;
using namespace glm;


vec3 SimplePathTracer::sampleRay(const Ray &ray, int) {
	// intersect ray with the scene
	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {

		// simple grey shape shading
		float f = abs(dot(-ray.direction, intersect.m_normal));
		vec3 grey(0.5, 0.5, 0.5);
		return mix(grey / 2.0f, grey, f);
	}

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



vec3 CorePathTracer::sampleRay(const Ray &ray, int) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the ambient, diffuse
	// and specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays). You do
	// not need to use the depth argument for this implementation.
	//-------------------------------------------------------------
	RayIntersection hit = m_scene->intersect(ray);
	if (!hit.m_valid) return vec3(0.3f, 0.3f, 0.4f);

	vec3 color(0.0f);
	vec3 n = normalize(hit.m_normal);
	vec3 v = normalize(-ray.direction);
	Material* mat = hit.m_material;

	vec3 ambient = vec3(0.1f) * mat->diffuse();
	color += ambient;

	for (auto& light : m_scene->lights()) {
		vec3 l = normalize(light->incidentDirection(hit.m_position));
		vec3 h = normalize(v + l);

		if (light->occluded(m_scene, hit.m_position + n * 1e-4f)) continue;

		float diff = std::max(dot(n, l), 0.0f);
		float spec = pow(std::max(dot(n, h), 0.0f), mat->shininess());

		vec3 diffuse = mat->diffuse() * diff;
		vec3 specular = mat->specular() * spec;

		color += (diffuse + specular) * light->irradiance(hit.m_position);
	}
	return color;
}



vec3 CompletionPathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Using the same requirements for the CorePathTracer add in 
	// a recursive element to calculate perfect specular reflection.
	// That is compute the reflection ray off your intersection and
	// sample a ray in that direction, using the result to additionally
	// light your object. To make this more realistic you may weight
	// the incoming light by the (1 - (1/shininess)).
	//-------------------------------------------------------------

	if (depth > 3) return vec3(0.0f);

	RayIntersection hit = m_scene->intersect(ray);
	if (!hit.m_valid) return vec3(0.3f, 0.3f, 0.4f);

	vec3 color = vec3(0.05f) * hit.m_material->diffuse();

	for (auto& light : m_scene->lights()) {
		if (light->occluded(m_scene, hit.m_position))
			continue;

		vec3 l = normalize(light->incidentDirection(hit.m_position));
		vec3 n = normalize(hit.m_normal);
		vec3 v = normalize(-ray.direction);
		vec3 h = normalize(v + l);

		float NdotL = std::max(dot(n, l), 0.0f);
		float NdotH = std::max(dot(n, h), 0.0f);

		vec3 diffuse = hit.m_material->diffuse() * NdotL;
		vec3 specular = hit.m_material->specular() * pow(NdotH, hit.m_material->shininess());

		color += light->irradiance(hit.m_position) * (diffuse + specular);
	}

	vec3 r = reflect(ray.direction, normalize(hit.m_normal));
	Ray reflectRay(hit.m_position + hit.m_normal * 1e-4f, r);

	float weight = 1.0f - 1.0f / (hit.m_material->shininess() + 1.0f);
	color += weight * sampleRay(reflectRay, depth + 1) * hit.m_material->specular();

	return color;

	// no intersection - return background color
	//return { 0.3f, 0.3f, 0.4f };
}



vec3 ChallengePathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the diffuse and 
	// specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays).
	// Implement support for textured materials (using a texture
	// for the diffuse portion of the material).
	//
	// EXTRA FOR EXPERTS :
	// Additionally implement indirect diffuse and specular instead
	// of using the ambient lighting term.
	// The diffuse is sampled from the surface hemisphere and the
	// specular is sampled from a cone of the phong lobe (which
	// gives a glossy look). For best results you need to normalize
	// the lighting (see http://www.thetenthplanet.de/archives/255)
	//-------------------------------------------------------------

	if (depth > 3) return vec3(0.0f);

	RayIntersection hit = m_scene->intersect(ray);
	if (!hit.m_valid) return vec3(0.3f, 0.3f, 0.4f);

	vec3 color(0.0f);
	vec3 n = normalize(hit.m_normal);
	vec3 v = normalize(-ray.direction);
	Material* mat = hit.m_material;

	vec3 kd = mat->diffuse();

	if (auto* textured = dynamic_cast<TexturedMaterial*>(mat)) {
		if (textured->hasTexture()) {
			kd *= textured->textureColor(hit.m_uv_coord);
		}
	}

	for (auto& light : m_scene->lights()) {
		vec3 l = normalize(light->incidentDirection(hit.m_position));
		vec3 h = normalize(v + l);

		if (light->occluded(m_scene, hit.m_position + n * 1e-4f)) continue;

		float NdotL = std::max(dot(n, l), 0.0f);
		float NdotH = std::max(dot(n, h), 0.0f);
		float shininess = mat->shininess();

		vec3 diffuse = kd * NdotL;
		vec3 specular = mat->specular() * pow(NdotH, shininess);

		color += (diffuse + specular) * light->irradiance(hit.m_position);
	}

	//EXTRA

	/*
	if (depth < MAX_DEPTH_INDIRECT) {
		vec3 tangent, bitangent;
		createOrthonormalBasis(n, tangent, bitangent);

		vec3 randDir = cosineWeightedHemisphereSample();
		vec3 worldDir = randDir.x * tangent + randDir.y * bitangent + randDir.z * n;

		Ray indirectRay(hit.m_position + n * 1e-4f, worldDir);
		vec3 indirectColor = sampleRay(indirectRay, depth + 1);
		color += kd * indirectColor;
	}
	*/

	return color;

	// no intersection - return background color
	//return { 0.3f, 0.3f, 0.4f };
}