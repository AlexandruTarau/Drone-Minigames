#include "lab_m1/Tema2/Obstacle.h"

Obstacle::Obstacle(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	float ownAreaRadius
) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->ownAreaRadius = ownAreaRadius;
}
Obstacle::Obstacle() {
	this->position = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation = glm::vec3(0, 0, 0);
	this->ownAreaRadius = 1;
}
float Obstacle::GetOwnAreaRadius() {
	return ownAreaRadius;
}

glm::vec3 Obstacle::GetPosition() {
	return position;
}