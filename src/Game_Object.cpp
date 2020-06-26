#include "Game_Object.hpp"


Game_Object::Game_Object() {
	position = glm::vec2(0.0f);
	size = glm::vec2(0.0f);
	velocity = glm::vec2(0.0f);
	colour = glm::vec3(0.0f);

	float rotation = 0.0f;
	solid = false;
	destroyed = true;
}


Game_Object::Game_Object(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 colour, glm::vec2 velocity) {
	this->position = position;
	this->size = size;
	this->sprite = sprite;
	this->colour = colour;
	this->velocity = velocity;
}



