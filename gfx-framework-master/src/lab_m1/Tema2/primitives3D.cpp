#include "primitives3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* primitives3D::CreateCylinder(
    const std::string& name,
    glm::vec3 center,
    float radius,
    float height,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Base circle center vertex
    vertices.push_back(VertexFormat(center));

    // Up circle center vertex
    vertices.push_back(VertexFormat(glm::vec3(center.x, center.y + height, center.z)));

    float numTriangles = 128;
    float angleStep = 2 * glm::pi<float>() / numTriangles;

    // Generate the half circle
    for (int i = 0; i <= numTriangles; i++) {
        float angle = i * angleStep;
        float x = center.x + radius * cos(angle);
        float z = center.z + radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, center.y, z)));
        vertices.push_back(VertexFormat(glm::vec3(x, center.y + height, z)));
    }

    // Generate the indices for the base and top circles
    for (int i = 1; i <= numTriangles; i++) {
        // Base circle
        indices.push_back(0);
        indices.push_back(2 * i);
        indices.push_back(2 * (i + 1));

        // Top circle
        indices.push_back(1);
        indices.push_back(2 * (i + 1) + 1);
        indices.push_back(2 * i + 1);
    }

    // Generate the indices for the side faces
    for (int i = 1; i <= numTriangles; i++) {
        // First triangle of the quad
        indices.push_back(2 * i);
        indices.push_back(2 * (i + 1));
        indices.push_back(2 * i + 1);

        // Second triangle of the quad
        indices.push_back(2 * i + 1);
        indices.push_back(2 * (i + 1));
        indices.push_back(2 * (i + 1) + 1);
    }

    Mesh* cylinder = new Mesh(name);

    if (!fill) {
        cylinder->SetDrawMode(GL_LINE_LOOP);
    }

    cylinder->InitFromData(vertices, indices);
    return cylinder;
}

