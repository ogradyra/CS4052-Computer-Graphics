#pragma warning(disable : 5208)
#define NOMINMAX
#define USE_MATH_DEFINES

#include <limits>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "structs.h"
#include "animation.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

bool first_time = true;
bool middle = false;
bool backwards = true;

GLfloat increment_r1 = 0.0f;
GLfloat increment_r2 = 0.0f;
GLfloat increment_r3 = 0.0f;
GLfloat increment_x = 0.5f;
GLfloat increment_z = 0.5f;

void walk(rotation& r, GLfloat& translate_x, GLfloat& translate_y, GLfloat& translate_z, GLfloat& rotate_body) {

	if (backwards) {
		translate_y += 0.05f;

		r.rz_j2 = 0.0f;
		r.rz_j3 = 15.0f;
		r.rz_j4 = 0.0f;
		r.rz_j5 = 15.0f;


		r.ry_j1 -= 10.0f;
		r.ry_j2 += 10.0f;
		r.ry_j3 -= 10.0f;
		r.ry_j4 += 10.0f;

		r.rl_j1 = 0.0f;
		r.rl_j2 = 0.0f;
		r.rl_j3 = 10.0f;
		r.rl_j4 = 0.0f;

		r.ry_j5 += 10.0f;
		r.ry_j6 -= 10.0f;
		r.ry_j7 += 10.0f;
		r.ry_j8 -= 10.0f;

		r.rl_j5 = -40.0f;
		r.rl_j6 = 0.0f;
		r.rl_j7 = -10.0f;
		r.rl_j8 = 20.0f;

		backwards = false;
	}

	else {
		translate_y -= 0.05f;

		r.rz_j2 = 15.0f;
		r.rz_j3 = 0.0f;
		r.rz_j4 = 15.0f;
		r.rz_j5 = 0.0f;


		r.ry_j1 += 10.0f;
		r.ry_j2 -= 10.0f;
		r.ry_j3 += 10.0f;
		r.ry_j4 -= 10.0f;

		r.rl_j1 = -40.0f;
		r.rl_j2 = 10.0f;
		r.rl_j3 = 0.0f;
		r.rl_j4 = 20.0f;

		r.ry_j5 -= 10.0f;
		r.ry_j6 += 10.0f;
		r.ry_j7 -= 10.0f;
		r.ry_j8 += 10.0f;

		r.rl_j5 = 0.0f;
		r.rl_j6 = -10.0f;
		r.rl_j7 = 0.0f;
		r.rl_j8 = 0.0f;

		backwards = true;
	}

	if (translate_x == -12.0f && -12.0f <= translate_z < 12.0f) {

		if (!first_time) {
			rotate_body = 360.0f;
		}

		translate_z += 1.0f;
		first_time = false;
	}

	if (translate_z == 12.0f && -12.0f <= translate_x < 12.0f) {
		rotate_body = 90.0f;
		translate_x += 1.0f;
	}

	if (translate_x == 12.0f && -12.0f < translate_z <= 12.0f) {
		rotate_body = 180.0f;
		translate_z -= 1.0f;
	}

	if (translate_z == -12.0f && -12.0f < translate_x <= 12.0f) {
		rotate_body = 270.0f;
		translate_x -= 1.0f;
	}

}

void animate(rotation& r, GLfloat& translate_y) {
	
	if (backwards && !middle) {
		translate_y += 0.05f;

		r.rz_j2 = 0.0f;
		r.rz_j3 = 15.0f;
		r.rz_j4 = 0.0f;
		r.rz_j5 = 15.0f;


		r.ry_j1 -= 5.0f;
		r.ry_j2 += 5.0f;
		r.ry_j3 -= 5.0f;
		r.ry_j4 += 5.0f;

		r.rl_j1 = 0.0f;
		r.rl_j2 = 0.0f;
		r.rl_j3 = 10.0f;
		r.rl_j4 = 0.0f;

		r.ry_j5 += 5.0f;
		r.ry_j6 -= 5.0f;
		r.ry_j7 += 5.0f;
		r.ry_j8 -= 5.0f;

		r.rl_j5 = -40.0f;
		r.rl_j6 = 0.0f;
		r.rl_j7 = -10.0f;
		r.rl_j8 = 20.0f;

		middle = true;
	}

	else if (middle && backwards) {
		translate_y += 0.05f;

		r.rz_j2 = 7.5f;
		r.rz_j3 = 7.5f;
		r.rz_j4 = 7.5f;
		r.rz_j5 = 7.5f;


		r.ry_j1 -= 5.0f;
		r.ry_j2 += 5.0f;
		r.ry_j3 -= 5.0f;
		r.ry_j4 += 5.0f;

		r.rl_j1 = -20.0f;
		r.rl_j2 = 5.0f;
		r.rl_j3 = 5.0f;
		r.rl_j4 = 10.0f;

		r.ry_j5 += 5.0f;
		r.ry_j6 -= 5.0f;
		r.ry_j7 += 5.0f;
		r.ry_j8 -= 5.0f;

		r.rl_j5 = -20.0f;
		r.rl_j6 = -5.0f;
		r.rl_j7 = -5.0f;
		r.rl_j8 = 10.0f;

		backwards = false;
	}

	else if (!backwards && middle) {
		translate_y -= 0.05f;

		r.rz_j2 = 15.0f;
		r.rz_j3 = 0.0f;
		r.rz_j4 = 15.0f;
		r.rz_j5 = 0.0f;


		r.ry_j1 += 5.0f;
		r.ry_j2 -= 5.0f;
		r.ry_j3 += 5.0f;
		r.ry_j4 -= 5.0f;

		r.rl_j1 = -40.0f;
		r.rl_j2 = 10.0f;
		r.rl_j3 = 0.0f;
		r.rl_j4 = 20.0f;

		r.ry_j5 -= 5.0f;
		r.ry_j6 += 5.0f;
		r.ry_j7 -= 5.0f;
		r.ry_j8 += 5.0f;

		r.rl_j5 = 0.0f;
		r.rl_j6 = -10.0f;
		r.rl_j7 = 0.0f;
		r.rl_j8 = 0.0f;

		middle = false;
	}

	else {
		translate_y -= 0.05f;

		r.rz_j2 = 7.5f;
		r.rz_j3 = 7.5f;
		r.rz_j4 = 7.5f;
		r.rz_j5 = 7.5f;


		r.ry_j1 += 5.0f;
		r.ry_j2 -= 5.0f;
		r.ry_j3 += 5.0f;
		r.ry_j4 -= 5.0f;

		r.rl_j1 = -20.0f;
		r.rl_j2 = 5.0f;
		r.rl_j3 = 5.0f;
		r.rl_j4 = 10.0f;

		r.ry_j5 -= 5.0f;
		r.ry_j6 += 5.0f;
		r.ry_j7 -= 5.0f;
		r.ry_j8 += 5.0f;

		r.rl_j5 = -20.0f;
		r.rl_j6 = -5.0f;
		r.rl_j7 = -5.0f;
		r.rl_j8 = 10.0f;

		backwards = true;
	}

}

