#version 330 core
layout (location = 0) in vec4 vertex;  // vec2 position + vec2 texcoord

out vec2 tex_coord;
out vec4 particle_color;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;


void main() {
	tex_coord = vertex.zw;

	particle_color = color;

	float scale = 10.0;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}
