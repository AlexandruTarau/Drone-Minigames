#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace primitives3D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateCylinder(const std::string& name, glm::vec3 center, float radius, float height, bool fill = false);
    Mesh* CreateCone(const std::string& name, glm::vec3 center, float radius, float height, bool fill);
    Mesh* CreatePrism(const std::string& name, glm::vec3 center, float width, float height, float depth, bool fill);
    Mesh* CreateArrow(const std::string& name, glm::vec3 color);
}
