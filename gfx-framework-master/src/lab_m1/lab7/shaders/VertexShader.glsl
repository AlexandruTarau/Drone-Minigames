#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    float light_color = 1;

    // TODO(student): Compute world space vectors
    vec3 world_pos = (Model * vec4(v_position, 1)).xyz;

    // Normal vector
    vec3 N = normalize(mat3(Model) * v_normal);

    // Light vector
    vec3 L = normalize(light_position - world_pos);

    // Eye vector
    vec3 V = normalize(eye_position - world_pos);

    // TODO(student): Define ambient light component
    float ambient_light = light_color * 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * light_color * max(dot(N, L), 0);

    // TODO(student): Compute specular light component
    vec3 R = reflect(-L, N);
    float receives_light = dot(N, L) > 0 ? 1 : 0;
    float specular_light = material_ks * light_color * receives_light * pow(max(dot(V, R), 0), material_shininess);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        
    }

    // TODO(student): Compute light
    float dampingFactor = 1.f / pow(distance(light_position, world_pos), 2);

    float light = ambient_light + dampingFactor * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    color = object_color * light;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
