#version 330

// Input vertex attributes
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_normal;

// Uniforms for transformations
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 color;
uniform float damageFactor;

// Outputs to the fragment shader
out vec4 frag_color;
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coord;

void main()
{
    // Pass color (including alpha) to fragment shader
    frag_color = color;

    // Transform vertex position to world space and pass it
    vec3 deform_position = v_position + v_normal * damageFactor;
    frag_position = vec3(Model * vec4(deform_position, 1.0));

    // Transform the normal (important for lighting calculations)
    frag_normal = mat3(transpose(inverse(Model))) * v_normal;

    // Pass texture coordinates to fragment shader
    frag_tex_coord = v_tex_coord;

    // Compute final position in clip space
    gl_Position = Projection * View * vec4(frag_position, 1.0);
}