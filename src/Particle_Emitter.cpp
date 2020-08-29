#include "Particle_Emitter.hpp"


Particle_Emitter::Particle::Particle() :
	position(0.0f),
	velocity(0.0f),
	color(1.0f),
	life(0.0f) { }


Particle_Emitter::Particle_Emitter(Shader shader, Texture2D texture, unsigned int num_particles) :
	shader(shader),
	texture(texture),
	num_particles(num_particles) {
	this->init();
	last_used_particle = 0;
}


void Particle_Emitter::init() {
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// this->particles = std::vector<Particle_Emitter::Particle>(this->num_particles, Particle_Emitter::Particle());
	for (unsigned int i = 0; i < this->num_particles; ++i)
        this->particles.push_back(Particle());
}


unsigned int Particle_Emitter::first_unused_particle() {
	for (unsigned int i = this->last_used_particle; i < this->num_particles; i++) {  // heuristic to find it faster.
		if (this->particles[i].life <= 0.0f) {
			this->last_used_particle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < this->last_used_particle; i++) {
		if (this->particles[i].life <= 0.0f) {
			this->last_used_particle = i;
			return i;
		}
	}
	// If we reach this part, then all particles are being used. Thus, we can just override the first one.
	this->last_used_particle = 0;
	return 0;
}


void Particle_Emitter::respawn_particle(Particle_Emitter::Particle &particle, Game_Object &object, glm::vec2 offset) {
	float random = ((rand() % 100) - 50) / 10.0f;
    float rand_color = 0.5f + ((rand() % 100) / 100.0f);
    particle.position = object.get_position() + random + offset;
    particle.color = glm::vec4(rand_color, rand_color, rand_color, 1.0f);
	particle.velocity = object.get_velocity() * 0.1f;

	/*
	A note before we go any further.
	In the LearnOpenGL tutorial, the following (adjusted) line of code looks like this:

	particle.life = 1.0f;

	IDK why, but that just doesn't work; it draws these really strange hexagons. This is the best workaround I could find.
	*/
    particle.life = (rand() % 100) / 100.0f;
}


void Particle_Emitter::update(float dt, Game_Object &object, unsigned int num_new_particles, glm::vec2 offset) {
	for (unsigned int i = 0; i < num_particles; i++) {
		unsigned int unused = this->first_unused_particle();
		this->respawn_particle(this->particles[unused], object, offset);
	}

	// Update all particles
	for (unsigned int i = 0; i < this->num_particles; i++) {
		Particle_Emitter::Particle &p = this->particles[i];
		p.life -= dt;
		
		if (p.life > 0.0f) {
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5f;
		}
	}
}


void Particle_Emitter::draw() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL blend function
	this->shader.use();
	for (Particle_Emitter::Particle &particle : this->particles) {
		if (particle.life > 0.0f) {
			this->shader.set_vec2("offset", particle.position);
			this->shader.set_vec4("color", particle.color);
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	std::cout << particles.size() << std::endl;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
