#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 layout_tex_coord;

uniform mat4 model;
uniform mat4 projection;

out vec2 tex_coord;


/*
Vertex shader for quad objects.

@input location = 0 vec2 vertex: vertex position
@input location = 1 vec2 layout_tex_coord: texture coordinate

@uniform mat4 model
@uniform mat4 projection: an orthogonal projection matrix. No need for view matrix for 2D.

@output vec2 tex_coord: texture coordinate for fragment shader
 */
void main() {
    tex_coord = layout_tex_coord;

    gl_Position = projection * model * vec4(vertex, 0.0, 0.0); 
}
