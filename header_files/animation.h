#pragma once
#include "structs.h"

void animate(rotation &r, GLfloat& translate_y);
void walk(rotation& r, GLfloat& translate_x, GLfloat& translate_y, GLfloat& translate_z, GLfloat& rotate_body);
void boundary_walk(GLfloat& translate_x, GLfloat& translate_z, float& rotate_body, bool turn);
void avoid_spider(GLfloat tx1, GLfloat tz1, GLfloat& tx2, GLfloat& tz2, GLfloat& rb2);
void avoid_fire(GLfloat& tx2, GLfloat& tz2, GLfloat& rb2);
void walk_spider(GLfloat& tx2, GLfloat& tz2, GLfloat rb2);
