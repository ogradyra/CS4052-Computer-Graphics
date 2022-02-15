#pragma once
#include "maths_funcs.h"
#include "structs.h"

void draw_spider(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rb, scale_tree s, unsigned int tex, int texnum, int texnum_loc, int matrix_location, vertex_array_object vao, rotation r, mesh m);

void draw_tree(GLfloat tx, GLfloat ty, GLfloat tz, scale_tree s, unsigned int texture1, unsigned int texture2, int texnum_loc, int matrix_location, vertex_array_object vao, mesh m);