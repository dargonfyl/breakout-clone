#pragma once

#include <glad/glad.h>


class Texture2D {
	private:
		unsigned int id;

		unsigned int width, height;

		unsigned int internal_format; // Format of texture object
		unsigned int image_format; // Format of loaded texture

		// Texture configs
		unsigned int wrap_s, wrap_t, filter_min, filter_max;

	public:
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
};
