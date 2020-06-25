#include "Sprite_Renderer.hpp"


/**
 * Transform a quad's center to the origin, then rotate, then translate back.
 * 
 * @param glm::vec2 size: size of the quad.
 * @param float rotate: angle in degrees to rotate.
 * 
 * @return glm::mat4: rotation matrix.
 */
glm::mat4 get_center_rotate(glm::vec2 size, float rotate) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::translate(result, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	result = glm::rotate(result, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate about z axis
	result = glm::translate(result, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	return result;
}


Sprite_Renderer::Sprite_Renderer(Shader &shader) {
	this->shader = shader;
	assert(this->shader.get_shader_id() != 0 && "Shader in Sprite_Renderer::Sprite_Renderer has shader_id == 0");
	this->init_render_data();
}


void Sprite_Renderer::init_render_data() {
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 
	
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	assert(this->VAO != 0 && "VAO == 0 in Sprite_Renderer::init_render_data()");

	glGenBuffers(1, &this->VBO);
	assert(this->VBO != 0 && "VBO == 0 in Sprite_Renderer::inin_render_data()");
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Static because they're usually unchanging

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Sprite_Renderer::draw_sprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 colour) {
	this->shader.use();

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	// model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));

	model = model * get_center_rotate(size, rotate);

	model = glm::scale(model, glm::vec3(size, 1.0f)); // scale, then rotate, then translate

	shader.set_mat4("model", model);
	shader.set_vec3("sprite_colour", colour);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	assert(VAO != 0 && "VAO == 0 when trying to draw sprite");
	glBindVertexArray(this->VAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
