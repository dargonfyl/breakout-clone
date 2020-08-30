#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Game_Object.hpp"


/**
 * Particle emitter. Kinda like a very specific factory, and the factory objects are non-public facing.
 */
class Particle_Emitter {
	private:
		/**
		 * Private particle object.
		 */
		class Particle {
			public:
				glm::vec2 position, velocity;
				glm::vec4 color;

				float life;  // If the life <= 0.0, then this particle is dead: don't draw it! Otherwise, draw it with intensity proportional to life.

				/**
				 * Default Particle constructor. All we need.
				 */
				Particle();
		};

		std::vector<Particle_Emitter::Particle> particles;
		unsigned int num_particles;
		unsigned int last_used_particle;  // Helps to quickly access next dead particle.

		// TODO: Should this backend drawing stuff be in its own class?
		Shader shader;
		Texture2D texture;
		unsigned int VAO;

		/**
		 * Initialize backend graphics stuff.
		 */
		void init();


		/**
		 * Finds the first unused particle index.
		 */
		unsigned int first_unused_particle();


		void respawn_particle(Particle_Emitter::Particle &particle, Game_Object &object, glm::vec2 offset=glm::vec2(0.0f));
	public:
		/**
		 * Constructor for Particle_Emitter object.
		 * 
		 * @param shader        Shader program to use.
		 * @param texture       Texture of particle.
		 * @param num_particles Number of particles to draw for every drawcycle
		 * 
		 * @return Particle_Emitter object.
		 */
		Particle_Emitter(Shader shader, Texture2D texture, unsigned int num_particles);


		/**
		 * Update for a render frame.
		 */
		void update(float dt, Game_Object &object, unsigned int num_new_particles, glm::vec2 offset=glm::vec2(0.0f));


		/**
		 * Draw method for a Particle_Emitter.
		 */
		void draw();
};
