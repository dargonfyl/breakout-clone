#include "Game.hpp"

#include <random>


namespace
{
	// RANDOM
	/**
	 * Bernoulli trial with one-in-X chance.
	 */
	bool bernoulli_one_in_X(unsigned int chance) {
		unsigned int threshold = rand() % chance;
		return threshold == 0;
	}

	/**
	 * Bernoulli trial with unsigned integer X% chance.
	 */
	bool bernoulli_X_percent(unsigned int chance) {
		unsigned int threshold = rand() % 100 + 1; // this is in [1, 100]
		return chance >= threshold;
	}

} // namespace


Game::Game(unsigned int width, unsigned int height) {
	assert(width > 0 && "width == 0 in Game constructor");
	assert(height > 0 && "height == 0 in Game constructor");

	this->width = width;
	this->height = height;
	state = GAME_ACTIVE;
}


Game::~Game() { }


Sprite_Renderer *renderer;
Postprocessor *postprocessor;

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
Ball_Object *ball;
Particle_Emitter *emitter;


void Game::init() {
	// Load shaders

	Resource_Manager::load_shader("../shaders/quad.vs", "../shaders/quad.fs", nullptr, "quad");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

	Shader &quad_shader = Resource_Manager::get_shader("quad");
	quad_shader.use().set_int("image", 0);
	quad_shader.use();
	quad_shader.set_mat4("projection", projection);

	renderer = new Sprite_Renderer(quad_shader);

	Resource_Manager::load_shader("../shaders/postprocess.vs", "../shaders/postprocess.fs", nullptr, "postprocessor");
	Shader &postprocessor_shader = Resource_Manager::get_shader("postprocessor");
	postprocessor = new Postprocessor(postprocessor_shader, this->width, this->height);

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

	this->player = new Game_Object(player_position, PLAYER_SIZE, Resource_Manager::get_texture("paddle"));  // TODO: create paddle class.

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

	// Ball
	glm::vec2 ball_position = player_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	ball = new Ball_Object(ball_position, BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_Manager::get_texture("face"));

	// Particle emitter
	Resource_Manager::load_shader("../shaders/particle.vs", "../shaders/particle.fs", nullptr, "particle");
	Resource_Manager::load_texture("../data/particle.png", true, "particle");
	Resource_Manager::get_shader("particle").use().set_mat4("projection", projection);
	emitter = new Particle_Emitter(Resource_Manager::get_shader("particle"), Resource_Manager::get_texture("particle"), 100);

	// Power ups
	Resource_Manager::load_texture("../data/speed.png", true, "speed");
	Resource_Manager::load_texture("../data/sticky.png", true, "sticky");
	Resource_Manager::load_texture("../data/pass.png", true, "pass");
	Resource_Manager::load_texture("../data/pad-size-up.png", true, "pad-size-up");
	Resource_Manager::load_texture("../data/chaos.png", true, "chaos");
	Resource_Manager::load_texture("../data/confuse.png", true, "confuse");
}


void Game::spawn_power_up(Game_Object &block) {
	glm::vec2 pos = block.get_position();
	if (bernoulli_one_in_X(75))
		power_ups.push_back(Power_Up(Speed, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, pos, Resource_Manager::get_texture("speed")));
	if (bernoulli_one_in_X(75))
		power_ups.push_back(Power_Up(Sticky, glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, pos, Resource_Manager::get_texture("sticky")));
	if (bernoulli_one_in_X(75))
		power_ups.push_back(Power_Up(Pass_Through, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, pos, Resource_Manager::get_texture("pass")));
	if (bernoulli_one_in_X(75))
		power_ups.push_back(Power_Up(Paddle_Size_Up, glm::vec3(1.0f, 0.6f, 0.4), 0.0f, pos, Resource_Manager::get_texture("pad-size-up")));
	if (bernoulli_one_in_X(15))
		power_ups.push_back(Power_Up(Confuse, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, pos, Resource_Manager::get_texture("confuse")));
	if (bernoulli_one_in_X(15))
		power_ups.push_back(Power_Up(Chaos, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, pos, Resource_Manager::get_texture("chaos")));
}


bool other_power_ups_active(std::vector<Power_Up> &power_ups, Power_Up_Type type) {
	for (Power_Up &power_up : power_ups) {
		if (power_up.is_active() && power_up.get_type() == type) {
			return true;
		}
	}
	return false;
}

void Game::update_power_ups(float dt) {
	for (Power_Up &power_up : power_ups) {
		glm::vec2 new_pos = power_up.get_position() + power_up.get_velocity() * dt;
		power_up.set_position(new_pos);

		if (power_up.is_active()) {
			power_up.set_duration(power_up.get_duration() - dt);

			// Kill powerup
			if (power_up.get_duration() <= 0.0f) {
				power_up.set_active(false);

				// In case there are multiples of a power-up active
				if (other_power_ups_active(power_ups, power_up.get_type())) {
					continue;
				}
				switch (power_up.get_type())
				{
				case (Sticky):
					ball->set_sticky(false);
					ball->set_colour(glm::vec3(1.0));
					break;
				case (Pass_Through):
					ball->set_pass_through(false);
					ball->set_colour(glm::vec3(1.0));
					break;
				case (Confuse):
					postprocessor->confuse = false;
					break;
				case (Chaos):
					postprocessor->chaos = false;
				}
			}
		}
	}

	power_ups.erase(std::remove_if(power_ups.begin(), power_ups.end(), [](Power_Up &power_up) {
		return !power_up.is_active() && power_up.get_destroyed();
	}), power_ups.end());
}


