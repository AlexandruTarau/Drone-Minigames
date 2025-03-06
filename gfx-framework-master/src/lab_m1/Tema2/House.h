#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include "lab_m1/Tema2/Obstacle.h"
#include <vector>
#include <tuple>

class House: public Obstacle
{
public:

	House(
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		float ownAreaRadius,
		float height,
		float width,
		float depth,
		float roofHeight
		);
	House();

	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawObstacle();
	bool IsColliding(glm::vec3 playerPosition, float playerRadius);

protected:
	float height;
	float width;
	float depth;
	float roofHeight;
};