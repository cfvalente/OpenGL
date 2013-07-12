#ifndef MOVEMENT
#define MOVEMENT


#include <iostream>
#include "Include/glm/glm.hpp"
#include "Include/GLFW/glfw3.h"

#define CLOSE_WINDOW 1
#define mouseSpeed 0.0005f
#define keyboard_movement_speed 1

void mouse_movement(double x,double y,double size_x,double size_y,glm::vec3 &direction, glm::vec3 &up);
int keyboard_movement(int key, int scancode, int action, int mods,glm::vec3 &position, glm::vec3 direction);

#endif