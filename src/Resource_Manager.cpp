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
	shaders[name] = load_shader_from_file(vertex_path, fragment_path, geometry_path);
	return shaders[name];
}


Shader Resource_Manager::get_shader(std::string name) {
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
		glDeleteProgram(iter.second.get_shader_id());
	for (auto iter : textures)
		glDeleteTextures(1, &iter.second.id);
}


Shader Resource_Manager::load_shader_from_file(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
	std::string vertex_code, fragment_code, geometry_code;

	vertex_code = read_shader(vertex_path);
	fragment_code = read_shader(fragment_path);
	if (geometry_path != nullptr) {
		geometry_code = read_shader(geometry_path);
	}

	return Shader(vertex_code.c_str(), fragment_code.c_str(), geometry_code.c_str());
}


Texture2D Resource_Manager::load_texture_from_file(const char *file, bool alpha) {
	Texture2D texture = Texture2D();
	if (alpha) {
		texture.set_formats(GL_RGBA, GL_RGBA);
	}

	int width, height, num_channels;
	unsigned char* data = stbi_load(file, &width, &height, &num_channels, 0);  // WTF does it ask for signed?

	texture.generate(width, height, data);
	stbi_image_free(data);
	return texture;
}
