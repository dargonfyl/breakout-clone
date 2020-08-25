#include "Game_Level.hpp"


Game_Level::Game_Level() { }


Game_Level::Game_Level(const char *file, unsigned int level_width, unsigned int level_height) {
	this->load(file, level_width, level_height);
}


void Game_Level::init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height) {
	// Compute dimensions of the game
	unsigned int height = tile_data.size();
	unsigned int width = tile_data[0].size();
	float unit_width = (float)(level_width / width);
	float unit_height = (float)(level_height / height);

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			// Check block type
			unsigned int tile = tile_data[y][x];
			if (tile == EMPTY_TILE) continue;
			
			// Here, we know it's a real block
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			// Solid/unbreakable tiles
			if (tile == SOLID_TILE) {
				Texture2D solid_tex = Resource_Manager::get_texture("solid_tile");
				glm::vec3 solid_col = glm::vec3(0.8f, 0.8f, 0.7f);
				Tile brick = Tile(pos, size, solid_tex, solid_col);

				this->bricks.push_back(brick);
			} else {
				// TODO: this is hard coded right now, but it should be dynamic, i.e. in a map and loaded by JSON
				glm::vec3 block_col;
				switch(tile) {
					case BLUE_TILE:
						block_col = glm::vec3(0.2f, 0.6f, 1.0f);
						break;
					case GREEN_TILE:
						block_col = glm::vec3(0.0f, 0.7f, 0.0f);
						break;
					case YELLOW_TILE:
						block_col = glm::vec3(0.8f, 0.8f, 0.4f);
						break;
					case RED_TILE:
						block_col = glm::vec3(1.0f, 0.5f, 0.0f);
						break;
					default:
						block_col = glm::vec3(1.0f);
						break;
				}

				Texture2D block_tex = Resource_Manager::get_texture("block_tile");
				Tile brick = Tile(pos, size, block_tex, block_col);
				this->bricks.push_back(brick);
			}
		}
	}
}


void Game_Level::load(const char *file, unsigned int level_width, unsigned int level_height) {
	
	this->bricks.clear();
	unsigned int tile_code;

	std::string line;
    std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tile_data;
	unsigned int row_length = 0;

	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);

			std::vector<unsigned int> row;
			while (sstream >> tile_code) {
				row.push_back(tile_code);
			}

			if (!row_length) {
				row_length = row.size();
			}
			assert(row_length == row.size() && "Row length is not consistent in level file");
			tile_data.push_back(row);
		}

		if (tile_data.size() > 0) {
			this->init(tile_data, level_width, level_height);
		}
	}
}


bool Game_Level::is_completed() {
    for (Tile &tile : this->bricks) {
		if (!tile.is_solid() && !tile.get_destroyed()) return false;
	}
    return true;
}


void Game_Level::draw(Sprite_Renderer &renderer) {
	for (Tile &tile : this->bricks) {
		if (!tile.get_destroyed()) tile.draw(renderer);
	}
}


std::vector<Tile> &Game_Level::get_bricks() {
	return this->bricks;  // Automatically returns reference
}
