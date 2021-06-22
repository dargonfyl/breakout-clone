#version 330 core

in vec2 tex_coord;

uniform sampler2D image;

// Filters
uniform int   edge_kernel[9];
uniform float blur_kernel[9];

// Offsets for particular effects
uniform vec2 offsets[9];

struct Effects {
    float time;
    bool chaos;
    bool confuse;
    bool shake;
};

uniform Effects effects;

out vec4 colour;


/*
Invert the rgb of the pixel at tex_coord and set alpha=1.0
*/
vec4 invert_colours() {
    return vec4(vec3(1.0) - texture(image, tex_coord).rgb, 1.0);
}


/*
Draws a quad and applies effects, if any. Meant to be used with the framebuffer.
*/
void main() {
    colour = vec4(0.0);

    vec3 sample[9];

    // Sample the 9 pixels of the original texture if a convolve is used
    if (effects.chaos || effects.shake) {
        for (int i = 0; i < 9; i++) {
            sample[i] = texture(image, tex_coord + offsets[i]).rgb;
        }
    }
    
    if (effects.chaos) {
        for (int i = 0; i < 9; i++) {
            colour += vec4(sample[i] * edge_kernel[i], 1.0);
        }
        colour.a = 1.0;
    } else if (effects.confuse) {
        colour = invert_colours();
    } else if (effects.shake) {
        for (int i = 0; i < 9; i++) {
            colour += vec4(sample[i] * blur_kernel[i], 1.0);
        }
        colour.a = 1.0;
    } else {
        colour = texture(image, tex_coord);
    }
}
