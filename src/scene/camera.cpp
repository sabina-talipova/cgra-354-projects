
#include <iostream>

// glm
#include <glm/gtc/matrix_transform.hpp>

// project
#include "camera.hpp"
#include "opengl.hpp"


using namespace std;
using namespace glm;


void Camera::setPositionOrientation(const vec3 &pos, float yaw, float pitch) {
	m_position = pos;
	m_yaw = yaw;
	m_pitch = pitch;

	// update rotation matrix (based on yaw and pitch)
	m_rotation = rotate(mat4(1), m_yaw, vec3(0, 1, 0)) * rotate(mat4(1), m_pitch, vec3(1, 0, 0));
}


Ray Camera::generateRay(const vec2 &pixel) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Generate a ray in the scene using the camera position,
	// rotation field of view on the y axis (fovy) and the image
	// size. The pixel is given in image coordinates [0, imagesize]
	// This COULD be done by first creating the ray in ViewSpace
	// then transforming it by the position and rotation to get
	// it into worldspace.
	//-------------------------------------------------------------

	glm::vec2 ndc = (pixel + 0.5f) / m_image_size;
	float sx = 2.0f * ndc.x - 1.0f;
	float sy = 2.0f * ndc.y - 1.0f;

	float scale = tan(m_fovy * 0.5f);
	float aspect = m_image_size.x / m_image_size.y;

	glm::vec3 direction_cam = glm::normalize(glm::vec3(sx * aspect * scale, sy * scale, -1.0f));

	glm::vec3 world_dir = glm::normalize(glm::vec3(m_rotation * glm::vec4(direction_cam, 0.0f)));

	return Ray(m_position, world_dir);
}