void boundary_walk(GLfloat& translate_x, GLfloat& translate_z, float& rotate_body, bool turn) {

	if (translate_x < 12.0f && translate_z < 12.0f && translate_x > -12.0f && translate_z > -12.0f) {

		increment_x = 0.5f * sin(glm::radians(rotate_body));
		increment_z = 0.5f * cos(glm::radians(rotate_body));

		translate_x += increment_x;
		translate_z += increment_z;

	}

	else {

		if (turn) {
			rotate_body += 5.0f;
		}

		else {
			rotate_body -= 5.0f;
		}

		increment_x = 0.5f * sin(glm::radians(rotate_body));
		increment_z = 0.5f * cos(glm::radians(rotate_body));

		translate_x += increment_x;
		translate_z += increment_z;

	}

}

void avoid_spider(GLfloat tx1, GLfloat tz1, GLfloat& tx2, GLfloat& tz2, GLfloat& rb2) {

	if (rb2 >= 360.0f) {
		rb2 -= 360.0f;
	}

	GLfloat x_diff = tx2 - tx1;
	GLfloat sq_diff_x = x_diff * x_diff;

	GLfloat z_diff = tz2 - tz1;
	GLfloat sq_diff_z = z_diff * z_diff;

	GLfloat d = sqrt(sq_diff_x + sq_diff_z);

	GLfloat x_hold = tx2;
	GLfloat z_hold = tz2;
	GLfloat d_hold;

	if (d <= 10.0f) {

		increment_x = 1.0f * sin(glm::radians(rb2));
		increment_z = 1.0f * cos(glm::radians(rb2));

		x_hold = x_hold + increment_x;
		z_hold = z_hold + increment_z;

		d_hold = sqrt(((x_hold - tx1) * (x_hold - tx1)) + ((z_hold - tz1) * (z_hold - tz1)));

		if (d_hold > d) {
			tx2 += increment_x;
			tz2 += increment_z;

		}

		else {
			tx2 -= increment_x;
			tz2 -= increment_z;

			rb2 -= 20.0f;
		}	

	}

	else {

		if (tx2 < 12.0f && tz2 < 12.0f && tx2 > -12.0f && tz2 > -12.0f) {

			rb2 -= 5.0f;

			increment_x = 0.5f * sin(glm::radians(rb2));
			increment_z = 0.5f * cos(glm::radians(rb2));

			tx2 += increment_x;
			tz2 += increment_z;

		}

		else {

			rb2 -= 5.0f;
		
			increment_x = 0.5f * sin(glm::radians(rb2));
			increment_z = 0.5f * cos(glm::radians(rb2));

			tx2 += increment_x;
			tz2 += increment_z;

		}

	}

}

void avoid_fire(GLfloat& tx2, GLfloat& tz2, GLfloat& rb2) {

	GLfloat d_fire = sqrt((tx2 * tx2) + (tz2 * tz2));

	GLfloat x_hold = tx2;
	GLfloat z_hold = tz2;
	GLfloat d_hold;

	if (d_fire <= 5.0f) {

		increment_x = 1.0f * sin(glm::radians(rb2));
		increment_z = 1.0f * cos(glm::radians(rb2));

		tx2 = tx2 - increment_x;
		tz2 = tz2 - increment_z;

		rb2 -= 45.0f;

	}
}

void walk_spider (GLfloat& tx2, GLfloat& tz2, GLfloat rb2) {

	increment_x = 0.75f * sin(glm::radians(rb2));
	increment_z = 0.75f * cos(glm::radians(rb2));

	tx2 += increment_x;
	tz2 += increment_z;
}

