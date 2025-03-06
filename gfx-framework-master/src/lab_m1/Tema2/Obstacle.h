#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include <vector>
#include <tuple>

class Obstacle {
public:
    Obstacle(
        glm::vec3 position,
        glm::vec3 scale,
        glm::vec3 rotation,
        float ownAreaRadius
    );

    Obstacle();

    virtual std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawObstacle() = 0;
	virtual bool IsColliding(glm::vec3 playerPosition, float playerRadius) = 0;

    float GetOwnAreaRadius();

	// Setters & Getters
	glm::vec3 GetPosition();

protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float ownAreaRadius;
};
