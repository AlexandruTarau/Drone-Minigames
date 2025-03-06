#include "lab_m1/Tema2/PowerUp.h"
#include <iostream>

PowerUp::PowerUp(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	float ownAreaRadius,
	float radius,
	PowerUpType type
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->ownAreaRadius = ownAreaRadius;
	this->radius = radius;
	this->type = type;
}
PowerUp::PowerUp() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->ownAreaRadius = 1;
	this->radius = 1;
	this->type = SPEED;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> PowerUp::DrawPowerUp() {
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;
	glm::mat4 modelMatrix = glm::mat4(1);

	// Draw the power-up
	modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
	modelMatrix *= transform3D::Scale(radius * scale.x, radius * scale.y, radius * scale.z);
	modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::RotateOY(rotation.y);
	modelMatrix *= transform3D::RotateOZ(rotation.z);

	switch (type) {
	case SPEED:
		// Draw a sphere with a blue color
		matrices.push_back(std::tuple(modelMatrix, glm::vec3(0, 0, 1), "sphere"));
		break;
	case STUN:
		// Draw a sphere with a gray color
		matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.3, 0.3, 0.3), "sphere"));
		break;
	case BLOCK:
		// Draw a sphere with a red color
		matrices.push_back(std::tuple(modelMatrix, glm::vec3(1, 0, 0), "sphere"));
		break;
	}

	return matrices;
}

bool PowerUp::IsColliding(glm::vec3 playerPosition, float playerRadius) {
	// Check if the player is colliding with the power-up
	if (glm::distance(playerPosition, position) < playerRadius + radius) {
		return true;
	}

	// No collision detected
	return false;
}

void PowerUp::ApplyPowerUp(Drone& srcDrone, Drone& dstDrone, Checkpoint** dstCheckpoint) {
	this->srcDrone = &srcDrone;
	this->dstDrone = &dstDrone;
	this->dstCheckpoint = dstCheckpoint;

	// Apply the power-up effect
	switch (type) {
	case SPEED:
		// Increase the speed of the drone
		srcDrone.ApplySpeedBoost(speedBoost, pitchBoost, yawBoost, rollBoost);
		break;
	case STUN:
		// Stun the drone
		dstDrone.SetStunned(true);
		break;
	case BLOCK:
		// Block the checkpoint
		(*dstCheckpoint)->SetBlocked(true);
		break;
	}
}

bool PowerUp::UpdatePowerUp(float deltaTime) {
	if (isActive) {
		switch (type) {
		case SPEED:
			// Update the speed boost timer
			speedBoostTimer += deltaTime;
			if (speedBoostTimer >= speedBoostDuration) {
				srcDrone->ApplySpeedBoost(-speedBoost, -pitchBoost, -yawBoost, -rollBoost);
				isActive = false;
				speedBoostTimer = 0;
				return true;
			}
			break;
		case STUN:
			// Update the stun timer
			stunTimer += deltaTime;
			if (stunTimer >= stunDuration) {
				dstDrone->SetStunned(false);
				isActive = false;
				stunTimer = 0;
				return true;
			}
			break;
		case BLOCK:
			// Update the block timer
			blockTimer += deltaTime;
			if (blockTimer >= blockDuration) {
				(*dstCheckpoint)->SetBlocked(false);
				isActive = false;
				blockTimer = 0;
				return true;
			}
			break;
		}
	}
	return false;
}

bool PowerUp::IsActive() {
	return isActive;
}

void PowerUp::SetActive(bool isActive) {
	this->isActive = isActive;
}