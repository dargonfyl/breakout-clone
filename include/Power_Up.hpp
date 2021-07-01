#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Game_Object.hpp"
#include "Texture.hpp"


enum Power_Up_Type {
	None           = 0,
	Speed          = 1,
	Sticky         = 2,
	Pass_Through   = 3,
	Paddle_Size_Up = 4,
	Confuse        = 5,
	Chaos          = 6
};

class Power_Up : public Game_Object {
	private:
		Power_Up_Type type;
		float         duration;
		bool 		  active;
	public:
		/**
		 * 
		 */
		Power_Up(const Power_Up_Type type, const glm::vec3 color, float duration, const glm::vec2 position, Texture2D &texture);


		~Power_Up();


		Power_Up_Type get_type();


		void set_active(bool active);


		bool is_active();


		float get_duration();


		void set_duration(float duration);
};
