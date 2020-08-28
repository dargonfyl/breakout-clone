#pragma once

#include <vector>
#include <assert.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Tile.hpp"
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
		std::vector<Tile> bricks;


		/**
		 * Initialize for a Game_Level.
		 * 
		 * @param tiledata     2D tile data
		 * @param level_width  width of level in pixels
		 * @param level_height height of level in pixels
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
		 * @param file         Filename of a level, in the appropriate level format.
		 * @param level_width  Width of level in pixels
		 * @param level_height Height of level in pixels
		 */
		Game_Level(const char *file, unsigned int level_width, unsigned int level_height);


		/**
		 * Loads a game level data from a file.
		 * 
		 * @param file         Filename of a level, in the appropriate level format.
		 * @param level_width  Width of level in pixels
		 * @param level_height Height of level in pixels
		 */
		void load(const char *file, unsigned int level_width, unsigned int level_height);  // TODO: why is this public?


		/**
		 * Draws the bricks of the level.
		 * 
		 * @param renderer Sprite renderer to use.
		 */
		void draw(Sprite_Renderer &renderer);


		/**
		 * Returns true if the level is completed, i.e. if all bricks are destroyed.
		 * 
		 * @return bool True if completed.
		 */
		bool is_completed();


		/**
		 * Gets a reference to the bricks vector. Note that std::vector is usually passed by value, so & passes it by reference.
		 * 
		 * @return `std::vector` of `Game_Object`, the bricks
		 */
		std::vector<Tile> &get_bricks();
};
