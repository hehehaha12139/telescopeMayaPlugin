#pragma once
#include <glm/glm.hpp>

using namespace std;

class ParameterizedCurve
{
public:
	// Curve Position Data
	glm::vec3 startPosition;
	glm::vec3 endPosition;

	// Curve Tangent Data
	glm::vec3 startTangent;
	glm::vec3 endTangent;

	// Transformation
	virtual void RotateAndOffset(glm::quat rotation, glm::vec3 center, glm::vec3 tangent, float radius) = 0;
};