Mesh* primitives3D::CreateCone(
    const std::string& name,
    glm::vec3 center,
    float radius,
    float height,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Base circle center vertex
    vertices.push_back(VertexFormat(center));

    // Top vertex of the cone
    vertices.push_back(VertexFormat(glm::vec3(center.x, center.y + height, center.z)));

    float numTriangles = 128;
    float angleStep = 2 * glm::pi<float>() / numTriangles;

    // Generate the vertices for the base circle
    for (int i = 0; i <= numTriangles; i++) {
        float angle = i * angleStep;
        float x = center.x + radius * cos(angle);
        float z = center.z + radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, center.y, z)));
    }

    // Generate the indices for the base circle
    for (int i = 1; i <= numTriangles; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    // Generate the indices for the side faces
    for (int i = 1; i <= numTriangles; i++) {
        indices.push_back(1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    indices.push_back(1);
    indices.push_back(numTriangles);
    indices.push_back(2);

    Mesh* cone = new Mesh(name);

    if (!fill) {
        cone->SetDrawMode(GL_LINE_LOOP);
    }

    cone->InitFromData(vertices, indices);
    return cone;
}

Mesh* primitives3D::CreatePrism(const std::string& name, glm::vec3 center, float width, float height, float depth, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Half size for positioning
    glm::vec3 halfDepth = glm::vec3(0, 0, depth / 2.0f);

    // Define vertices for the triangular prism
    vertices.push_back(VertexFormat(center + glm::vec3(-width / 2, -height / 2, -halfDepth.z), glm::vec3(0, 0, -1))); // 0
    vertices.push_back(VertexFormat(center + glm::vec3(width / 2, -height / 2, -halfDepth.z), glm::vec3(0, 0, -1)));  // 1
    vertices.push_back(VertexFormat(center + glm::vec3(0, height / 2, -halfDepth.z), glm::vec3(0, 0, -1)));           // 2

    vertices.push_back(VertexFormat(center + glm::vec3(-width / 2, -height / 2, halfDepth.z), glm::vec3(0, 0, 1)));   // 3
    vertices.push_back(VertexFormat(center + glm::vec3(width / 2, -height / 2, halfDepth.z), glm::vec3(0, 0, 1)));    // 4
    vertices.push_back(VertexFormat(center + glm::vec3(0, height / 2, halfDepth.z), glm::vec3(0, 0, 1)));             // 5

    // Define the indices for faces
    indices = {
        // Front triangular face
        0, 1, 2,
        // Back triangular face
        3, 5, 4,
        // Bottom rectangular face
        0, 3, 4, 0, 4, 1,
        // Left rectangular face
        0, 2, 5, 0, 5, 3,
        // Right rectangular face
        1, 4, 5, 1, 5, 2
    };

    // Create the mesh and return it
    Mesh* prism = new Mesh(name);
    prism->InitFromData(vertices, indices);
    return prism;
}

Mesh* primitives3D::CreateArrow(const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Shaft (cylindrical-like part of the arrow)
    float shaftLength = 1.0f; // Length of the arrow shaft
    float shaftRadius = 0.05f; // Thickness of the shaft
    glm::vec3 shaftStart = glm::vec3(0.0f, 0.0f, 0.0f); // Base of the arrow shaft
    glm::vec3 shaftEnd = glm::vec3(0.0f, 0.0f, shaftLength); // End of the arrow shaft

    // Shaft vertices (rectangular prism)
    vertices = {
        // Bottom face (near shaftStart)
        VertexFormat(shaftStart + glm::vec3(-shaftRadius, -shaftRadius, 0.0f), color),
        VertexFormat(shaftStart + glm::vec3(shaftRadius, -shaftRadius, 0.0f), color),
        VertexFormat(shaftStart + glm::vec3(shaftRadius, shaftRadius, 0.0f), color),
        VertexFormat(shaftStart + glm::vec3(-shaftRadius, shaftRadius, 0.0f), color),

        // Top face (near shaftEnd)
        VertexFormat(shaftEnd + glm::vec3(-shaftRadius, -shaftRadius, 0.0f), color),
        VertexFormat(shaftEnd + glm::vec3(shaftRadius, -shaftRadius, 0.0f), color),
        VertexFormat(shaftEnd + glm::vec3(shaftRadius, shaftRadius, 0.0f), color),
        VertexFormat(shaftEnd + glm::vec3(-shaftRadius, shaftRadius, 0.0f), color),
    };

    indices = {
        0, 1, 2,  2, 3, 0, // Bottom face
        4, 5, 6,  6, 7, 4, // Top face
        0, 1, 5,  5, 4, 0, // Front face
        1, 2, 6,  6, 5, 1, // Right face
        2, 3, 7,  7, 6, 2, // Back face
        3, 0, 4,  4, 7, 3  // Left face
    };

    // Tip (cone-like part of the arrow)
    float tipLength = 0.3f; // Length of the arrow tip
    float tipBaseRadius = 0.1f; // Radius of the base of the tip
    glm::vec3 tipBaseCenter = shaftEnd; // Center of the tip's base
    glm::vec3 tipVertex = shaftEnd + glm::vec3(0.0f, 0.0f, tipLength); // Tip point

    // Create vertices for the base of the tip (circle approximation)
    int numSegments = 16; // Number of segments for the circular base
    float angleStep = 2.0f * glm::pi<float>() / numSegments;
    unsigned int baseStartIndex = vertices.size();

    for (int i = 0; i < numSegments; i++) {
        float angle = i * angleStep;
        float x = tipBaseRadius * cos(angle);
        float y = tipBaseRadius * sin(angle);
        vertices.emplace_back(tipBaseCenter + glm::vec3(x, y, 0.0f), color);
    }

    // Add the tip vertex
    unsigned int tipVertexIndex = vertices.size();
    vertices.emplace_back(tipVertex, color);

    // Indices for the tip
    for (int i = 0; i < numSegments; i++) {
        unsigned int next = (i + 1) % numSegments;
        indices.push_back(baseStartIndex + i);
        indices.push_back(baseStartIndex + next);
        indices.push_back(tipVertexIndex);
    }

    // Create the mesh
    Mesh* arrow = new Mesh(name);
    arrow->InitFromData(vertices, indices);

    return arrow;
}
