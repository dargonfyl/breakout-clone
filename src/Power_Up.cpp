#include "Power_Up.hpp"


Power_Up::Power_Up(const Power_Up_Type type, const glm::vec3 color, float duration, const glm::vec2 position, Texture2D &texture) :
    Game_Object(position, glm::vec2(60.0f, 20.0f), texture, color, glm::vec2(0.0f, 350.0f)),
    type(type),
    duration(duration),
    active(false) { }


Power_Up::~Power_Up() { }


Power_Up_Type Power_Up::get_type() {
    return type;
}


void Power_Up::set_active(bool active) {
    this->active = active;
}


bool Power_Up::is_active() {
    return this->active;
}


float Power_Up::get_duration() {
    return this->duration;
}


void Power_Up::set_duration(float duration) {
    this->duration = duration;
}
