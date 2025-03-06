#include "lab_m1/Tema2/Drone.h"
#include <iostream>

Drone::Drone(
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
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->colliderRadius = colliderRadius;
	this->thrust = thrust;
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
	this->angularSpeed = angularSpeed;
	this->propellerAngularSpeed = propellerAngularSpeed;
	this->propellersRotation = 0;
	this->dashCooldown = dashCooldown;
	this->dashDuration = dashDuration;
	this->dashMultiplier = dashMultiplier;
	this->damageFactor = damageFactor;
    forward = glm::vec3(0, 0, -1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);
	isDashing = false;
	barWidth = 0.5f;
	barHeight = 0.1f;
	isStunned = false;
}
Drone::Drone() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->colliderRadius = 1;
	this->thrust = 0;
    this->pitch = 0;
	this->yaw = 0;
	this->roll = 0;
	this->angularSpeed = 0;
	this->propellerAngularSpeed = 0;
	this->propellersRotation = 0;
	this->dashCooldown = 0;
	this->dashDuration = 0;
	this->dashMultiplier = 0;
    this->damageFactor = 0;
    forward = glm::vec3(0, 0, -1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);
	isDashing = false;
    barWidth = 0.5f;
    barHeight = 0.1f;
	isStunned = false;
}

std::vector<std::pair<glm::mat4, DronePart>> Drone::DrawDrone() {
	std::vector<std::pair<glm::mat4, DronePart>> matrices;
	glm::mat4 modelMatrix;

	float scaledBaseWidth = baseWidth * scale.x;
	float scaledBaseHeight = baseHeight * scale.y;
	float scaledBaseDepth = baseDepth * scale.z;
	float scaledPropellerWidth = propellerWidth * scale.x;
	float scaledPropellerHeight = propellerHeight * scale.y;
	float scaledPropellerDepth = propellerDepth * scale.z;

    // Base 1
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::RotateOY(M_PI_4);
        modelMatrix *= transform3D::Scale(scaledBaseWidth, scaledBaseHeight, scaledBaseDepth);
        matrices.push_back(std::make_pair(modelMatrix, BASE));
    }

    // Base 2
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::RotateOY(-M_PI_4);
        modelMatrix *= transform3D::Scale(scaledBaseWidth, scaledBaseHeight, scaledBaseDepth);
        matrices.push_back(std::make_pair(modelMatrix, BASE));
    }

	// Propeller support 1.1
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
		modelMatrix *= transform3D::RotateOX(rotation.x);
		modelMatrix *= transform3D::RotateOY(rotation.y);
		modelMatrix *= transform3D::RotateOZ(rotation.z);
		modelMatrix *= transform3D::Translate(
			(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(M_PI_4),
            scaledBaseHeight,
			(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(M_PI_4));
		modelMatrix *= transform3D::RotateOY(M_PI_4);
		modelMatrix *= transform3D::Scale(scaledBaseDepth, scaledBaseDepth, scaledBaseDepth);
		matrices.push_back(std::make_pair(modelMatrix, BASE));
	}

    // Propeller support 1.2
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(M_PI_4),
            scaledBaseHeight,
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(M_PI_4));
        modelMatrix *= transform3D::RotateOY(M_PI_4);
        modelMatrix *= transform3D::Scale(scaledBaseDepth, scaledBaseDepth, scaledBaseDepth);
        matrices.push_back(std::make_pair(modelMatrix, BASE));
    }

    // Propeller support 2.1
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(-M_PI_4),
            scaledBaseHeight,
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(-M_PI_4));
        modelMatrix *= transform3D::RotateOY(-M_PI_4);
        modelMatrix *= transform3D::Scale(scaledBaseDepth, scaledBaseDepth, scaledBaseDepth);
        matrices.push_back(std::make_pair(modelMatrix, BASE));
    }

    // Propeller support 2.2
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(-M_PI_4),
            scaledBaseHeight,
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(-M_PI_4));
        modelMatrix *= transform3D::RotateOY(-M_PI_4);
        modelMatrix *= transform3D::Scale(scaledBaseDepth, scaledBaseDepth, scaledBaseDepth);
        matrices.push_back(std::make_pair(modelMatrix, BASE));
    }

    // Propeller 1.1
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(M_PI_4),
            scaledBaseHeight + scaledBaseDepth / 2.f + scaledPropellerHeight / 2.f,
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(M_PI_4));
        modelMatrix *= transform3D::RotateOY(propellersRotation);
        modelMatrix *= transform3D::Scale(scaledPropellerWidth, scaledPropellerHeight, scaledPropellerDepth);
        matrices.push_back(std::make_pair(modelMatrix, PROPELLERS));
    }

    // Propeller 1.2
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(M_PI_4),
            scaledBaseHeight + scaledBaseDepth / 2.f + scaledPropellerHeight / 2.f,
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(M_PI_4));
        modelMatrix *= transform3D::RotateOY(propellersRotation);
        modelMatrix *= transform3D::Scale(scaledPropellerWidth, scaledPropellerHeight, scaledPropellerDepth);
        matrices.push_back(std::make_pair(modelMatrix, PROPELLERS));
    }

    // Propeller 2.1
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(-M_PI_4),
            scaledBaseHeight + scaledBaseDepth / 2.f + scaledPropellerHeight / 2.f,
            (scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(-M_PI_4));
        modelMatrix *= transform3D::RotateOY(propellersRotation);
        modelMatrix *= transform3D::Scale(scaledPropellerWidth, scaledPropellerHeight, scaledPropellerDepth);
        matrices.push_back(std::make_pair(modelMatrix, PROPELLERS));
    }

	// Propeller 2.2
	{
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::RotateOX(rotation.x);
        modelMatrix *= transform3D::RotateOY(rotation.y);
        modelMatrix *= transform3D::RotateOZ(rotation.z);
        modelMatrix *= transform3D::Translate(
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * cos(-M_PI_4),
            scaledBaseHeight + scaledBaseDepth / 2.f + scaledPropellerHeight / 2.f,
            -(scaledBaseWidth / 2.f - scaledBaseDepth / 2.f) * sin(-M_PI_4));
        modelMatrix *= transform3D::RotateOY(propellersRotation);
        modelMatrix *= transform3D::Scale(scaledPropellerWidth, scaledPropellerHeight, scaledPropellerDepth);
		matrices.push_back(std::make_pair(modelMatrix, PROPELLERS));
	}

	return matrices;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> Drone::DrawDashCD(implemented::Camera* camera, const glm::mat4& projectionMatrix, float posY) {
    std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;

    // Identity matrix to start
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Position the timer bar in front of the camera, slightly above the center
    glm::vec3 cameraPos = camera->position;
    glm::vec3 cameraForward = camera->forward;
    glm::vec3 barPosition = cameraPos + cameraForward * 2.0f + glm::vec3(0.0f, posY, 0.0f);  // Offset in y for height adjustment

    // Apply translation for bar position
    modelMatrix = glm::translate(glm::mat4(1.0f), barPosition);

    // Billboard effect: Align the bar to always face the camera
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    viewMatrix[3] = glm::vec4(0, 0, 0, 1);  // Reset translation in the view matrix
    modelMatrix *= glm::inverse(viewMatrix);  // Apply inverse for camera rotation alignment

    // Calculate the width of the timer bar based on remaining cooldown time
    float progress = (dashCooldownTimer > 0.0f) ? dashCooldownTimer / dashCooldown : 0.0f;
    float barWidthScale = barWidth * progress;  // Adjust width proportionally to cooldown progress

    // Apply scaling: Width is dynamic, height and depth are fixed
    modelMatrix = glm::scale(modelMatrix, glm::vec3(barWidthScale, barHeight, 0.01f));  // Thin depth

    // Add the cooldown bar (green) to the rendering list
    matrices.push_back(std::tuple<glm::mat4, glm::vec3, std::string>(modelMatrix, glm::vec3(0.0f, 3.0f, 6.0f), "box"));

    return matrices;
}


