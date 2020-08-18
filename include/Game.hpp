#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <vector>

#include "Sprite_Renderer.hpp"
#include "Resource_Manager.hpp"
#include "Game_Level.hpp"
#include "Ball_Object.hpp"

#define PLAYER_SIZE glm::vec2(100.0f, 20.0f)
#define PLAYER_VELOCITY 500.0f


enum Gamestate {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


/**
 * Main Game object.
 */
class Game {
	private:
		enum Gamestate state;


		// Tracks whether a key is pressed or not; true for pressed, false for released/unpressed
		bool keys[1024];


		unsigned int width, height;


		// Level things.
		std::vector<Game_Level> levels;
		int current_level;  // -1 for no current level


		Game_Object *player;


	public:
		/**
		 * Constructor for Game class. Note that there should only be 1 of these.
		 * 
		 * @param width
		 * @param height
		 */
		Game(unsigned int width, unsigned int height);


		/**
		 * Game deconstructor.
		 * 
		 */
		~Game();

		/**
		 * Initializes the game and its non-constructor members 
		 */
		void init();


		// Game loop methods
		/**
		 * Processes user input.
		 * @param dt time interval
		 */
		void process_input(float dt);


		/**
		 * Updates the game state, information, etc.
		 * @param dt time interval
		 */
		void update(float dt);


		/**
		 * Renders the objects of the scene
		 */
		void render();


		/**
		 * Set the state of the Game.
		 */
		void set_state(enum Gamestate state);


		/**
		 * Marks a key as pressed.
		 * 
		 * @param key Key by GLFW key id
		 */
		void press_key(int key);


		/**
		 * Marks a key as released/not pressed.
		 * 
		 * @param key Key by GLFW key id
		 */
		void release_key(int key);

};
