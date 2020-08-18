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
		unsigned int shader_id = 0; // Shader program id. If this is 0, this is not a valid shader and should not be referenced.


		/**
		 * Reports a shader error for a shader object with id shader_object.
		 * 
		 * @param shader_object GLuint id of shader object
		 * @param type          Type of the shader object. (i.e. "program", "fragment shader", etc.)
		 */
		void shader_error(GLuint shader_object, std::string type);


		/**
		 * Compiles a shader.
		 * 
		 * @param shader_source path to glsl file of shader
		 * @param shader_type   Type of the shader, defined by a GLenum
		 * @param shader        shader id reference to set
		 * 
		 * @return int/GLint: GL_TRUE if compile success, GL_FALSE otherwise
		 */
		int compile(const char *shader_source, GLenum shader_type, unsigned int &shader);


		/**
		 * Links a vertex & fragment shader into a program. Will also set shader_id.
		 * 
		 * @param vertex_shader  vertex shader id
		 * @param frag_shader    fragment shader id
		 * @param shader_program shader program id to set
		 * 
		 * @return int/GLint: GL_TRUE if compile success, GL_FALSE otherwise
		 */
		int link(const unsigned int vertex_shader, const unsigned int frag_shader, unsigned int &shader_program, const unsigned int geom_shader=0);


	public:
		/**
		 * Deprecated. Default constructor so that std::map has a default constructor.
		 * 
		 * @deprecated
		 */
		Shader() { }


		/**
		 * Shader constrctor. We enforce that Shader::get_shader_id() != 0 after constructor, otherwise this is an invalid shader.
		 * 
		 * @param vertex_code   code for a vertex shader
		 * @param fragment_code code for a fragment shader
		 * @param geometry_code Optional code for a geometry shader
		 */
		Shader(const char *vertex_code, const char *fragment_code, const char *geometry_code=nullptr); // Constructor


		/**
		 * Activates a shader. Remember to set uniforms!
		 */
		Shader &use(); // Used to activate shader


		/**
		 * Deletes the associated glShaderProgram.
		 * 
		 * NOTE: this does not delete the object!
		 */
		void delete_shader();


		/**
		 * Gets this shader program's id.
		 */
		unsigned int get_shader_id();


		// Utilities: setters for uniforms
		// NOTE: const functions can only be called by an object that's been declared const


		void set_bool(const std::string & name, bool value) const;
		void set_int(const std::string & name, int value) const;
		void set_float(const std::string & name, float value) const;


		/**
		 * Sets a 4x4 transformation matrix. 
		 * 
		 * Warning: glm is column major; you may need to transpose your matrix!
		 * 
		 * @param name name of uniform
		 * @param value value of uniform
		 */
		void set_mat4(const std::string & name, glm::mat4 value) const;


		void set_vec2(const std::string & name, glm::vec2 value) const;
		void set_vec3(const std::string & name, glm::vec3 value) const;
		void set_vec4(const std::string & name, glm::vec4 value) const;
};
