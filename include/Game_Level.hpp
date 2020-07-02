#pragma once

#include <vector>
#include <assert.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Game_Object.hpp"
#include "Sprite_Renderer.hpp"
#include "Resource_Manager.hpp"


enum Block_Type {
	EMPTY_TILE = 0,
	SOLID_TILE = 1,
	BLUE_TILE = 2,
	GREEN_TILE = 3,
	YELLOW_TILE = 4,
	RED_TILE = 5,
};


class Game_Level {
	private:
		std::vector<Game_Object> bricks;


		/**
		 * Initialize for a Game_Level.
		 * 
		 * @param std::vector<std::vector<unsigned int>> tile_data: 2D tile data
		 * @param unsigned int level_width: width of level in pixels
		 * @param unsigned int level_height: height of level in pixels
		 */
		void init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);


		


	public:
		/**
		 * Default constructor. Do not use this if you can avoid it!
		 */
		Game_Level();


		/**
		 * Constructor; loads a game level from a file.
		 * 
		 * @param const char *file: filename of a level, in the appropriate level format.
		 * @param unsigned int level_width: width of level in pixels
		 * @param unsigned int level_height: height of level in pixels
		 */
		Game_Level(const char *file, unsigned int level_width, unsigned int level_height);


		/**
		 * Loads a game level data from a file.
		 * 
		 * @param const char *file: filename of a level, in the appropriate level format.
		 * @param unsigned int level_width: width of level in pixels
		 * @param unsigned int level_height: height of level in pixels
		 */
		void load(const char *file, unsigned int level_width, unsigned int level_height);


		/**
		 * Draws the bricks of the level.
		 * 
		 * @param Sprite_Renderer &renderer: sprite renderer to use.
		 */
		void draw(Sprite_Renderer &renderer);


		/**
		 * Returns true if the level is completed, i.e. if all bricks are destroyed.
		 * 
		 * @return bool
		 */
		bool is_completed();


};
