#version 330 core
in vec4 particle_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D sprite;


void main() {
	frag_color = texture(sprite, tex_coord) * particle_color;
	// frag_color = vec4(1.0);
}
