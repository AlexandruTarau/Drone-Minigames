#include "lab_m1/Tema2/Tree.h"
#include <iostream>

Tree::Tree(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	float ownAreaRadius,
	float trunkRadius,
	float trunkHeight,
	int leavesNumber,
	float leavesRadius,
	float leavesHeight
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->ownAreaRadius = ownAreaRadius;
	this->trunkHeight = trunkHeight;
	this->trunkRadius = trunkRadius;
	this->leavesNumber = leavesNumber;
	this->leavesHeight = leavesHeight;
	this->leavesRadius = leavesRadius;
}
Tree::Tree() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->ownAreaRadius = 1;
	this->trunkRadius = 0.2f;
	this->trunkHeight = 2.f;
	this->leavesHeight = 1.f;
	this->leavesNumber = 2;
	this->leavesRadius = 0.5f;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> Tree::DrawObstacle() {
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;
	glm::mat4 modelMatrix;

	float scaledTrunkRadiusX = trunkRadius * scale.x;
	float scaledTrunkRadiusZ = trunkRadius * scale.z;
	float scaledTrunkHeight = trunkHeight * scale.y;
	float scaledLeavesRadiusX = leavesRadius * scale.x;
	float scaledLeavesRadiusZ = leavesRadius * scale.z;
	float scaledLeavesHeight = leavesHeight * scale.y;

	// Tree trunk
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
	modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::RotateOY(rotation.y);
	modelMatrix *= transform3D::RotateOZ(rotation.z);
	modelMatrix *= transform3D::Scale(scaledTrunkRadiusX, scaledTrunkHeight, scaledTrunkRadiusZ);
	matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.26, 0.2, 0.086), "cylinder"));

	// Tree leaves
	for (int i = 0; i < leavesNumber; i++) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(position.x, position.y + scaledTrunkHeight + scaledLeavesHeight * i / 2.f, position.z);
		modelMatrix *= transform3D::RotateOX(rotation.x);
		modelMatrix *= transform3D::RotateOY(rotation.y);
		modelMatrix *= transform3D::RotateOZ(rotation.z);
		modelMatrix *= transform3D::Scale(scaledLeavesRadiusX, scaledLeavesHeight, scaledLeavesRadiusZ);
		matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.176, 0.471, 0.094), "cone"));
	}

	return matrices;
}

bool Tree::IsColliding(glm::vec3 playerPosition, float playerRadius) {
	// Check collision with the trunk (cylinder)
	float scaledTrunkRadius = trunkRadius * scale.x;
	float scaledTrunkHeight = trunkHeight * scale.y;

	// Calculate the distance from the player to the trunk's center in the XZ plane
	glm::vec2 playerPosXZ = glm::vec2(playerPosition.x, playerPosition.z);
	glm::vec2 trunkPosXZ = glm::vec2(position.x, position.z);
	float distanceXZ = glm::distance(playerPosXZ, trunkPosXZ);

	// Check if the player is within the trunk's radius and height
	if (distanceXZ <= (scaledTrunkRadius + playerRadius) &&
		playerPosition.y <= (position.y + scaledTrunkHeight) &&
		playerPosition.y >= position.y) {
		return true;
	}

	// Check collision with the leaves (cones)
	float scaledLeavesRadius = leavesRadius * scale.x;
	float scaledLeavesHeight = leavesHeight * scale.y;

	for (int i = 0; i < leavesNumber; i++) {
		// Calculate the position of the current cone's base
		glm::vec3 coneBasePos = position;
		coneBasePos.y += scaledTrunkHeight + scaledLeavesHeight / 2.f * i;

		// Calculate the distance from the player to the cone's base in the XZ plane
		glm::vec2 coneBasePosXZ = glm::vec2(coneBasePos.x, coneBasePos.z);
		float distanceXZ = glm::distance(playerPosXZ, coneBasePosXZ);

		// Check if the player is within the vertical bounds of the cone
		if (playerPosition.y >= coneBasePos.y && playerPosition.y <= (coneBasePos.y + scaledLeavesHeight)) {
			// Calculate height difference (player's height above the base)
			float heightDifference = playerPosition.y - coneBasePos.y;

			// Compute the expected radius at the player's height inside the cone
			float currentRadiusAtHeight = (scaledLeavesRadius * (scaledLeavesHeight - heightDifference)) / scaledLeavesHeight;

			// Check if the player is within the sloped cone's radius at their Y position
			if (distanceXZ <= (currentRadiusAtHeight + playerRadius)) {
				return true; // Collision detected
			}
		}
	}


	// No collision detected
	return false;
}