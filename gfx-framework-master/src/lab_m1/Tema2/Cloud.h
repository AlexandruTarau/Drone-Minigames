#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include <vector>
#include <tuple>

class Cloud
{
public:

	Cloud(
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		glm::vec3 velocity,
		float partsNumber,
		float speed
		);
	Cloud();

	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawCloud();
	void UpdateCloud(float deltaTime, glm::vec3 worldBounds);
	static void GenerateClouds(std::vector<Cloud>& clouds, int cloudCount, glm::vec3 mapSize);
	void SetPosition(const glm::vec3 newPosition);
	glm::vec3 GetPosition() const;

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 velocity;
	float partsNumber;
	float speed;
};