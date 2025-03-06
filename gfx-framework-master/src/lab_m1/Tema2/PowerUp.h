#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include <vector>
#include <tuple>
#include "lab_m1/Tema2/Drone.h"
#include "lab_m1/Tema2/Checkpoint.h"

enum PowerUpType {
	SPEED,
	STUN,
	BLOCK,
};

class PowerUp
{
public:

	PowerUp(
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		float ownAreaRadius,
		float radius,
		PowerUpType type
		);
	PowerUp();

	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawPowerUp();
	bool IsColliding(glm::vec3 playerPosition, float playerRadius);
	void ApplyPowerUp(Drone& srcDrone, Drone& dstDrone, Checkpoint** dstCheckpoint);
	bool UpdatePowerUp(float deltaTime);

	// Setters & Getters
	bool IsActive();
	void SetActive(bool isActive);

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float ownAreaRadius;
	float radius;
	PowerUpType type;
	float stunDuration = 1;
	float blockDuration = 5;
	float speedBoost = 2;
	float pitchBoost = 1;
	float yawBoost = 1;
	float rollBoost = 1;
	float speedBoostDuration = 5;
	float speedBoostTimer = 0;
	float stunTimer = 0;
	float blockTimer = 0;
	bool isActive = false;
	Drone* srcDrone = NULL;
	Drone* dstDrone = NULL;
	Checkpoint** dstCheckpoint = NULL;
};