#include "lab_m1/Tema2/House.h"
#include <iostream>

House::House(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	float ownAreaRadius,
	float height,
	float width,
	float depth,
	float roofHeight
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->ownAreaRadius = ownAreaRadius;
	this->depth = depth;
	this->height = height;
	this->width = width;
	this->roofHeight = roofHeight;
}
House::House() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->ownAreaRadius = 1;
	this->depth = 1;
	this->height = 1;
	this->width = 1;
	this->roofHeight = 0.5;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> House::DrawObstacle() {
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;
	glm::mat4 modelMatrix;

	float scaledHeight = height * scale.y;
	float scaledWidth = width * scale.x;
	float scaledDepth = depth * scale.z;
	float scaledRoofHeight = roofHeight * scale.y;

	// House base
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(position.x, position.y + scaledHeight / 2.f, position.z);
	modelMatrix *= transform3D::Scale(scaledWidth, scaledHeight, scaledDepth);
	modelMatrix *= transform3D::RotateOZ(rotation.z);
	modelMatrix *= transform3D::RotateOY(rotation.y);
	modelMatrix *= transform3D::RotateOX(rotation.x);
	matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.3, 0.3, 0.1), "box"));

	// House roof
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(position.x, position.y + scaledHeight + scaledRoofHeight / 2.f, position.z);
	modelMatrix *= transform3D::Scale(scaledWidth, scaledRoofHeight, scaledDepth);
	modelMatrix *= transform3D::RotateOZ(rotation.z);
	modelMatrix *= transform3D::RotateOY(rotation.y);
	modelMatrix *= transform3D::RotateOX(rotation.x);
	matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.2, 0.2, 0.8), "prism"));

	return matrices;
}

bool House::IsColliding(glm::vec3 playerPosition, float playerRadius) {
	float scaledWidth = width * scale.x;
	float scaledHeight = height * scale.y;
	float scaledDepth = depth * scale.z;
	float scaledRoofHeight = roofHeight * scale.y;

	// AABB for the rectangular base, expanded by playerRadius
	float baseMinX = position.x - scaledWidth / 2.f - playerRadius;
	float baseMaxX = position.x + scaledWidth / 2.f + playerRadius;
	float baseMinY = position.y - playerRadius;
	float baseMaxY = position.y + scaledHeight;
	float baseMinZ = position.z - scaledDepth / 2.f - playerRadius;
	float baseMaxZ = position.z + scaledDepth / 2.f + playerRadius;

	// Check collision with the base
	if (playerPosition.x >= baseMinX && playerPosition.x <= baseMaxX &&
		playerPosition.y >= baseMinY && playerPosition.y <= baseMaxY &&
		playerPosition.z >= baseMinZ && playerPosition.z <= baseMaxZ) {
		return true;
	}

	// Prism Base Position (roof start position)
	glm::vec3 prismBasePos = position + glm::vec3(0, scaledHeight, 0);

	// AABB for the prism's bounding box expanded by playerRadius
	float roofMinX = prismBasePos.x - scaledWidth / 2.f - playerRadius;
	float roofMaxX = prismBasePos.x + scaledWidth / 2.f + playerRadius;
	float roofMinZ = prismBasePos.z - scaledDepth / 2.f - playerRadius;
	float roofMaxZ = prismBasePos.z + scaledDepth / 2.f + playerRadius;
	float roofBaseY = prismBasePos.y;
	float roofApexY = roofBaseY + scaledRoofHeight + playerRadius;

	// Skip slope check if outside AABB for prism
	if (playerPosition.x < roofMinX || playerPosition.x > roofMaxX ||
		playerPosition.z < roofMinZ || playerPosition.z > roofMaxZ ||
		playerPosition.y < roofBaseY || playerPosition.y > roofApexY) {
		return false;
	}

	// Slope check: Handle slope with radius awareness
	float halfWidth = scaledWidth / 2.f;
	float distX = glm::abs(playerPosition.x - prismBasePos.x);  // X-axis distance from prism center
	float maxRoofY = roofBaseY + (1.0f - (distX / halfWidth)) * scaledRoofHeight;

	// Adjust maxRoofY for player radius in the slope's surface range
	maxRoofY += playerRadius * (1.0f - distX / halfWidth);

	// Check if the player collider is fully under the sloped roof surface
	if (playerPosition.y <= maxRoofY) {
		return true;
	}

	return false;
}
