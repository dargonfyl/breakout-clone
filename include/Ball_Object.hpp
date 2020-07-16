#pragma once

#include <glm/glm.hpp>

#include "Game_Object.hpp"


class Ball_Object : public Game_Object {
	private:
		float radius;  // Ball radius.
		bool stuck;  // If true, the ball will not move. I.e. is true at the start of a stage.
	

	public:
		/**
		 * Default constructor. Do not use if you can avoid it!
		 */
		Ball_Object();

		/**
		 * Constructor. The ball will have an "initial" velocity.
		 * 
		 * @param glm::vec2 pos
		 * @param float radius
		 * @param glm::vec2 velocity: initial velocity.
		 * @param Texture2D sprite
		 * 
		 * @return Ball_Object
		 */
		Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);


		/**
		 * Moves the ball by a certain amount.
		 */
		glm::vec2 move(float dt, unsigned int window_width);


		/**
		 * Resets the ball to a position & a velocity.
		 * 
		 * @param glm::vec2 position: position ball is to be in
		 * @param glm::vec2 velocity: velocity ball should have after released.
		 */
		void reset(glm::vec2 position, glm::vec2 velocity);

		/**
		 * Returns if stuck
		 * 
		 * @return bool
		 */
		bool get_stuck();


		/**
		 * Returns ball radius.
		 * 
		 * @return float
		 */
		float get_radius();
};
