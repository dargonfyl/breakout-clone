#include "main.hpp"


Game Breakout(WINDOW_WIDTH, WINDOW_HEIGHT); // Singleton ughhhh

/**
 * Resizes viewport should the window resize.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}



void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// Exit key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024) { // These are the standard keyboard keys
		if (action == GLFW_PRESS) Breakout.press_key(key);
		else if (action == GLFW_RELEASE) Breakout.release_key(key);
	}
}


/**
 * Initializes a GLFW window & sets GLFW/GL states.
 * 
 * NOTE: There should only ever be 1 window, but I do not recommend singleton.
 */
GLFWwindow *init_window() {
	// INITIALIZATION
	if (!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	#ifndef _WIN32
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No idea why, but this breaks everything on Windows 7
	#endif

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	glfwWindowHint(GLFW_RESIZABLE, false);

	// Creates window
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BREAKOUT", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD initialize; we must do this before all OpenGL lib functions
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	// GL config; these don't change much
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);  // Blend for alpha things
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}


int main() {
	GLFWwindow *window = init_window();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	Breakout.init();
	Breakout.set_state(GAME_MENU);

	float delta_time = 0.0f;
	float last_frame = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		float current_frame = glfwGetTime(); // TODO: Framerate things into their own class?
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		glfwPollEvents();

		// Game events
		Breakout.process_input(delta_time);


		// Update game state
		Breakout.update(delta_time);

		// Rendering things
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.render();

		glfwSwapBuffers(window);

	}
	Resource_Manager::clear();

	glfwTerminate();

	return 0;
}
