#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void keypress(unsigned char key, int x, int y);
void mouseclick(int xpos, int ypos);
void view_proj_matrix(glm::mat4 &persp_proj, glm::mat4 &view);