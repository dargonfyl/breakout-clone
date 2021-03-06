#include "Texture.hpp"


Texture2D::Texture2D() {
	width = 0;
	height = 0;
	internal_format = GL_RGB;
	image_format = GL_RGB;

	wrap_s = GL_REPEAT;
	wrap_t = GL_REPEAT;
	filter_min = GL_LINEAR;
	filter_max = GL_LINEAR;

	glGenTextures(1, &this->id);
	assert(id != 0 && "Texture2D::id == 0 in constructor");
}


void Texture2D::generate(unsigned int width, unsigned int height, unsigned char *data) {
	assert(width > 0 && "width is 0 in Texture2D::generate");
	assert(height > 0 && "height is 0 in Texture2D::generate");
	this->width = width;
	this->height = height;

	assert(id != 0 && "Texture2D::id == 0 in Texture2D::generate");
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

	glBindTexture(GL_TEXTURE_2D, 0);
}


unsigned int Texture2D::get_id() {
	return id;
}


void Texture2D::delete_texture() {
	glDeleteTextures(1, &id);
}


void Texture2D::bind() {
	assert(id != 0 && "Texture2D::id == 0 in Texture2D::bind");
    glBindTexture(GL_TEXTURE_2D, this->id);
}


void Texture2D::set_formats(GLenum internal_format, GLenum image_format) {
	this->internal_format = internal_format == 0 ? this->internal_format : internal_format;
	this->image_format = image_format == 0 ? this->image_format : image_format;
}
