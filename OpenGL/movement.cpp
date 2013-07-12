#include "movement.h"

static float vertical_angle = 0.0f;
static float horizontal_angle = 3.14f;
static glm::vec3 right = glm::vec3(1.0f,0.0f,0.0f);

void mouse_movement(double x,double y,double size_x,double size_y,glm::vec3 &direction, glm::vec3 &up)
{
	horizontal_angle += mouseSpeed * float( size_x/2.0 - x );
	vertical_angle   += mouseSpeed * float( size_y/2.0 - y );
	if(vertical_angle > 3.14) vertical_angle = 3.14;
	else if(vertical_angle < -3.14) vertical_angle = -3.14;
	//std::cout << "Horizontal: " << horizontal_angle << "\n";
	//std::cout << "Vertical:   " << vertical_angle << "\n";
	direction = glm::vec3(cos(vertical_angle) * sin(horizontal_angle),sin(vertical_angle),cos(vertical_angle) * cos(horizontal_angle));
	right = glm::vec3(sin(horizontal_angle - 3.14f/2.0f),0,cos(horizontal_angle - 3.14f/2.0f));
	up = glm::cross( right, direction );
}

int keyboard_movement(int key, int scancode, int action, int mods,glm::vec3 &position, glm::vec3 direction)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		return CLOSE_WINDOW;
	if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		position += direction * glm::vec3(keyboard_movement_speed);
	if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		position -= direction * glm::vec3(keyboard_movement_speed);
	if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		position -= right * glm::vec3(keyboard_movement_speed);
	if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		position += right * glm::vec3(keyboard_movement_speed);
	return !CLOSE_WINDOW;
}