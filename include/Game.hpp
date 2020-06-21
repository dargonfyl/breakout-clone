/**
 * Main Game object.
 */

#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>


enum Gamestate {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class Game {
	private:
		enum Gamestate state;

		bool keys[1024];

		unsigned int width, height;

	public:
		Game(unsigned int width, unsigned int height);
		~Game();

		/**
		 * Initializes the game and its non-constructor members 
		 */
		void init();

		// Game loop methods
		/**
		 * Processes user input.
		 * @param float dt: time interval
		 */
		void process_input(float dt);

		/**
		 * Updates the game state, information, etc.
		 * @param float dt: time interval
		 */
		void update(float dt);
		
		/**
		 * Renders the objects of the scene
		 */
		void render();

};
