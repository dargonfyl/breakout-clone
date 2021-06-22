#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Shader.hpp>
#include <Sprite_Renderer.hpp>
#include <Texture.hpp>

class Postprocessor {
private:
    // GL variables
    
    /*
    This is a multisampled FBO.
    We will render everything here first to achieve antialiased objects.
    */
    unsigned int MSFBO;

    // ... the above requires a renderbuffer object
    unsigned int RBO;

    /*
    This is a regular FBO.
    This is used to draw to the screen.
    */
    unsigned int FBO;

    // Regular VAO for the pipeline
    unsigned int VAO;

    // Constant variables that should not be changed
    unsigned int width, height;
    Shader postprocessing_shader;
    Texture2D texture;

public:
    // Effect flags
    bool confuse, chaos, shake;

    Postprocessor(Shader shader, const unsigned int &width, const unsigned int &height);

    void begin_render();

    void end_render();

    void render(const float &time);
};
