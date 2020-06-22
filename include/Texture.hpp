#pragma once

#include <glad/glad.h>


class Texture2D {
	private:
		unsigned int width, height;

		unsigned int internal_format; // Format of texture object
		unsigned int image_format; // Format of loaded texture

		// Texture configs
		unsigned int wrap_s, wrap_t, filter_min, filter_max;

	public:
		// How the hell do you put this thing into private while letting opengl change it in gldeletetexture?????
		// if i had to be honest, i'd say that the solution is to call gldeletetexture in a destructor rather than exposing this member
		// but honestly, mixing the c++ and c styles is a terrible idea
		// TODO: this is dumb
		unsigned int id;


		/**
		 * Constructor for Texture2D
		 * A bunch of internal members are set to their default values.
		 */
		Texture2D();


		/**
		 * Generates texture from image data
		 * 
		 * @param unsigned int width: image width
		 * @param unsigned int height: image height
		 * @param unsigned char *data: image data
		 */
		void generate(unsigned int width, unsigned int height, unsigned char *data);


		/**
		 * Bind this texture to the graphics pipeline.
		 */
		void bind() const;


		/**
		 * Sets the internal and image formats of this texture.
		 * Set an argument to 0 if you want to leave it unchanged.
		 * 
		 * @param GLenum internal_format
		 * @param GLenum image_format
		 */
		void set_formats(GLenum internal_format, GLenum image_format);
};
