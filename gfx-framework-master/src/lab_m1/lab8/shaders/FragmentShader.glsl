#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 light_direction2;
uniform vec3 light_position2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int light_type1;
uniform int light_type2;
uniform float cut_off_angle1;
uniform float cut_off_angle2;

// Output
layout(location = 0) out vec4 out_color;

// Function to calculate the contribution of a point light
void point_light_contribution(vec3 light_pos, float light_color, out float diffuse_contribution, out float specular_contribution)
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_pos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, N);

    float ambient_light = 0.25;

    // Compute diffuse light component
    diffuse_contribution = material_kd * light_color * max(dot(N, L), 0);

    // Compute specular light component
    specular_contribution = material_ks * light_color * pow(max(dot(V, R), 0), material_shininess);
}


void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
    float light_color = 1;

    float ambient_light = 0.25;
    float diffuse_light1 = 0;
    float specular_light1 = 0;
    float diffuse_light2 = 0;
    float specular_light2 = 0;

    vec3 L = normalize(light_position - world_position);
    vec3 L2 = normalize(light_position2 - world_position);

    point_light_contribution(light_position, light_color, diffuse_light1, specular_light1);
    point_light_contribution(light_position2, light_color, diffuse_light2, specular_light2);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!

    float dampingFactor1 = 1.f / pow(distance(light_position, world_position), 2);
    float dampingFactor2 = 1.f / pow(distance(light_position2, world_position), 2);

    float light1, light2;

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if (light_type1 == 1) {
        float cut_off = radians(cut_off_angle1);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);

        // Quadratic attenuation
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        if (spot_light > spot_light_limit) {
			light1 = ambient_light + dampingFactor1 * light_att_factor * (diffuse_light1 + specular_light1);
		} else {
            light1 = ambient_light;
		}
	} else {
        light1 = ambient_light + dampingFactor1 * (diffuse_light1 + specular_light1);
    }

    if (light_type2 == 1) {
        float cut_off = radians(cut_off_angle2);
        float spot_light = dot(-L2, light_direction2);
        float spot_light_limit = cos(cut_off);

        // Quadratic attenuation
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        if (spot_light > spot_light_limit) {
			light2 = ambient_light + dampingFactor2 * light_att_factor * (diffuse_light2 + specular_light2);
		} else {
            light2 = ambient_light;
		}
	} else {
        light2 = ambient_light + dampingFactor2 * (diffuse_light2 + specular_light2);
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * (light1 + light2), 1);
}