/**
 * Helper to find best direction to flip for a collision - goes through the 4 cardinal directions to see which one the incoming vector is closest to. If there is no best direction, then the best one will be the up direction.
 *
 * @param incoming glm::vec2 direction of the incoming object's velocity.
 *
 * @return Direction enum of which direction the incoming object best matches.
 */
Direction vector_direction(glm::vec2 incoming) {
	// assert(incoming.x > 0.0f && incoming.y > 0.0f && "Game.cpp: vector_direction: incoming vector must be nonzero");
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
    };

	float max = 0.0f;
	// HACK: this is not the best way of doing this, but it at least prevents a crash when the ball is in the paddle.
	unsigned int best_match = 0; // up by default. Probably a dumb idea!
	for (unsigned int i = 0; i < 4; i++) {
		float dot = glm::dot(glm::normalize(incoming), compass[i]);

		if (dot > max) {
			max = dot;
			best_match = i;
		}
	}

	assert(best_match >= 0 && best_match < 4 && "Game.cpp: vector_direction: best_match not a Direction enum");
	return (Direction)best_match;
}


/**
 * Helper for checking collisions between the ball and tiles.
 *
 * Very quick & dirty explanation of what this does:
 *
 * Given a ball and a tile, we want to find the points on the tile that are closest to the ball. ("clamped")
 * Afterwards, if clamped and the ball's center are less than a radius apart, then we've intersected.
 */
Collision check_collision(Ball_Object &o1, Game_Object &o2) {
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

	// return glm::length(difference) < o1.get_radius();
	if (glm::length(difference) <= o1.get_radius()) return std::make_tuple(true, vector_direction(difference), difference);
	else return std::make_tuple(false, UP, glm::vec2(0.0f));
}


/**
 * Checks collision between game objects in general.
 */
bool check_collision(Game_Object &o1, Game_Object &o2) {
	glm::vec2 p1 = o1.get_position();
	glm::vec2 size1 = o1.get_size();
	glm::vec2 p2 = o2.get_position();
	glm::vec2 size2 = o2.get_size();

	bool x_collide = p1.x + size1.x >= p2.x && p2.x + size2.x >= p1.x;
	bool y_collide = p1.y + size1.y >= p2.y && p2.y + size2.y >= p1.y;

	return x_collide && y_collide;
}

float shake_time = 0.0f;

void Game::check_collisions() {
	// Check for non-destroyed tiles
	for (Tile &tile : this->levels[this->current_level].get_bricks()) {
		if (!tile.get_destroyed()) {
			Collision collision = check_collision(*ball, tile);
			if (std::get<0>(collision)) {
				if (tile.get_breakable()) {
					tile.destory_object();  // Only for solid objects
					this->spawn_power_up(tile);
				} else {
					shake_time = 0.05f;
					postprocessor->shake = true;
				}

				// If pass through is active and the tile is breakable, do not deflect.
				if (!ball->get_pass_through() || !tile.get_breakable()) {
					Direction dir = std::get<1>(collision);
					glm::vec2 diff_vector = std::get<2>(collision);
					glm::vec2 ball_velocity = ball->get_velocity();

					if (dir == LEFT || dir == RIGHT) {
						ball->set_velocity(glm::vec2(-ball_velocity.x, ball_velocity.y));

						// Relocate
						float penetration = ball->get_radius() - std::abs(diff_vector.x);
						float offset = dir == LEFT ? penetration : -penetration;
						float new_ball_x = ball->get_position().x + offset;
						ball->set_position(glm::vec2(new_ball_x, ball->get_position().y));
					} else if (dir == UP || dir == DOWN) {
						ball->set_velocity(glm::vec2(ball_velocity.x, -ball_velocity.y));

						// Relocate
						float penetration = ball->get_radius() - std::abs(diff_vector.y);
						float offset = dir == DOWN ? penetration : -penetration;
						float new_ball_y = ball->get_position().y + offset;
						ball->set_position(glm::vec2(ball->get_position().x, new_ball_y));
					}
				}
			}
		}
	}

	// Check for the paddle (the player)
	Collision paddle_collide = check_collision(*ball, *player);
	if (!ball->is_stuck() && std::get<0>(paddle_collide)) {
		// check where it hit the board, and change velocity based on where it hit the board
		float paddle_center = player->get_position().x + player->get_size().x / 2.0f;
		float distance = ball->get_position().x + ball->get_radius() - paddle_center;
		float percentage = distance / (player->get_size().x / 2.0f);  // Proportionally, how "horizontal" the ball is

		// Move according to how "horizontal" the ball is
		glm::vec2 old_velocity = ball->get_velocity();
		float new_velocity_x = INITIAL_BALL_VELOCITY.x * percentage * 2.0f;
		float new_velocity_y = -abs(ball->get_velocity().y);
		ball->set_velocity(glm::vec2(new_velocity_x, new_velocity_y));

		ball->set_velocity(glm::normalize(ball->get_velocity()) * glm::length(old_velocity));  // Preserve final speed of the ball

		ball->set_stuck(ball->get_sticky());
	}

	for (Power_Up &power_up : this->power_ups) {
		if (!power_up.get_destroyed()) {
			if (power_up.get_position().y >= this->height) {
				power_up.destory_object();
			}

			if (check_collision(*player, power_up)) {
				this->activate_power_up(power_up);
				power_up.destory_object();
				power_up.set_active(true);
			}
		}
	}
}


