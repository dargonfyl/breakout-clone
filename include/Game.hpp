#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>

#include "Sprite_Renderer.hpp"
#include "Resource_Manager.hpp"


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


	public:
		/**
		 * Constructor for Game class. Note that there should only be 1 of these.
		 * 
		 * @param unsigned int width
		 * @param unsigned int height
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


		/**
		 * Set the state of the Game.
		 */
		void set_state(enum Gamestate state);


		/**
		 * Marks a key as pressed.
		 * 
		 * @param int key: key by id
		 */
		void press_key(int key);


		/**
		 * Marks a key as released/not pressed.
		 * 
		 * @param int key: key by id
		 */
		void release_key(int key);

};
