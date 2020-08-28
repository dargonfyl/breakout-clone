#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <math.h>
#include <assert.h>

#include "Texture.hpp"
#include "Sprite_Renderer.hpp"
#include "Game_Object.hpp"


/**
 * Tile wrapper for Game_Object. Has its own destroy().
 */
class Tile : public Game_Object {
	private:
		bool breakable;

		/**
		 * Destroy for Tile.
		 */
		void destroy();


		/**
		 * Undestroy for Tile.
		 */
		void undestroy();
	

	public:
		/**
		 * Default constructor. Do not use if you can avoid it!
		 */
		Tile();


		/**
		 * Constructor for Tile class.
		 * 
		 * @param position    Position in (x, y)
		 * @param size        Size in (x, y)
		 * @param texture     Texture to draw the tile with, preferably one of the tile textures.
		 * @param unbreakable If the tile is breakable.
		 * @param colour      OPTIONAL: the overall color of the object.
		 * 
		 * @return Tile object
		 */
		Tile(glm::vec2 position, glm::vec2 size, Texture2D sprite, bool unbreakable, glm::vec3 colour=glm::vec3(1.0f));


		bool get_breakable();
};
