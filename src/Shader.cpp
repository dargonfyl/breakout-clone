#include "Shader.hpp"


Shader::Shader(const char *vertex_code, const char *fragment_code, const char *geometry_code) {
	int success;
	unsigned int vertex_shader, fragment_shader, geometry_shader;
	
	success = compile(vertex_code, GL_VERTEX_SHADER, vertex_shader);
	if (!success) shader_error(vertex_shader, "VERTEX");

	success = compile(fragment_code, GL_FRAGMENT_SHADER, fragment_shader);
	if (!success) shader_error(fragment_shader, "FRAGMENT");

	if (geometry_code != nullptr) {
		success = compile(fragment_code, GL_FRAGMENT_SHADER, geometry_shader);
		if (!success) shader_error(fragment_shader, "GEOMETRY");
	}

	unsigned int shader_program;
	success = geometry_code == nullptr ? link(vertex_shader, fragment_shader, shader_program) : 
										 link(vertex_shader, fragment_shader, shader_program, geometry_shader);
	if (!success) shader_error(fragment_shader, "PROGRAM");

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


int Shader::link(const unsigned int vertex_shader, const unsigned int frag_shader, unsigned int &shader_program, const unsigned int geom_shader) {
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, frag_shader);
	if (geom_shader != 0) glAttachShader(shader_program, geom_shader);
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
		std::cerr << "ERROR::SHADER::" << type << "::LINK_FAILED\n" << info_log << std::endl;
	exit(EXIT_FAILURE);
}


unsigned int Shader::get_shader_id() {
	return shader_id; 
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
