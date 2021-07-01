#include "Ball_Object.hpp"


Ball_Object::Ball_Object() :
	Game_Object(),
	radius(12.5),
	stuck(true),
	sticky(false),
	pass_through(false) { }


Ball_Object::Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) :
	Game_Object(pos, glm::vec2(radius * 2.0f), sprite, glm::vec3(1.0), velocity),
	radius(radius),
	stuck(true),
	sticky(false),
	pass_through(false) { }


void Ball_Object::destroy() { }


void Ball_Object::undestroy() { }


glm::vec2 Ball_Object::move(float dt, unsigned int window_width) {
	if (!this->stuck) {
		// Move the ball
		glm::vec2 current_pos = this->get_position();
		glm::vec2 current_velocity = this->get_velocity();
		current_pos += current_velocity * dt;

		// Check if position out of bounds. If it is, move to bounds & reverse velocity.
		if (current_pos.x <= 0.0f) {
			current_pos.x = 0.0f;
			current_velocity.x = -current_velocity.x;
		} else if (current_pos.x + this->get_size().x >= window_width) {
			current_pos.x = window_width - this->get_size().x;
			current_velocity.x = -current_velocity.x;
		}

		if (current_pos.y <= 0.0f) {
			current_pos.y = 0.0f;
			current_velocity.y = -current_velocity.y;
		}

		// Set
		this->set_position(current_pos);
		this->set_velocity(current_velocity);
	}

	return this->get_position();
}


void Ball_Object::reset(glm::vec2 position, glm::vec2 velocity) {
	this->set_position(position);
	this->set_velocity(velocity);
	this->stuck = true;
}


bool Ball_Object::is_stuck() {
	return this->stuck;
}


void Ball_Object::set_stuck(bool stuck) {
	this->stuck = stuck;
}


float Ball_Object::get_radius() {
	return this->radius;
}


bool Ball_Object::get_sticky() {
	return this->sticky;
}


void Ball_Object::set_sticky(bool sticky) {
	this->sticky = sticky;
}


bool Ball_Object::get_pass_through() {
	return this->pass_through;
}


void Ball_Object::set_pass_through(bool pass_through) {
	this->pass_through = pass_through;
}
