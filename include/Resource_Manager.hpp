#pragma once

#include <glad/glad.h>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>

#include "Texture.hpp"
#include "Shader.hpp"
#include "stb_image.h"


/**
 * Singleton resource manager.
 * Hosts functions for shaders and textures. All members are static.
 */
// TODO: this is kinda dumb. FInd a non-singleton way of doing this, or a cleaner implementation!
class Resource_Manager {
	private:
		static std::map<std::string, Shader> shaders; // shaders of the game
		static std::map<std::string, Texture2D> textures; // textures of the game


		Resource_Manager() { } // Private constructor; never instantiate an instance of this class


		/**
		 * Loads a shader from filepaths.
		 * 
		 * @param vertex_path   Vertex shader path
		 * @param fragment_path Fragment shader path
		 * @param geometry_path Optional geometry shader path
		 * 
		 * @return Shader: a shader program object
		 */
		static Shader load_shader_from_file(const char *vertex_path, const char *fragment_path, const char *geometry_path=nullptr);


		/**
		 * Loads a texture from an image path.
		 * 
		 * @param const char *file: file path
		 * @param bool alpha: reads in alpha channel, if it exists & is true
		 * 
		 * @return Texture2D: a texture object
		 */
		static Texture2D load_texture_from_file(const char *file, bool alpha);
	
	
	public:
		/**
		 * Load & generate a shader program from a bunch of filepaths, and puts it into the internal map.
		 * 
		 * @param vertex_path   Vertex shader path
		 * @param fragment_path Fragment shader path
		 * @param geometry_path Optional. Set to nullptr or NULL if undesired.
		 * @param std::string name: name in the map.
		 * 
		 * @return Shader: A shader program
		 */
		static Shader load_shader(const char *vertex_path, const char *fragment_path, const char *geometry_path, std::string name);


		/**
		 * Get a shader.
		 * 
		 * @param name Name of shader
		 * 
		 * @return Shader: shader with the name.
		 */
		static Shader get_shader(std::string name);


		/**
		 * Load & generate a texture from a filepath, and puts it into the internal map.
		 * 
		 * @param path
		 * @param alpha True to read in alpha channel, if it exists
		 * @param name  Name in the ResourceManager's map
		 * 
		 * @return Texture2D: A texture
		 */
		static Texture2D load_texture(const char *file, bool alpha, std::string name);


		/**
		 * Get a texture
		 * 
		 * @param name Name of texture in resource manager.
		 * 
		 * @return Texture2D: texture with the name
		 */
		static Texture2D get_texture(std::string name);


		/**
		 * Clears all resources in both maps
		 */
		static void clear();
};
