// Postprocessor.vs

#version 330 core

#define EFFECTS_FACTOR 0.3
#define SHAKE_FACTOR 0.01

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 layout_tex_coord;

// Effects Uniforms
struct Effects {
    float time;
    bool  chaos;
    bool  confuse;
    bool  shake;
};
uniform Effects effects;

out vec2 tex_coord;


/*
Framebuffer and geometric effects on framebuffers
*/
void main() {
    gl_Position = vec4(vertex, 0.0, 1.0);

    float time = effects.time;
    tex_coord = layout_tex_coord;
    if (effects.chaos) {
        vec2 pos = vec2(layout_tex_coord.x + sin(time) * EFFECTS_FACTOR, layout_tex_coord.y + cos(time) * EFFECTS_FACTOR);

        tex_coord = tex_coord;
    } else if (effects.confuse) {
        tex_coord = vec2(1.0 - layout_tex_coord.x, 1.0 - layout_tex_coord.y);
    }

    if (effects.shake) {
        gl_Position.x += cos(time * 10.0) * SHAKE_FACTOR;
        gl_Position.y += cos(time * 15.0) * SHAKE_FACTOR;
    }
}
