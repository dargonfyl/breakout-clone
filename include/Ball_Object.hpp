#pragma once

#include <glm/glm.hpp>

#include "Game_Object.hpp"


class Ball_Object : public Game_Object {
	private:
		float radius;  // Ball radius.
		bool stuck;  // If true, the ball will not move. I.e. is true at the start of a stage.
		bool sticky;
		bool pass_through;

		/**
		 * Destroy for the Ball_Object class.
		 */
		void destroy();


		/**
		 * Undestroy for the Ball_Object class.
		 */
		void undestroy();


	public:
		/**
		 * Default constructor. Do not use if you can avoid it!
		 */
		Ball_Object();

		/**
		 * Constructor. The ball will have an "initial" velocity.
		 * 
		 * @param pos      Ball position in (x, y)
		 * @param radius   Ball radius
		 * @param velocity Initial velocity.
		 * @param sprite   Ball sprite
		 * 
		 * @return `Ball_Object`
		 */
		Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);


		/**
		 * Moves the ball by a certain amount.
		 * 
		 * @param dt           Time difference (delta time).
		 * @param window_width Width of the window.
		 * 
		 * @return The resulting position of the ball, for checking if stuck.
		 */
		glm::vec2 move(float dt, unsigned int window_width);


		/**
		 * Resets the ball to a position & a velocity.
		 * 
		 * @param position Position ball is to be in
		 * @param velocity Velocity ball should have after released.
		 */
		void reset(glm::vec2 position, glm::vec2 velocity);

		/**
		 * Returns if stuck
		 * 
		 * @return bool If stuck
		 */
		bool is_stuck();

		
		/**
		 * Sets the ball to stuck or not stuck.
		 * 
		 * @param stuck If stuck
		 */
		void set_stuck(bool stuck);


		/**
		 * Returns ball radius.
		 * 
		 * @return float Radius of the ball
		 */
		float get_radius();


		bool get_sticky();


		void set_sticky(bool sticky);


		void set_pass_through(bool pass_through);


		bool get_pass_through();
};
