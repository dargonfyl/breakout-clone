#pragma once

#include <glad/glad.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"


/**
 * Sprite renderer class.
 * Takes in a shader to render certain kinds of sprites, i.e. quads, menus, buttons, etc.
 */
class Sprite_Renderer {
	private:
		Shader shader;

		unsigned int VAO, VBO;


		/**
		 * Initializes the rendering data, which includes VAO, VBO attributes, EBO, etc.
		 */
		void init_render_data();


	public:
		/**
		 * Constructor for Sprite_Renderer. A reminder that the shader is passed in by reference.
		 * 
		 * 
		 * @param &shader Shader object by reference.
		 * 
		 * @return Sprite_Renderer
		 */
		Sprite_Renderer(Shader &shader);


		/**
		 * Deconstructor for Sprite_Renderer
		 */
		~Sprite_Renderer();


		/**
		 * Draw a sprite.
		 * 
		 * @param texture  Reference to texture to use
		 * @param position (Centroid) position of the quad,
		 * @param size     (Optional) size in (width, height)
		 * @param rotate   (Optional) amount to rotate in in degrees
		 * @param colour   (Optional) colour in normalized RGB
		 */
		void draw_sprite(Texture2D &texture, glm::vec2 position, glm::vec2 size=glm::vec2(10.0f, 10.0f), float rotate=0.0f, glm::vec3 colour=glm::vec3(1.0f));


};