void Game::activate_power_up(Power_Up &power_up) {
	switch (power_up.get_type())
	{
	case (Speed):
		ball->set_velocity(1.2f * ball->get_velocity());
		break;
	case (Sticky):
		ball->set_sticky(true);
		player->set_colour(glm::vec3(1.0f, 0.5f, 1.0f));
		break;
	case (Pass_Through):
		ball->set_pass_through(true);
		ball->set_colour(glm::vec3(1.0f, 0.5f, 0.5f));
		break;
	case (Paddle_Size_Up):
		glm::vec2 size = player->get_size();
		size.x += 50;
		player->set_size(size);
		break;
	case (Confuse):
		if (!postprocessor->chaos) {
			postprocessor->confuse = true;
		}
		break;
	case (Chaos):
		if (!postprocessor->confuse) {
			postprocessor->chaos = true;
		}
		break;
	default:
		break;
	}
}


void Game::reset_level() {
	for (Tile &tile : this->levels[this->current_level].get_bricks()) {
		tile.undestroy_object();
	}
}


void Game::reset_player() {
	glm::vec2 initial_paddle_position = glm::vec2(
		this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->height - PLAYER_SIZE.y
    );

	player->set_position(initial_paddle_position);
	glm::vec2 initial_ball_position = initial_paddle_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	player->set_size(PLAYER_SIZE);
	player->set_colour(glm::vec3(1.0f));

	ball->set_position(initial_ball_position);
	ball->set_stuck(true);
	ball->set_sticky(false);
	ball->set_pass_through(false);
	ball->set_colour(glm::vec3(1.0f));
	ball->set_velocity(INITIAL_BALL_VELOCITY);

	postprocessor->confuse = false;
	postprocessor->chaos = false;

	power_ups.clear();
}



void Game::update(float dt) {
	ball->move(dt, this->width);
	this->check_collisions();

	if (ball->get_position().y >= this->height) {
		this->reset_level();
		this->reset_player();
	}

	if (shake_time > 0.0f) {
		shake_time -= dt;
		if (shake_time <= 0.0f) {
			postprocessor->shake = false;
		}
	}
	update_power_ups(dt);
	emitter->update(dt, *ball, 10, glm::vec2(ball->get_radius() / 2.0f));
}


void Game::process_input(float dt) {
	if (this->state == GAME_ACTIVE) {
		if (this->keys[GLFW_KEY_A] != this->keys[GLFW_KEY_D]) {  // One or the other
			// NOTE: i tried this out a bunch, and I don't see any race conditions, so I'm leaving it like this.

			float velocity = dt * PLAYER_VELOCITY;

			// Keyboard movements
			glm::vec2 pos = player->get_position();
			glm::vec2 ball_pos = ball->get_position();

			float right_difference = this->width - player->get_size().x - pos.x;
			float left_difference = pos.x;

			if (right_difference >= 0.0f && left_difference >= 0.0f) {
				// Ensure the positions aren't going to be out of bounds
				float displacement = this->keys[GLFW_KEY_A] ? fmaxf(-velocity, -left_difference) : fminf(velocity, right_difference);
				player->set_position(glm::vec2(pos.x + displacement, pos.y));
				if (ball->is_stuck()) {
					ball->set_position(glm::vec2(ball_pos.x + displacement, ball_pos.y));
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
		postprocessor->begin_render();

		Texture2D &bg_tex = Resource_Manager::get_texture("background");
		renderer->draw_sprite(bg_tex, glm::vec2(0.0f), glm::vec2(this->width, this->height), 0.0f);

		this->levels[current_level].draw(*renderer);
		this->player->draw(*renderer);
		if (!ball->is_stuck()) emitter->draw();
		ball->draw(*renderer);

		for (Power_Up &power_up : power_ups) {
			if (!power_up.get_destroyed())
				power_up.draw(*renderer);
		}

		postprocessor->end_render();
		postprocessor->render(static_cast<float>(glfwGetTime()));
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
