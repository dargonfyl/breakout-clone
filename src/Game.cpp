#include "Game.hpp"


Game::Game(unsigned int width, unsigned int height) {
	assert(width > 0 && "width == 0 in Game constructor");
	assert(height > 0 && "height == 0 in Game constructor");

	this->width = width;
	this->height = height;
	state = GAME_ACTIVE;
}


Game::~Game() { }


Sprite_Renderer *renderer;

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
Ball_Object *ball; 


void Game::init() {
	// Load shaders
	Resource_Manager::load_shader("../shaders/quad.vs", "../shaders/quad.fs", nullptr, "quad");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

	Shader quad_shader = Resource_Manager::get_shader("quad");
	quad_shader.use().set_int("image", 0);
	quad_shader.use();
	quad_shader.set_mat4("projection", projection);

	renderer = new Sprite_Renderer(quad_shader);

	Resource_Manager::load_texture("../data/block_tile.png", false, "block_tile");
	Resource_Manager::load_texture("../data/solid_tile.png", false, "solid_tile");

	Resource_Manager::load_texture("../data/awesomeface.png", true, "face");
	Resource_Manager::load_texture("../data/background.jpg", false, "background");

	Resource_Manager::load_texture("../data/paddle.png", true, "paddle");

	// Player resources
	glm::vec2 player_position = glm::vec2(
		this->width / 2.0f - PLAYER_SIZE.x / 2.0f, 
		this->height - PLAYER_SIZE.y
    );

	this->player = new Game_Object(player_position, PLAYER_SIZE, Resource_Manager::get_texture("paddle"));

	// Levels
	Game_Level one("../data/levels/one.lvl", this->width, this->height / 2);
	Game_Level two("../data/levels/two.lvl", this->width, this->height / 2);
	Game_Level three("../data/levels/three.lvl", this->width, this->height / 2);
	Game_Level four("../data/levels/four.lvl", this->width, this->height / 2);

	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->current_level = 0;  // First level

	// Ball stuff
	glm::vec2 ball_position = player_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	ball = new Ball_Object(ball_position, BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_Manager::get_texture("face"));
}

/**
 * Helper for checking collisions between the ball and tiles.
 * 
 * Very quick & dirty explanation of what this does:
 * 
 * Given a ball and a tile, we want to find the points on the tile that are closest to the ball. ("clamped")
 * Afterwards, if clamped and the ball's center are less than a radius apart, then we've intersected.
 */
bool check_collision(Ball_Object &o1, Game_Object &o2) {
	glm::vec2 ball_center = o1.get_position() + o1.get_radius();

	// AABB for the tile
	glm::vec2 aabb_half_extents = glm::vec2(0.5) * o2.get_size();
	glm::vec2 aabb_center = o2.get_position() + aabb_half_extents;

	// Get difference between the two cobjects' centers
	glm::vec2 difference = ball_center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// Find closest point to ball
	glm::vec2 closest = aabb_center + clamped;

	difference = closest - ball_center;

	return glm::length(difference) < o1.get_radius();
}


void Game::check_collisions() {
	 for (Game_Object &tile : this->levels[this->current_level].get_bricks()) {
		 if (!tile.get_destroyed()) {
			if (check_collision(*ball, tile) && tile.is_solid()) tile.destory_object();
		 }
	 }
}


void Game::update(float dt) {
	ball->move(dt, this->width);
	this->check_collisions();
}


void Game::process_input(float dt) {
	if (this->state == GAME_ACTIVE) {
		float velocity = dt * PLAYER_VELOCITY;  // TODO: this is a retarded way of doing this. Make it a vec2, ffs.

		// Keyboard movements
		glm::vec2 pos = player->get_position();
		glm::vec2 ball_pos = ball->get_position();
		if (this->keys[GLFW_KEY_A]) {  // TODO: this is kinda dumb, probably can be cleaned up.
			if (pos.x >= 0.0f) {
				player->set_position(glm::vec2(pos.x - velocity, pos.y));

				if (ball->is_stuck()) {
					ball->set_position(glm::vec2(ball_pos.x - velocity, ball_pos.y));
				}
			}
		} else if (this->keys[GLFW_KEY_D]) {
			if (pos.x <= this->width - player->get_size().x) {
				player->set_position(glm::vec2(pos.x + velocity, pos.y));

				if (ball->is_stuck()) {
					ball->set_position(glm::vec2(ball_pos.x + velocity, ball_pos.y));
				}
			}
		}

		if (this->keys[GLFW_KEY_SPACE]) {
			ball->set_stuck(false);
		}
	}
}


void Game::render() {
	if(this->state == GAME_ACTIVE) {
		Texture2D bg_tex = Resource_Manager::get_texture("background");
		renderer->draw_sprite(bg_tex, glm::vec2(0.0f), glm::vec2(this->width, this->height), 0.0f);

		this->levels[current_level].draw(*renderer);
		this->player->draw(*renderer);

		ball->draw(*renderer);
	}
}


void Game::set_state(enum Gamestate state) {
	this->state = state;
}


void Game::press_key(int key) {
	assert(key >= 0 && key < 1024 && "key in Game::press_key is not in bounds");
	keys[key] = true;
}


void Game::release_key(int key) {
	assert(key >= 0 && key < 1024 && "key in Game::release_key is not in bounds");
	keys[key] = false;
}
