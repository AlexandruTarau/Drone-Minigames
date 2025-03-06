#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2Dmodified
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftTopCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTank(const std::string& name, glm::vec3 bottomMiddleCenter, float length, float height, float radius, glm::vec3 bottomColor, glm::vec3 topColor, bool fill = false);
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
    Mesh* CreateLine(const std::string& name, glm::vec3 color);
}
#pragma once
