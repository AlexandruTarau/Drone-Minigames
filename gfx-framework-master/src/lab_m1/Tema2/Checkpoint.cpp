#include "lab_m1/Tema2/Checkpoint.h"
#include <iostream>

Checkpoint::Checkpoint(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	float width,
	float height,
	float depth,
	float ownAreaRadius
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->ownAreaRadius = ownAreaRadius;
	this->isBlocked = false;
}
Checkpoint::Checkpoint() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->width = 1;
	this->height = 1;
	this->depth = 0.05f;
	this->ownAreaRadius = 2.f;
	this->isBlocked = false;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> Checkpoint::DrawCheckpoint() {
    std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;
    glm::mat4 modelMatrix;

    float scaledWidth = width * scale.x;
    float scaledHeight = height * scale.y;
    float scaledDepth = depth * scale.z;

    glm::vec3 colorFront = glm::vec3(0.8, 0.8, 0.8);
    glm::vec3 colorBack = glm::vec3(0.8, 0.8, 0.8);

    // Top Edge Front (aligned along X-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x, position.y + scaledHeight / 2.f, position.z + scaledDepth / 4.f);
	modelMatrix *= transform3D::Translate(0, -scaledHeight / 2.f, -scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::Translate(0, scaledHeight / 2.f, scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledWidth, scaledDepth, scaledDepth / 2.f);
    
    matrices.push_back(std::tuple(modelMatrix, colorFront, "box"));

    // Top Edge Back (aligned along X-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x, position.y + scaledHeight / 2.f, position.z - scaledDepth / 4.f);
    modelMatrix *= transform3D::Translate(0, -scaledHeight / 2.f, scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::Translate(0, scaledHeight / 2.f, -scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledWidth, scaledDepth, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorBack, "box"));

    // Bottom Edge Front (aligned along X-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x, position.y - scaledHeight / 2.f, position.z + scaledDepth / 4.f);
	modelMatrix *= transform3D::Translate(0, scaledHeight / 2.f, -scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::Translate(0, -scaledHeight / 2.f, scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledWidth, scaledDepth, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorFront, "box"));

    // Bottom Edge Back (aligned along X-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x, position.y - scaledHeight / 2.f, position.z - scaledDepth / 4.f);
    modelMatrix *= transform3D::Translate(0, scaledHeight / 2.f, scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
    modelMatrix *= transform3D::Translate(0, -scaledHeight / 2.f, -scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledWidth, scaledDepth, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorBack, "box"));

    // Right Edge Front (aligned along Z-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x + scaledWidth / 2.f, position.y, position.z + scaledDepth / 4.f);
	modelMatrix *= transform3D::Translate(-scaledWidth / 2.f, 0, -scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::Translate(scaledWidth / 2.f, 0, scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledDepth, scaledHeight, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorFront, "box"));

    // Right Edge Back (aligned along Z-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x + scaledWidth / 2.f, position.y, position.z - scaledDepth / 4.f);
    modelMatrix *= transform3D::Translate(-scaledWidth / 2.f, 0, scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
    modelMatrix *= transform3D::Translate(scaledWidth / 2.f, 0, -scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledDepth, scaledHeight, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorBack, "box"));

    // Left Edge Front (aligned along Z-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x - scaledWidth / 2.f, position.y, position.z + scaledDepth / 4.f);
    modelMatrix *= transform3D::Translate(scaledWidth / 2.f, 0, -scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
    modelMatrix *= transform3D::Translate(-scaledWidth / 2.f, 0, scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledDepth, scaledHeight, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorFront, "box"));

    // Left Edge Back (aligned along Z-axis)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x - scaledWidth / 2.f, position.y, position.z - scaledDepth / 4.f);
	modelMatrix *= transform3D::Translate(scaledWidth / 2.f, 0, scaledDepth / 4.f);
    modelMatrix *= transform3D::RotateOZ(rotation.z);
    modelMatrix *= transform3D::RotateOY(rotation.y);
    modelMatrix *= transform3D::RotateOX(rotation.x);
	modelMatrix *= transform3D::Translate(-scaledWidth / 2.f, 0, -scaledDepth / 4.f);
    modelMatrix *= transform3D::Scale(scaledDepth, scaledHeight, scaledDepth / 2.f);
    matrices.push_back(std::tuple(modelMatrix, colorBack, "box"));

    return matrices;
}


bool Checkpoint::Passed(glm::vec3 playerPosition, glm::vec3 prevPlayerPosition) {
	if (isBlocked) {
		return false;
	}

    const float margin = 0.1f; // Margin to account for small inaccuracies

    // Compute the rotation matrix of the checkpoint
    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

    // Forward vector in world space (Z-axis of the checkpoint's rotation)
    glm::vec3 forward = glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 0));

    // Check dot product with respect to global positions (no transformation back)
    glm::vec3 toPlayerNow = playerPosition - position;
    glm::vec3 toPlayerPrev = prevPlayerPosition - position;

    float currentDot = glm::dot(toPlayerNow, forward);
    float prevDot = glm::dot(toPlayerPrev, forward);

    // Bounding box check to ensure the player is within checkpoint bounds
    glm::vec3 localPlayerPos = glm::inverse(rotationMatrix) * glm::vec4(toPlayerNow, 1.0f);
    bool insideWidth = std::abs(localPlayerPos.x) <= (width / 2.0f) + margin;
    bool insideHeight = std::abs(localPlayerPos.y) <= (height / 2.0f) + margin;
    bool insideDepth = std::abs(localPlayerPos.z) <= (depth / 2.0f) + margin * 4;

    bool insideBounds = insideWidth && insideHeight && insideDepth;

    // Check for crossing: prevDot < 0 means behind, currentDot > 0 means in front
    if (prevDot < 0 && currentDot > 0 && insideBounds) {
        return true;
    }

    return false;
}


glm::vec3 Checkpoint::GetPosition() {
	return position;
}

float Checkpoint::GetOwnAreaRadius() {
	return ownAreaRadius;
}

bool Checkpoint::IsBlocked() {
	return isBlocked;
}

void Checkpoint::SetBlocked(bool isBlocked) {
	this->isBlocked = isBlocked;
}
