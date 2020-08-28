#include <Tile.hpp>
#include <iostream>


Tile::Tile() { }


Tile::Tile(glm::vec2 position, glm::vec2 size, Texture2D sprite, bool breakable, glm::vec3 colour) :
	Game_Object(position, size, sprite, colour),
	breakable(breakable) { }


void Tile::destroy() {
	std::cout << "destroyed " << this->get_id() << std::endl; 
}


void Tile::undestroy() { }


bool Tile::get_breakable() {
	return this->breakable;
}
