#include "Resource_Manager.hpp"


std::map<std::string, Texture2D> Resource_Manager::textures;
std::map<std::string, Shader> Resource_Manager::shaders;


/**
 * Reads code of a glsl file and puts it into a string
 * 
 * @param const char *path
 * 
 * @return std::string: code of a shader file
 */
std::string read_shader(const char *path) {
	try {
		std::ifstream filestream(path);
		std::stringstream codestream;

		codestream << filestream.rdbuf();

		filestream.close();

		return codestream.str();
	} catch (std::exception e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		exit(EXIT_FAILURE);
	}
}


Shader Resource_Manager::load_shader(const char *vertex_path, const char *fragment_path, const char *geometry_path, std::string name) {
	Shader shader = load_shader_from_file(vertex_path, fragment_path, geometry_path);
	assert(shader.get_shader_id() != 0 && "Resource_Manager::load_shader shader has Shader::get_shader_id() == 0");
	shaders[name] = shader;
	
	return shaders[name];
}


Shader Resource_Manager::get_shader(std::string name) {
	assert(shaders[name].get_shader_id() != 0 && "Resource_Manager::get_shader shader has Shader::get_shader_id() == 0");
	return shaders[name];
}


Texture2D Resource_Manager::load_texture(const char *file, bool alpha, std::string name) {
	textures[name] = load_texture_from_file(file, alpha);
	return textures[name];
}


Texture2D Resource_Manager::get_texture(std::string name) {
	return textures[name];
}


void Resource_Manager::clear() {
	for (auto iter : shaders)
		iter.second.delete_shader();
	for (auto iter : textures)
		iter.second.delete_texture();
}


Shader Resource_Manager::load_shader_from_file(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
	std::string vertex_code, fragment_code, geometry_code;

	vertex_code = read_shader(vertex_path);
	assert(vertex_code.compare("") != 0 && "Resource_Manager::load_shader_from_file: vertex_code not read properly or is empty");
	fragment_code = read_shader(fragment_path);
	assert(fragment_code.compare("") != 0 && "Resource_Manager::load_shader_from_file: fragment_code not read properly or is empty");

	if (geometry_path != nullptr) {
		geometry_code = read_shader(geometry_path);
		assert(geometry_code.compare("") != 0 && "Resource_Manager::load_shader_from_file: geometry_code not read properly or is empty");
		return Shader(vertex_code.c_str(), fragment_code.c_str(), geometry_code.c_str());
	}

	return Shader(vertex_code.c_str(), fragment_code.c_str(), nullptr);
}


Texture2D Resource_Manager::load_texture_from_file(const char *file, bool alpha) {
	Texture2D texture = Texture2D();
	if (alpha) {
		texture.set_formats(GL_RGBA, GL_RGBA);
	}

	int width, height, num_channels;
	unsigned char *data = stbi_load(file, &width, &height, &num_channels, 0);  // WTF does it ask for signed?
	if (!data) {
		std::cerr << "Failed to load image" << std::endl;
		exit(EXIT_FAILURE);
	}

	texture.generate(width, height, data);
	stbi_image_free(data);
	return texture;
}
