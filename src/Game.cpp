#include "Game.hpp"


Game::Game(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	state = GAME_ACTIVE;
}


Game::~Game() {

}


void Game::init() {

}


void Game::update(float dt) {

}


void Game::process_input(float dt) {

}


void Game::render() {
	
}


void Game::set_state(enum Gamestate state) {
	this->state = state;
}


void Game::press_key(int key) {
	keys[key] = true;
}


void Game::release_key(int key) {
	keys[key] = false;
}
