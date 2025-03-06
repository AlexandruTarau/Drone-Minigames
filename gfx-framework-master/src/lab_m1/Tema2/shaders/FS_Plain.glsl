#version 330

// Input interpolated from the vertex shader
in vec4 frag_color;        // Fragment color (including alpha)
in vec3 frag_position;     // Fragment world position (from vertex shader)

// Outputs to the framebuffer
layout(location = 0) out vec4 out_color;         // Output color (RGBA)
layout(location = 1) out vec4 out_world_position; // Output world position

void main()
{
    // Set the pixel color using frag_color (includes alpha)
    out_color = frag_color;

    // Set the world position as a vec4 (w = 1.0 for positions)
    out_world_position = vec4(frag_position, 1.0);
}
