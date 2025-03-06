#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include "lab_m1/Tema2/Obstacle.h"
#include <vector>
#include <tuple>
#include "lab_m1/lab5/lab_camera.h"

// Enum for drone parts
enum DronePart {
	BASE,
	PROPELLERS,
};

class Drone
{
public:
	static constexpr float baseWidth = 1.f;
	static constexpr float baseHeight = 0.1f;
	static constexpr float baseDepth = 0.1f;
	static constexpr float propellerWidth = 0.2f;
	static constexpr float propellerHeight = 0.02f;
	static constexpr float propellerDepth = 0.02f;

	Drone(
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		float colliderRadius,
		float thrust,
		float pitch,
		float yaw,
		float roll,
		float angularSpeed,
		float propellerAngularSpeed,
		float dashMultiplier,
		float dashDuration,
		float dashCooldown,
		float damageFactor
		);
	Drone();

	std::vector<std::pair<glm::mat4, DronePart>> DrawDrone();
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawDashCD(implemented::Camera* camera, const glm::mat4& projectionMatrix, float posY);
	void MoveForward(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void MoveBackward(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void MoveRight(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void MoveLeft(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void MoveUp(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void MoveDown(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void RotateOX(float deltaTime);
	void RotateOY(float deltaTime);
	void RotateOZ(float deltaTime);
	void RotatePropellers(float deltaTime);
	bool CheckCollisionWithStaticObjects(glm::vec3 position, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void ApplyGravity(float deltaTime, float gravity, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void StartDash();
	void UpdateDash(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles);
	void ApplySpeedBoost(float speedBoost, float pitchBoost, float yawBoost, float rollBoost);

	// Getter and setters
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetScale() const;
	void SetScale(glm::vec3 scale);
	glm::vec3 GetRotation() const;
	void SetRotation(glm::vec3 rotation);
	bool IsDashing() const;
	float GetDamageFactor() const;
	void SetDamageFactor(float damageFactor);
	bool IsStunned() const;
	void SetStunned(bool isStunned);
	float GetColliderRadius() const;

protected:
	// Main body
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float thrust;
	float pitch;
	float yaw;
	float roll;
	float angularSpeed;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	float colliderRadius;
	float damageFactor;
	bool isStunned;

	// Propellers
	float propellersRotation;
	float propellerAngularSpeed;

	// Dash
	float dashMultiplier;
	float dashDuration;
	float dashCooldown;
	bool isDashing;
	float dashTimer;
	float dashCooldownTimer;
	float barWidth;
	float barHeight;
};