bool Drone::CheckCollisionWithStaticObjects(glm::vec3 position, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    // Check if collision with terrain (XOZ plane)
    if (position.y - colliderRadius <= 0.5) {
        return false;
    }

    // Check collision with world borders
    if (position.x <= 0 || position.z <= 0 || position.x >= maxHeight || position.z >= maxWidth) {
        return false;
    }

	for (auto& obstacle : obstacles) {
		if (obstacle->IsColliding(position, colliderRadius)) {
			return false;
		}
	}

    return true;
}

void Drone::RotatePropellers(float deltaTime) {
    if (!isStunned) {
        propellersRotation += propellerAngularSpeed * deltaTime;
    }
}

void Drone::MoveForward(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
	if (!isStunned) {
		glm::vec3 newPosition = position + forward * thrust * deltaTime;
		if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
			position = newPosition;
		}
	}
}

void Drone::MoveRight(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (!isStunned) {
        glm::vec3 newPosition = position + right * thrust * deltaTime;
        if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
            position = newPosition;
        }
    }
}

void Drone::MoveUp(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (!isStunned) {
        glm::vec3 newPosition = position + up * thrust * deltaTime;
        if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
            position = newPosition;
        }
    }
}

void Drone::MoveBackward(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (!isStunned) {
        glm::vec3 newPosition = position - forward * thrust * deltaTime;
        if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
            position = newPosition;
        }
    }
}

