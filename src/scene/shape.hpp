
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "ray.hpp"
#include "scene.hpp"


class Shape {
public:
	virtual RayIntersection intersect(const Ray &ray) = 0;
};


class AABB : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 m_halfsize;

public:
	AABB(const glm::vec3 &c, float hs) : m_center(c), m_halfsize(hs) { }
	AABB(const glm::vec3 &c, const glm::vec3 &hs) : m_center(c), m_halfsize(hs) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};


class Sphere : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Sphere(const glm::vec3 &c, float radius) : m_center(c), m_radius(radius) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};

//-------------------------------------------------------------
// [Assignment 4] :
// Implement the following additional Shapes :
// - Plane
// - Disk
// - Triangle
// Follow the pattern shown by AABB and Sphere for implementing
// a class that subclasses Shape making sure that you implement
// the intersect method for each new Shape.
//-------------------------------------------------------------

class Plane : public Shape {
private:
	glm::vec3 m_point;
	glm::vec3 m_normal;

public:
	Plane(const glm::vec3& point, const glm::vec3& normal)
		: m_point(point), m_normal(glm::normalize(normal)) {}

	virtual RayIntersection intersect(const Ray& ray) override {
		RayIntersection intersect;
		float denom = glm::dot(m_normal, ray.direction);
		if (glm::abs(denom) < 1e-6f) return intersect;

		float t = glm::dot(m_point - ray.origin, m_normal) / denom;
		if (t < 0) return intersect;

		intersect.m_valid = true;
		intersect.m_distance = t;
		intersect.m_position = ray.origin + t * ray.direction;
		intersect.m_normal = m_normal;
		intersect.m_shape = this;

		return intersect;
	}
};

class Disk : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 m_normal;
	float m_radius;

public:
	Disk(const glm::vec3& center, const glm::vec3& normal, float radius)
		: m_center(center), m_normal(glm::normalize(normal)), m_radius(radius) {}

	virtual RayIntersection intersect(const Ray& ray) override {
		RayIntersection intersect;
		float denom = glm::dot(m_normal, ray.direction);
		if (glm::abs(denom) < 1e-6f) return intersect;

		float t = glm::dot(m_center - ray.origin, m_normal) / denom;
		if (t < 0) return intersect;

		glm::vec3 hitPoint = ray.origin + t * ray.direction;
		if (glm::length(hitPoint - m_center) > m_radius) return intersect;

		intersect.m_valid = true;
		intersect.m_distance = t;
		intersect.m_position = hitPoint;
		intersect.m_normal = m_normal;
		intersect.m_shape = this;

		return intersect;
	}
};

class Triangle : public Shape {
private:
	glm::vec3 m_v0, m_v1, m_v2;

public:
	Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
		: m_v0(v0), m_v1(v1), m_v2(v2) {}

	virtual RayIntersection intersect(const Ray& ray) override {
		RayIntersection intersect;

		const float EPSILON = 1e-6f;
		glm::vec3 edge1 = m_v1 - m_v0;
		glm::vec3 edge2 = m_v2 - m_v0;
		glm::vec3 h = glm::cross(ray.direction, edge2);
		float a = glm::dot(edge1, h);
		if (fabs(a) < EPSILON) return intersect;

		float f = 1.0f / a;
		glm::vec3 s = ray.origin - m_v0;
		float u = f * glm::dot(s, h);
		if (u < 0.0f || u > 1.0f) return intersect;

		glm::vec3 q = glm::cross(s, edge1);
		float v = f * glm::dot(ray.direction, q);
		if (v < 0.0f || u + v > 1.0f) return intersect;

		float t = f * glm::dot(edge2, q);
		if (t < EPSILON) return intersect;

		intersect.m_valid = true;
		intersect.m_distance = t;
		intersect.m_position = ray.origin + t * ray.direction;
		intersect.m_normal = glm::normalize(glm::cross(edge1, edge2));
		intersect.m_shape = this;

		return intersect;
	}
};



