#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float noiseScale;
uniform float noiseAmplitude;
uniform float heightThresholds[3];
uniform vec3 terrainColors[3];

out vec3 frag_color;
out vec3 frag_position;

float random(vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

float fade(float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float noise(vec3 pos) {
    vec3 p = floor(pos);
    vec3 f = fract(pos);

    f.x = fade(f.x);
    f.y = fade(f.y);
    f.z = fade(f.z);

    float n = dot(p, vec3(1.0, 57.0, 113.0));
    
    float res = mix(
        mix(mix(fract(sin(n) * 43758.5453), fract(sin(n + 1.0) * 43758.5453), f.x),
            mix(fract(sin(n + 57.0) * 43758.5453), fract(sin(n + 58.0) * 43758.5453), f.x), f.y),
        mix(mix(fract(sin(n + 113.0) * 43758.5453), fract(sin(n + 114.0) * 43758.5453), f.x),
            mix(fract(sin(n + 170.0) * 43758.5453), fract(sin(n + 171.0) * 43758.5453), f.x), f.y),
        f.z
    );

    return res;
}


void main() {
    vec3 position = v_position;
    float newThresholds[3];

    // Change vertex y coordinate
    float noiseVal = noise(position * noiseScale) * noiseAmplitude;
    position.y += noiseVal;

    // Generate random variation for thresholds
    newThresholds[0] = heightThresholds[0];
    for (int i = 1; i < 2; ++i) {
        float variation = (random(vec2(position.x, position.z)) * 2.0f - 1.0f) * (heightThresholds[i] * 0.1f);
        newThresholds[i] = min(heightThresholds[i] + variation, heightThresholds[2]);
    }
    newThresholds[2] = heightThresholds[2];

    // Color variation
    for (int i = 0; i < 3; ++i) {
        if (position.y < newThresholds[i]) {
            // Generate a random value based on the fragment position and index
            vec2 st = vec2(position.x + float(i), position.z); // Add the index to make it more consistent
            float randValue = random(st); // Randomness based on position and index

            // Apply randomness to color, with a smoother range
            // Scale the random variation to be smaller (±0.05 or ±0.1)
            vec3 randomColor = terrainColors[i] + (randValue - 0.5f) * 0.05f; // Smooth color variation

            // Set the final color
            //frag_color = randomColor;
            frag_color = mix(terrainColors[i], randomColor, noiseVal);
            break;
        }
    }


    //frag_position = position;

    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