void Drone::MoveLeft(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (!isStunned) {
        glm::vec3 newPosition = position - right * thrust * deltaTime;
        if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
            position = newPosition;
        }
    }
}

void Drone::MoveDown(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (!isStunned) {
        glm::vec3 newPosition = position - up * thrust * deltaTime;
        if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
            position = newPosition;
        }
    }
}

void Drone::RotateOX(float deltaTime) {
    if (!isStunned) {
        rotation.x += pitch * deltaTime;
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), pitch * deltaTime, right);
        forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));
    }
}

void Drone::RotateOY(float deltaTime) {
    if (!isStunned) {
        rotation.y += yaw * deltaTime;
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), yaw * deltaTime, up);
        forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));
        right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));
    }
}

void Drone::RotateOZ(float deltaTime) {
    if (!isStunned) {
        rotation.z += roll * deltaTime;
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -roll * deltaTime, forward);
        right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));
        forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));
    }
}

void Drone::ApplyGravity(float deltaTime, float gravity, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
	glm::vec3 newPosition = position - glm::vec3(0, 1, 0) * gravity * deltaTime;
	if (CheckCollisionWithStaticObjects(newPosition, maxHeight, maxWidth, obstacles)) {
		position = newPosition;
	}
}

void Drone::StartDash() {
    if (!isStunned) {
        if (dashCooldownTimer <= 0.0f) {
            isDashing = true;
            dashTimer = dashDuration;
        }
    }
}

void Drone::UpdateDash(float deltaTime, float maxHeight, float maxWidth, std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    if (isDashing) {
		MoveForward(dashMultiplier * deltaTime, maxHeight, maxWidth, obstacles);
        dashTimer -= deltaTime;
        if (dashTimer <= 0.0f) {
            isDashing = false;
            dashCooldownTimer = dashCooldown;
        }
    }

    if (dashCooldownTimer > 0.0f) {
        dashCooldownTimer -= deltaTime;
    }
}

void Drone::ApplySpeedBoost(float speedBoost, float pitchBoost, float yawBoost, float rollBoost) {
	thrust += speedBoost;
	pitch += pitchBoost;
	yaw += yawBoost;
	roll += rollBoost;
}

// Getters and Setters
glm::vec3 Drone::GetPosition() const {
	return position;
}

void Drone::SetPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Drone::GetForward() const {
	return forward;
}

glm::vec3 Drone::GetRight() const {
	return right;
}

glm::vec3 Drone::GetUp() const {
	return up;
}

glm::vec3 Drone::GetScale() const {
	return scale;
}

void Drone::SetScale(glm::vec3 scale) {
	this->scale = scale;
}

glm::vec3 Drone::GetRotation() const {
	return rotation;
}

void Drone::SetRotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

bool Drone::IsDashing() const {
	return isDashing;
}

float Drone::GetDamageFactor() const {
	return damageFactor;
}

void Drone::SetDamageFactor(float damageFactor) {
	this->damageFactor = damageFactor;
}

bool Drone::IsStunned() const {
	return isStunned;
}

void Drone::SetStunned(bool isStunned) {
	this->isStunned = isStunned;
}

float Drone::GetColliderRadius() const {
	return colliderRadius;
}