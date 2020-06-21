#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
	private:
		unsigned int shader_id; // Shader program id

		void shader_error(GLuint shader_object, std::string type);

		int compile(const char *shader_source, GLenum shader_type, unsigned int &shader);
		int link(const unsigned int vertex_shader, const unsigned int frag_shader, unsigned int &shader_program);
	public:
		Shader(const char *vertex_path, const char *fragment_path); // Constructor

		Shader &use(); // Used to activate shader

		unsigned int get_shader_id();

		// Utilities: setters for uniforms
		// NOTE: const functions can only be called by an object that's been declared const
		void set_bool(const std::string & name, bool value) const;
		void set_int(const std::string & name, int value) const;
		void set_float(const std::string & name, float value) const;

		/**
		 * Sets a 4x4 transformation matrix. Recall that glm/glsl is column major: THIS WILL NOT TRANSPOSE
		 */
		void set_mat4(const std::string & name, glm::mat4 value) const;

		void set_vec2(const std::string & name, glm::vec2 value) const;
		void set_vec3(const std::string & name, glm::vec3 value) const;
		void set_vec4(const std::string & name, glm::vec4 value) const;
};
