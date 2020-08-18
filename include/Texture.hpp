#pragma once

#include <glad/glad.h>
#include <assert.h>


/**
 * Texture2D class.
 * Handles 2D textures and their attributes.
 */
class Texture2D {
	private:
		unsigned int id;
		unsigned int width, height;

		unsigned int internal_format; // Format of texture object
		unsigned int image_format; // Format of loaded texture

		// Texture configs
		unsigned int wrap_s, wrap_t, filter_min, filter_max;


	public:
		/**
		 * Constructor for Texture2D
		 * A bunch of internal members are set to their default values.
		 */
		Texture2D();


		/**
		 * Gets the texture id for this glTexture
		 */
		unsigned int get_id();


		/**
		 * Deletes a glTexture.
		 * 
		 * NOTE: this does not delete this object!
		 */
		void delete_texture();


		/**
		 * Generates texture from image data
		 * 
		 * @param width  image width
		 * @param height image height
		 * @param data   image data
		 */
		void generate(unsigned int width, unsigned int height, unsigned char *data);


		/**
		 * Bind this texture to the graphics pipeline.
		 */
		void bind();


		/**
		 * Sets the internal and image formats of this texture. 
		 * 
		 * Set an argument to 0 if you want to leave it unchanged.
		 * 
		 * @param internal_format As defined by a GLenum
		 * @param image_format    As defined by a GLenum
		 */
		void set_formats(GLenum internal_format, GLenum image_format);
};
