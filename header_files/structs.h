#pragma once
#include "maths_funcs.h"

// GLM includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

typedef struct {
	ModelData ground, body, joint, trunk, leaves, moon, fire, spider, leg_1, leg_1l, leg_2, leg_2l, leg_3, leg_3l, leg_4, leg_4l,
		leg_5, leg_5l, leg_6, leg_6l, leg_7, leg_7l, leg_8, leg_8l;
} mesh;

typedef struct {
	GLuint b, j, t, l, mn, fr, sky, spd, ul1, ul2, ul3, ul4, ur1, ur2, ur3, ur4, ll1, ll2, ll3, ll4, lr1, lr2, lr3, lr4;
} vertex_array_object;

typedef struct {
	GLfloat scale_x = 1.0f;
	GLfloat scale_y = 1.0f;
	GLfloat	scale_z = 1.0f;
} scale_tree;

typedef struct {

	GLfloat ry_j1 = 10.0f;
	GLfloat ry_j2 = 0.0f;
	GLfloat ry_j3 = 10.0f;
	GLfloat ry_j4 = 0.0f;

	GLfloat rz_j2 = 0.0f;
	GLfloat rz_j3 = -10.0f;
	GLfloat rz_j4 = -10.0f;
	GLfloat rz_j5 = 10.0f;

	GLfloat ry_j5 = 0.0f;
	GLfloat ry_j6 = 0.0f;
	GLfloat ry_j7 = 0.0f;
	GLfloat ry_j8 = 0.0f;

	GLfloat rl_j1 = 0.0f;
	GLfloat rl_j2 = 0.0f;
	GLfloat rl_j3 = 0.0f;
	GLfloat rl_j4 = 0.0f;

	GLfloat rl_j5 = 0.0f;
	GLfloat rl_j6 = 0.0f;
	GLfloat rl_j7 = 0.0f;
	GLfloat rl_j8 = 0.0f;

}rotation;

typedef struct {
	GLfloat tx, ty, tz;
	GLfloat rotate_body;
}spider_data;

typedef struct Particle{
	glm::vec3 position, velocity;
	glm::vec4 color;
	float life;

	Particle()
		: position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};