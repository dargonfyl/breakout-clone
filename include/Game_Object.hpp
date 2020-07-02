#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <math.h>
#include <assert.h>

#include "Texture.hpp"
#include "Sprite_Renderer.hpp"


/**
 * Container for the states of a game object.
 * 
 * Every game object needs at least most of the members described here.
 * 
 * NOTE: This is an abstract class! c++ doesn't have a nice way to declare that, which is annoying.
 * 
 * @abstract: update() and draw() are (non-pure) virtual.
 * 
 */
class Game_Object {
	private:
		// Object state members
		glm::vec2 position, size, velocity;
		glm::vec3 colour;

		float rotation;  // In degrees. This is in the range [0, 360)
		bool solid, destroyed;

		// Render state
		Texture2D sprite;


		/**
		 * Call this to do any behaviour that needs to happen upon destroy.
		 */
		void destroy();


	public:
		/**
		 * Constructor for Game_Object.
		 * 
		 * Default constructor; this will set everything to 0.
		 */
		Game_Object();


		/**
		 * Constructor for Game_Object
		 * 
		 * @param glm::vec2 position: position
		 * @param glm::vec2 size
		 * @param Texture2D sprite
		 * @param OPTIONAL glm::vec3 colour: defaults to white (vec3(1.0f))
		 * @param OPTIONAL glm::vec2 velocity: defaults to vec2(0.0f)
		 */
		Game_Object(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 colour=glm::vec3(1.0f), glm::vec2 velocity=glm::vec2(0.0f));


		/**
		 * Updates this object for a frame.
		 * 
		 * @virtual
		 */
		virtual void update();


		/**
		 * Draw the sprite associated with this object.
		 * 
		 * @param SpriteRenderer &renderer: sprite renderer to use.
		 */
		virtual void draw(Sprite_Renderer &renderer);


		/**
		 * Gets position.
		 * 
		 * @return glm::vec2: position in pixels
		 */
		glm::vec2 get_position();

		
		/**
		 * Sets the position of this object, in pixels.
		 * 
		 * @param glm::vec2 position
		 */
		void set_position(glm::vec2 position);


		/**
		 * Gets the colour of this object.
		 * 
		 * @return glm::vec3: colour in rgb tuple.
		 */
		glm::vec3 get_colour();


		/**
		 * Sets the colour of this object.
		 *
		 * @param glm::vec3 colour
		 */
		void set_colour(glm::vec3 colour);


		/**
		 * Gets the velocity of this object.
		 * 
		 * @return glm::vec2: velocity in pixels/tick
		 */
		glm::vec2 get_velocity();
		

		/**
		 * Sets the velocity of this object.
		 * 
		 * @param glm::vec2 velocity: velocity, in pixels/tick
		 */
		void set_velocity(glm::vec2 velocity);


		/**
		 * Gets the size of this object.
		 * 
		 * @return glm::vec2: size in pixels, (width, height) format
		 */
		glm::vec2 get_size();


		/**
		 * Sets the size of this object.
		 * 
		 * @param glm::vec2 size: size in pixels, (width, height) format
		 */
		void set_size(glm::vec2 size);


		/**
		 * Gets the rotation in degrees of this object.
		 * 
		 * @return float: angle in degrees, in range [0, 360)
		 */
		float get_rotation();


		/**
		 * Rotates this object by an amount in degrees.
		 * Note: this rotates the object, and does not set the angle. To do that, use set_rotation.
		 * 
		 * @param float angle: angle to rotate. Note that the final representation will be in [0, 360)
		 */
		void rotate(float angle);


		/**
		 * Sets the rotation of this object.
		 * 
		 * @param float angle: angle in degrees, in the range [0, 360)
		 */
		void set_rotation(float angle);


		/**
		 * Returns is solid.
		 * 
		 * @return bool: solidity
		 */
		bool is_solid();


		/**
		 * Sets the solidity (is that a word?) of this object, and returns the previous state.
		 * 
		 * @param bool solidity: is solid
		 * 
		 * @return bool: previous state
		 */
		bool set_solid(bool solidity);


		/**
		 * Gets whether this object is destroyed or not.
		 * 
		 * @return bool: destroyed
		 */
		bool get_destroyed();


		/**
		 * Destroys this object. Sets destroyed to true. Will also call the private destroy().
		 */
		void destory_object();


		/**
		 * Gets the sprite associated with this object.
		 * 
		 * @return Texture2D: sprite
		 */
		Texture2D get_sprite();


};
