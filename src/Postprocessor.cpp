#include <Postprocessor.hpp>

namespace
{
	// Only initializes VAO/VBO
	void init_render_data(unsigned int *VAO) {
		// Ughhhhh
		unsigned int VBO;

		float vertices[] = {
			// pos        // tex
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(1, VAO);
		assert(*VAO);
		glBindVertexArray(*VAO);

		glGenBuffers(1, &VBO);
		assert(VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
} // namespace


Postprocessor::Postprocessor(Shader shader, const unsigned int &width, const unsigned int &height) {
	this->width = width;
	this->height = height;
	this->postprocessing_shader = shader;

	// Generate framebuffers
	glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);

	assert(this->MSFBO && this->FBO && this->RBO);

	// Initialize renderbuffer regularly.
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);

	// Attach RBO to current framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Could not init multisample framebuffer\n";
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->texture.generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.get_id(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Could not init framebuffer\n";
		exit(1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	init_render_data(&this->VAO);

	// this->postprocessing_shader.set_int("image", 0);
	this->shake   = false;
	this->chaos   = false;
	this->confuse = false;
}

void Postprocessor::begin_render() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Postprocessor::end_render() {
	// Resolve MSFBO into the intermediate FBO
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Will bind both read/draw framebuffers to 0
}

void Postprocessor::render(const float &time) {
	Shader shader = this->postprocessing_shader;
	shader.use();
	shader.set_float("effects.time", time);
	shader.set_bool("effects.chaos", this->chaos);
	shader.set_bool("effects.confuse", this->confuse);
	shader.set_bool("effects.shake", this->shake);

	const unsigned int shader_id = this->postprocessing_shader.get_shader_id();
	const float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right
	};
	glUniform2fv(
		glGetUniformLocation(
			shader_id,
			"offsets"
		),
		9,
		(float *)offsets
	);

	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(
		glGetUniformLocation(
			shader_id,
			"edge_kernel"
		),
		9,
		edge_kernel
	);

	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(
		glGetUniformLocation(
			shader_id,
			"blur_kernel"
		),
		9,
		blur_kernel
	);


	// Render quad
	glActiveTexture(GL_TEXTURE0);
	this->texture.bind();

	assert(this->VAO && "VAO not 0");
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
