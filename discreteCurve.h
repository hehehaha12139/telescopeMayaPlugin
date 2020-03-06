#pragma once
#include "parameterizedCurve.h"
#include <vector>

class DiscreteCurve : ParameterizedCurve 
{
public:
	glm::vec3 startingPoint;
	DiscreteCurve* ParentCurve;

	// Get final point of curve
	glm::vec3 getLastPoint();
private:
	glm::vec3 startingTangent;
	glm::vec3 startingBinormal;
	glm::vec3 targetEndPoint;
	
	// Store curve points

	
};