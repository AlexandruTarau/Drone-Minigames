#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include <vector>
#include <tuple>

class Checkpoint
{
public:
	constexpr static float margin = 0.1f;

	Checkpoint(
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		float width,
		float height,
		float depth,
		float ownAreaRadius
		);
	Checkpoint();

	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawCheckpoint();
	bool Passed(glm::vec3 playerPosition, glm::vec3 prevPlayerPosition);

	// Getters & Setters
	glm::vec3 GetPosition();
	float GetOwnAreaRadius();
	bool IsBlocked();
	void SetBlocked(bool isBlocked);

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float width;
	float height;
	float depth;
	float ownAreaRadius;
	bool isBlocked;
};