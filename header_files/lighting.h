#pragma once

// GLM includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void change_light(unsigned char key, int x, int y);
void spider_lighting(int programID, glm::mat4 view);
void env_lighting(int programID, glm::mat4 view);
void fire_lighting(int programID, glm::mat4 view);