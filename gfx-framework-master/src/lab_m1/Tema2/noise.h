#include <cmath>
#include <glm/glm.hpp>

float fade(double t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float noise(glm::vec3 pos) {
    glm::vec3 p = glm::floor(pos);
    glm::vec3 f = glm::fract(pos);
    
    f.x = fade(f.x);
    f.y = fade(f.y);
    f.z = fade(f.z);

    double n = glm::dot(p, glm::vec3(1.0f, 57.0f, 113.0f));

    float res = glm::mix(
        glm::mix(glm::mix(glm::fract(glm::sin(n) * 43758.5453f), glm::fract(glm::sin(n + 1.0f) * 43758.5453f), f.x),
            glm::mix(glm::fract(glm::sin(n + 57.0f) * 43758.5453f), glm::fract(glm::sin(n + 58.0f) * 43758.5453f), f.x), f.y),
        glm::mix(glm::mix(glm::fract(glm::sin(n + 113.0f) * 43758.5453f), glm::fract(glm::sin(n + 114.0f) * 43758.5453f), f.x),
            glm::mix(glm::fract(glm::sin(n + 170.0f) * 43758.5453f), glm::fract(glm::sin(n + 171.0f) * 43758.5453f), f.x), f.y),
        f.z
    );

    return res;
}