#include "Game_Object.hpp"


unsigned int num_objects = 0;

Game_Object::Game_Object() {
	position = glm::vec2(0.0f);
	size = glm::vec2(0.0f);
	velocity = glm::vec2(0.0f);
	colour = glm::vec3(0.0f);

	rotation = 0.0f;
	solid = true;
	destroyed = false;

	this->id = num_objects;
	num_objects++;
}


Game_Object::Game_Object(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 colour, glm::vec2 velocity) {
	this->position = position;
	this->size = size;
	this->sprite = sprite;
	this->colour = colour;
	this->velocity = velocity;

	this->solid = true;
	this->destroyed = false;
	this->rotation = 0.0f;
	this->id = num_objects;
	num_objects++;
}


void Game_Object::update() { }


void Game_Object::draw(Sprite_Renderer &renderer) {
	renderer.draw_sprite(this->sprite, this->position, this->size, this->rotation, this->colour);
}


unsigned int Game_Object::get_id() {
	return this->id;
}


glm::vec2 Game_Object::get_position() {
	return this->position;
}


void Game_Object::set_position(glm::vec2 position) {
	this->position = position;
}


glm::vec3 Game_Object::get_colour() {
	return this->colour;
}


void Game_Object::set_colour(glm::vec3 colour) {
	this->colour = colour;
}


glm::vec2 Game_Object::get_velocity() {
	return this->velocity;
}


void Game_Object::set_velocity(glm::vec2 velocity) {
	this->velocity = velocity;
}


glm::vec2 Game_Object::get_size() {
	return this->size;
}


void Game_Object::set_size(glm::vec2 size) {
	this->size = size;
}


float Game_Object::get_rotation() {
	return this->rotation;
}


void Game_Object::rotate(float angle) {
	this->rotation = fmod(this->rotation + angle, 360.0f);
}


void Game_Object::set_rotation(float angle) {
	assert(angle >= 0.0 && angle < 360.0 && "angle is not in [0, 360)");
	this->rotation = angle;
}


bool Game_Object::is_solid() {
	return this->solid;
}


bool Game_Object::set_solid(bool solidity) {
	bool previous = this->solid;
	this->solid = solidity;
	return previous;
}


bool Game_Object::get_destroyed() {
	return this->destroyed;
}


void Game_Object::destory_object() {
	this->destroyed = true;
}


Texture2D Game_Object::get_sprite() {
	return this->sprite;
}

