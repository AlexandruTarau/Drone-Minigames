#include "Cloud.h"
#include <glm/glm.hpp>
#include <memory>
#include <random>

Cloud::Cloud(
	glm::vec3 position,
	glm::vec3 scale,
	glm::vec3 rotation,
	glm::vec3 velocity,
	float partsNumber,
	float speed
) : position(position), scale(scale), rotation(rotation), velocity(velocity), partsNumber(partsNumber), speed(speed) {}

Cloud::Cloud() : position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), partsNumber(1), speed(1) {}

void Cloud::UpdateCloud(float deltaTime, glm::vec3 mapBounds) {
    position += velocity * speed * deltaTime;

    if (position.x > mapBounds.x) {
        position.x = 0;
    }
    else if (position.x < 0) {
        position.x = mapBounds.x;
    }

    if (position.z > mapBounds.z) {
        position.z = 0;
    }
    else if (position.z < 0) {
        position.z = mapBounds.z;
    }
}


std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> Cloud::DrawCloud() {
    std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> cloudParts;

    glm::mat4 baseTransform = glm::mat4(1.0f);
    baseTransform = glm::translate(baseTransform, position);
    baseTransform = glm::rotate(baseTransform, rotation.x, glm::vec3(1, 0, 0));
    baseTransform = glm::rotate(baseTransform, rotation.y, glm::vec3(0, 1, 0));
    baseTransform = glm::rotate(baseTransform, rotation.z, glm::vec3(0, 0, 1));

    glm::vec3 partColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 cloudBaseScale = scale;

    for (int i = 0; i < partsNumber; i++) {
        glm::vec3 offset(
            (i % 3 - 1) * cloudBaseScale.x * (0.6f + 0.3f * (i % 2)),
            (i % 2 == 0 ? cloudBaseScale.y * 0.3f : -cloudBaseScale.y * 0.3f),
            (i - partsNumber / 2.0f) * cloudBaseScale.z * 0.4f
        );

        glm::vec3 randomScale = glm::vec3(
            cloudBaseScale.x * (0.8f + 0.3f * ((i + 1) % 3)),
            cloudBaseScale.y * (0.9f + 0.2f * (i % 2)),
            cloudBaseScale.z * 0.8f
        );

        glm::mat4 partTransform = glm::translate(baseTransform, offset);
        partTransform = glm::scale(partTransform, randomScale);

        cloudParts.push_back(std::make_tuple(partTransform, partColor, "box"));
    }

    return cloudParts;
}

void Cloud::GenerateClouds(std::vector<Cloud>& clouds, int cloudCount, glm::vec3 mapSize) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distX(0.f, mapSize.x);
    std::uniform_real_distribution<float> distZ(0.f, mapSize.z);
    std::uniform_real_distribution<float> distY(mapSize.y - 2.f, mapSize.y + 2.f);
    std::uniform_real_distribution<float> distSpacing(10.f, 20.f);
    std::uniform_int_distribution<int> distParts(1, 6);
    std::uniform_real_distribution<float> distSpeed(8.f, 15.f);
    std::uniform_real_distribution<float> distScaleX(2.f, 3.f);
    std::uniform_real_distribution<float> distScaleY(1.f, 1.5f);

    std::vector<glm::vec3> cloudPositions;
    cloudPositions.reserve(cloudCount);

    float lastX = 0.0f;

    for (int i = 0; i < cloudCount; i++) {
        glm::vec3 cloudPosition;

        cloudPosition.x = distX(rng);
        cloudPosition.y = distY(rng);
        cloudPosition.z = distZ(rng);

        bool validPosition = true;
        for (const glm::vec3& pos : cloudPositions) {
            if (glm::distance(pos, cloudPosition) < 10.f) {
                validPosition = false;
                break;
            }
        }

        if (validPosition) {
            cloudPositions.push_back(cloudPosition);

            int partsNumber = distParts(rng);
            float speed = distSpeed(rng);

            glm::vec3 scale(
                distScaleX(rng),
                distScaleY(rng),
                distScaleX(rng)
            );

            clouds.push_back(Cloud(cloudPosition, scale, glm::vec3(0.f), glm::vec3(0.1f, 0.f, 0.f), partsNumber, speed));

            lastX = cloudPosition.x;
        }
        else {
            i--;
        }
    }
}



// Getters and setters
glm::vec3 Cloud::GetPosition() const {
    return position;
}

void Cloud::SetPosition(const glm::vec3 position) {
    this->position = position;
}
