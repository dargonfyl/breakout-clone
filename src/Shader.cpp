#include "Shader.hpp"


Shader::Shader(const char *vertex_path, const char *fragment_path) {
	std::string vertex_code, fragment_code;
	std::ifstream vshader_file, fshader_file;

	vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files
		vshader_file.open(vertex_path);
		fshader_file.open(fragment_path);
		std::stringstream vshader_stream, fshader_stream;

		// Read file's buffer contents into stream
		vshader_stream << vshader_file.rdbuf();
		fshader_stream << fshader_file.rdbuf();

		// Close file handlers
		vshader_file.close();
		fshader_file.close();

		// Convert stream to string
		vertex_code = vshader_stream.str();
		fragment_code = fshader_stream.str();
	} catch(std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		exit(EXIT_FAILURE);
	}

	const char *vshader_code = vertex_code.c_str();
	const char *fshader_code = fragment_code.c_str();
	int success;

	unsigned int vertex_shader, fragment_shader;
	success = compile(vshader_code, GL_VERTEX_SHADER, vertex_shader);
	if (!success) shader_error(vertex_shader, "VERTEX_SHADER");

	success = compile(fshader_code, GL_FRAGMENT_SHADER, fragment_shader);
	if (!success) shader_error(vertex_shader, "FRAGMENT_SHADER");

	unsigned int shader_program;
	success = link(vertex_shader, fragment_shader, shader_program);
	if (!success) shader_error(vertex_shader, "PROGRAM");

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	shader_id = shader_program;
}


Shader &Shader::use() {
	glUseProgram(shader_id);
	return *this;  // Returns reference, so return value of this pointer
}


int Shader::compile(const char *shader_source, GLenum shader_type, unsigned int &shader) {
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, 0);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // this returns a parameter from a shader object
	return success;
}


int Shader::link(const unsigned int vertex_shader, const unsigned int frag_shader, unsigned int &shader_program) {
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	int success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	return success;
}


void Shader::shader_error(GLuint shader_object, std::string type) {
	char info_log[512];
	glGetShaderInfoLog(shader_object, 512, NULL, info_log);
	if (type != "PROGRAM")
		std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << info_log << std::endl;
	else
		std::cerr << "ERROR::SHADER::" << type << "::LINK-FAILED\n" << info_log << std::endl;
	exit(EXIT_FAILURE);
}


void Shader::set_bool(const std::string & name, bool value) const {         
	glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)value); 
}


void Shader::set_int(const std::string & name, int value) const { 
	glUniform1i(glGetUniformLocation(shader_id, name.c_str()), value); 
}


void Shader::set_float(const std::string & name, float value) const { 
	glUniform1f(glGetUniformLocation(shader_id, name.c_str()), value); 
} 


void Shader::set_mat4(const std::string & name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(shader_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::set_vec2(const std::string & name, glm::vec2 value) const {
	glUniform2fv(glGetUniformLocation(shader_id, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::set_vec3(const std::string & name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(shader_id, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::set_vec4(const std::string & name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(shader_id, name.c_str()), 1, glm::value_ptr(value));
}