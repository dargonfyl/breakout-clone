#version 330 core

in vec2 tex_coord;

uniform sampler2D image;
uniform vec3 sprite_colour;

out vec4 colour;


/*
    Fragment shader for quads.

    @input vec2 tex_coord

    @uniform sampler2D image: texture for the quad. Should be greyscale.
    @uniform vec3 sprite_colour: the actual colour of the sprite.

    @out vec4 colour.
*/
void main() {
    colour = vec4(sprite_colour, 1.0) * texture(image, tex_coord);
}
