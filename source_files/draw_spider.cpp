#pragma warning(disable : 5208)
#define NOMINMAX

#include <limits>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
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
#include "draw_spider.h"
#include "structs.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image.h"

void draw_spider(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rb, scale_tree s, unsigned int tex, int texnum, int texnum_loc, int matrix_location, vertex_array_object vao, rotation r, mesh m) {

	// joint 0
	glm::mat4 joint = glm::mat4(1.0f);
	glm::mat4 Rj = glm::mat4(1.0f);
	glm::mat4 Sj = glm::mat4(1.0f);
	glm::mat4 T_origin = glm::mat4(1.0f);
	glm::mat4 Tj = glm::mat4(1.0f);
	Sj = glm::scale(glm::mat4(1.0f), glm::vec3(s.scale_x, s.scale_y, s.scale_z));
	Rj = glm::rotate(glm::mat4(1.0f), rb, glm::vec3(0.0f, 1.0f, 0.0f));
	//T_origin = glm::translate(glm::mat4(1.0f), glm::vec3(-5.8f, 1.22f, -0.76f));
	Tj = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
	joint = Tj * T_origin * Rj * Sj;

	glm::mat4 origin = joint;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(origin));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//body of spider (root)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.8f, -1.22f, 0.76f));
	model = joint * model;

	glm::mat4 global = model;
	
	if (texnum == 6) {
		glUniform1i(texnum_loc, 6);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global));

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, tex);

		glBindVertexArray(vao.b);
		glDrawArrays(GL_TRIANGLES, 0, m.body.mPointCount);
		glBindVertexArray(0);

	}
	
	else {
		glUniform1i(texnum_loc, 7);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global));

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, tex);

		glBindVertexArray(vao.b);
		glDrawArrays(GL_TRIANGLES, 0, m.body.mPointCount);
		glBindVertexArray(0);
	}

#pragma region LEFT LEGS
	//joint 1
	glm::mat4 joint1 = glm::mat4(1.0f);
	glm::mat4 Rj1 = glm::mat4(1.0f);
	glm::mat4 Tj1 = glm::mat4(1.0f);
	Rj1 = glm::rotate(glm::mat4(1.0f), r.ry_j1, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj1 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.03f, 0.83f, 0.7f));
	joint1 = Tj1 * Rj1;

	glm::mat4 global0 = model * joint1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global0));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);;

	//leg 1
	glm::mat4 leg1 = glm::mat4(1.0f);
	leg1 = glm::translate(glm::mat4(1.0f), glm::vec3(5.03f, -0.83f, -0.7f));

	glm::mat4 global1 = model * joint1 * leg1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global1));
	glBindVertexArray(vao.ul1);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_1.mPointCount);

	//lower joint 1
	glm::mat4 joint1l = glm::mat4(1.0f);
	glm::mat4 Rj1l = glm::mat4(1.0f);
	glm::mat4 Tj1l = glm::mat4(1.0f);
	Rj1l = glm::rotate(glm::mat4(1.0f), r.rl_j1, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj1l = glm::translate(glm::mat4(1.0f), glm::vec3(-3.04f, 2.27f, 3.19f));
	joint1l = Tj1l * Rj1l;

	glm::mat4 global0l = model * joint1 * leg1 * joint1l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global0l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//lower leg 1
	glm::mat4 leg1l = glm::mat4(1.0f);
	leg1l = glm::translate(glm::mat4(1.0f), glm::vec3(3.04f, -2.27f, -3.19f));

	glm::mat4 global1l = model * joint1 * leg1 * joint1l * leg1l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global1l));
	glBindVertexArray(vao.ll1);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_1l.mPointCount);

	/*********************************************************************************************/

	//joint 2
	glm::mat4 joint2 = glm::mat4(1.0f);
	glm::mat4 Rj2 = glm::mat4(1.0f);
	glm::mat4 Ry2 = glm::mat4(1.0f);
	glm::mat4 Tj2 = glm::mat4(1.0f);
	Ry2 = glm::rotate(glm::mat4(1.0f), r.rz_j2, glm::vec3(0.0f, 1.0f, 0.0f));
	Rj2 = glm::rotate(glm::mat4(1.0f), r.ry_j2, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj2 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.03f, 0.83f, 0.29f));
	joint2 = Tj2 * Rj2 * Ry2;

	glm::mat4 global2 = model * joint2;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global2));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 2
	glm::mat4 leg2 = glm::mat4(1.0f);
	leg2 = glm::translate(glm::mat4(1.0f), glm::vec3(5.03f, -0.83f, -0.29f));

	glm::mat4 global3 = model * joint2 * leg2;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global3));
	glBindVertexArray(vao.ul2);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_2.mPointCount);

	//lower joint 2
	glm::mat4 joint2l = glm::mat4(1.0f);
	glm::mat4 Rj2l = glm::mat4(1.0f);
	glm::mat4 Tj2l = glm::mat4(1.0f);
	Rj2l = glm::rotate(glm::mat4(1.0f), r.rl_j2, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj2l = glm::translate(glm::mat4(1.0f), glm::vec3(-1.91f, 2.36f, 0.9f));
	joint2l = Tj2l * Rj2l;

	glm::mat4 global2l = model * joint2 * leg2 * joint2l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global2l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//lower leg 2
	glm::mat4 leg2l = glm::mat4(1.0f);
	leg2l = glm::translate(glm::mat4(1.0f), glm::vec3(1.91f, -2.36f, -0.9f));

	glm::mat4 global3l = model * joint2 * leg2 * joint2l * leg2l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global3l));
	glBindVertexArray(vao.ll2);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_2l.mPointCount);

	/*********************************************************************************************/

	//joint 3
	glm::mat4 joint3 = glm::mat4(1.0f);
	glm::mat4 Rj3 = glm::mat4(1.0f);
	glm::mat4 Ry3 = glm::mat4(1.0f);
	glm::mat4 Tj3 = glm::mat4(1.0f);
	Ry3 = glm::rotate(glm::mat4(1.0f), r.rz_j3, glm::vec3(0.0f, 1.0f, 0.0f));
	Rj3 = glm::rotate(glm::mat4(1.0f), r.ry_j3, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj3 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.07f, 0.88f, -0.13f));
	joint3 = Tj3 * Rj3 * Ry3;

	glm::mat4 global4 = model * joint3;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global4));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 3
	glm::mat4 leg3 = glm::mat4(1.0f);
	leg3 = glm::translate(glm::mat4(1.0f), glm::vec3(5.07f, -0.88f, 0.13f));

	glm::mat4 global5 = model * joint3 * leg3;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global5));
	glBindVertexArray(vao.ul3);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_3.mPointCount);

	// lower joint 3
	glm::mat4 joint3l = glm::mat4(1.0f);
	glm::mat4 Rj3l = glm::mat4(1.0f);
	glm::mat4 Tj3l = glm::mat4(1.0f);
	Rj3l = glm::rotate(glm::mat4(1.0f), r.rl_j3, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj3l = glm::translate(glm::mat4(1.0f), glm::vec3(-1.99f, 2.36f, -0.88f));
	joint3l = Tj3l * Rj3l;

	glm::mat4 global4l = model * joint3 * leg3 * joint3l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global4l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 3
	glm::mat4 leg3l = glm::mat4(1.0f);
	leg3l = glm::translate(glm::mat4(1.0f), glm::vec3(1.99f, -2.36f, 0.88f));

	glm::mat4 global5l = model * joint3 * leg3 * joint3l * leg3l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global5l));
	glBindVertexArray(vao.ll3);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_3l.mPointCount);

	/*********************************************************************************************/

	//joint 4
	glm::mat4 joint4 = glm::mat4(1.0f);
	glm::mat4 Rj4 = glm::mat4(1.0f);
	glm::mat4 Tj4 = glm::mat4(1.0f);
	Rj4 = glm::rotate(glm::mat4(1.0f), r.ry_j4, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj4 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.21f, 0.88f, -0.52f));
	joint4 = Tj4 * Rj4;

	glm::mat4 global6 = model * joint4;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global6));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 4
	glm::mat4 leg4 = glm::mat4(1.0f);
	leg4 = glm::translate(glm::mat4(1.0f), glm::vec3(5.21f, -0.88f, 0.52f));

	glm::mat4 global7 = model * joint4 * leg4;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global7));
	glBindVertexArray(vao.ul4);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_4.mPointCount);

	// lower joint 4
	glm::mat4 joint4l = glm::mat4(1.0f);
	glm::mat4 Rj4l = glm::mat4(1.0f);
	glm::mat4 Tj4l = glm::mat4(1.0f);
	Rj4l = glm::rotate(glm::mat4(1.0f), r.rl_j4, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj4l = glm::translate(glm::mat4(1.0f), glm::vec3(-2.57f, 2.36f, -2.27f));
	joint4l = Tj4l * Rj4l;

	glm::mat4 global6l = model * joint4 * leg4 * joint4l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global6l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 4
	glm::mat4 leg4l = glm::mat4(1.0f);
	leg4l = glm::translate(glm::mat4(1.0f), glm::vec3(2.57f, -2.36f, 2.27f));

	glm::mat4 global7l = model * joint4 * leg4 * joint4l * leg4l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global7l));
	glBindVertexArray(vao.ll4);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_4l.mPointCount);
#pragma endregion LEFT LEGS

#pragma region RIGHT LEGS
	//joint 5
	glm::mat4 joint5 = glm::mat4(1.0f);
	glm::mat4 Rj5 = glm::mat4(1.0f);
	glm::mat4 Tj5 = glm::mat4(1.0f);
	Rj5 = glm::rotate(glm::mat4(1.0f), r.ry_j5, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj5 = glm::translate(glm::mat4(1.0f), glm::vec3(-6.6f, 0.82f, 0.67f));
	joint5 = Tj5 * Rj5;

	glm::mat4 global8 = model * joint5;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global8));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 5
	glm::mat4 leg5 = glm::mat4(1.0f);
	leg5 = glm::translate(glm::mat4(1.0f), glm::vec3(6.6f, -0.82f, -0.67f));

	glm::mat4 global9 = model * joint5 * leg5;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global9));
	glBindVertexArray(vao.ur1);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_5.mPointCount);

	// lower joint 5
	glm::mat4 joint5l = glm::mat4(1.0f);
	glm::mat4 Rj5l = glm::mat4(1.0f);
	glm::mat4 Tj5l = glm::mat4(1.0f);
	Rj5l = glm::rotate(glm::mat4(1.0f), r.rl_j5, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj5l = glm::translate(glm::mat4(1.0f), glm::vec3(-8.75f, 2.3f, 2.92f));
	joint5l = Tj5l * Rj5l;

	glm::mat4 global8l = model * joint5 * leg5 * joint5l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global8l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 5
	glm::mat4 leg5l = glm::mat4(1.0f);
	leg5l = glm::translate(glm::mat4(1.0f), glm::vec3(8.75f, -2.3f, -2.92f));

	glm::mat4 global9l = model * joint5 * leg5 * joint5l * leg5l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global9l));
	glBindVertexArray(vao.lr1);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_5l.mPointCount);

	/*********************************************************************************************/

	//joint 6
	glm::mat4 joint6 = glm::mat4(1.0f);
	glm::mat4 Rj6 = glm::mat4(1.0f);
	glm::mat4 Ry6 = glm::mat4(1.0f);
	glm::mat4 Tj6 = glm::mat4(1.0f);
	Ry6 = glm::rotate(glm::mat4(1.0f), r.rz_j4, glm::vec3(0.0f, 1.0f, 0.0f));
	Rj6 = glm::rotate(glm::mat4(1.0f), r.ry_j6, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj6 = glm::translate(glm::mat4(1.0f), glm::vec3(-6.57f, 0.84f, 0.25f));
	joint6 = Tj6 * Rj6 * Ry6;

	glm::mat4 global10 = model * joint6;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global10));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 6
	glm::mat4 leg6 = glm::mat4(1.0f);
	leg6 = glm::translate(glm::mat4(1.0f), glm::vec3(6.57f, -0.84f, -0.25f));

	glm::mat4 global11 = model * joint6 * leg6;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global11));
	glBindVertexArray(vao.ur2);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_6.mPointCount);

	// lower joint 6
	glm::mat4 joint6l = glm::mat4(1.0f);
	glm::mat4 Rj6l = glm::mat4(1.0f);
	glm::mat4 Tj6l = glm::mat4(1.0f);
	Rj6l = glm::rotate(glm::mat4(1.0f), r.rl_j6, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj6l = glm::translate(glm::mat4(1.0f), glm::vec3(-9.69f, 2.35f, 0.88f));
	joint6l = Tj6l * Rj6l;

	glm::mat4 global10l = model * joint6 * leg6 * joint6l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global10l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 6
	glm::mat4 leg6l = glm::mat4(1.0f);
	leg6l = glm::translate(glm::mat4(1.0f), glm::vec3(9.69f, -2.35f, -0.88f));

	glm::mat4 global11l = model * joint6 * leg6 * joint6l * leg6l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global11l));
	glBindVertexArray(vao.lr2);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_6l.mPointCount);

	/*********************************************************************************************/

	//joint 7
	glm::mat4 joint7 = glm::mat4(1.0f);
	glm::mat4 Rj7 = glm::mat4(1.0f);
	glm::mat4 Ry7 = glm::mat4(1.0f);
	glm::mat4 Tj7 = glm::mat4(1.0f);
	Ry7 = glm::rotate(glm::mat4(1.0f), r.rz_j5, glm::vec3(0.0f, 1.0f, 0.0f));
	Rj7 = glm::rotate(glm::mat4(1.0f), r.ry_j7, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj7 = glm::translate(glm::mat4(1.0f), glm::vec3(-6.52f, 0.87f, -0.11f));
	joint7 = Tj7 * Rj7 * Ry7;

	glm::mat4 global12 = model * joint7;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global12));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 7
	glm::mat4 leg7 = glm::mat4(1.0f);
	leg7 = glm::translate(glm::mat4(1.0f), glm::vec3(6.52f, -0.87f, 0.11f));

	glm::mat4 global13 = model * joint7 * leg7;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global13));
	glBindVertexArray(vao.ur3);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_7.mPointCount);

	// lower joint 7
	glm::mat4 joint7l = glm::mat4(1.0f);
	glm::mat4 Rj7l = glm::mat4(1.0f);
	glm::mat4 Tj7l = glm::mat4(1.0f);
	Rj7l = glm::rotate(glm::mat4(1.0f), r.rl_j7, glm::vec3(0.0f, 0.0f, 1.0f));
	Tj7l = glm::translate(glm::mat4(1.0f), glm::vec3(-9.56f, 2.36f, -0.86f));
	joint7l = Tj7l * Rj7l;

	glm::mat4 global12l = model * joint7 * leg7 * joint7l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global12l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 7
	glm::mat4 leg7l = glm::mat4(1.0f);
	leg7l = glm::translate(glm::mat4(1.0f), glm::vec3(9.56f, -2.36f, 0.86f));

	glm::mat4 global13l = model * joint7 * leg7 * joint7l * leg7l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global13l));
	glBindVertexArray(vao.lr3);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_7l.mPointCount);

	/*********************************************************************************************/

	//joint 8
	glm::mat4 joint8 = glm::mat4(1.0f);
	glm::mat4 Rj8 = glm::mat4(1.0f);
	glm::mat4 Tj8 = glm::mat4(1.0f);
	Rj8 = glm::rotate(glm::mat4(1.0f), r.ry_j8, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj8 = glm::translate(glm::mat4(1.0f), glm::vec3(-6.38f, 0.91f, -0.44f));
	joint8 = Tj8 * Rj8;

	glm::mat4 global14 = model * joint8;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global14));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	//leg 8
	glm::mat4 leg8 = glm::mat4(1.0f);
	leg8 = glm::translate(glm::mat4(1.0f), glm::vec3(6.38f, -0.91f, 0.44f));

	glm::mat4 global15 = model * joint8 * leg8;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global15));
	glBindVertexArray(vao.ur4);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_8.mPointCount);

	// lower joint 8
	glm::mat4 joint8l = glm::mat4(1.0f);
	glm::mat4 Rj8l = glm::mat4(1.0f);
	glm::mat4 Tj8l = glm::mat4(1.0f);
	Rj8l = glm::rotate(glm::mat4(1.0f), r.rl_j8, glm::vec3(1.0f, 0.0f, 0.0f));
	Tj8l = glm::translate(glm::mat4(1.0f), glm::vec3(-8.99f, 2.36f, -2.19f));
	joint8l = Tj8l * Rj8l;

	glm::mat4 global14l = model * joint8 * leg8 * joint8l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global14l));
	glBindVertexArray(vao.j);
	glDrawArrays(GL_TRIANGLES, 0, m.joint.mPointCount);

	// lower leg 8
	glm::mat4 leg8l = glm::mat4(1.0f);
	leg8l = glm::translate(glm::mat4(1.0f), glm::vec3(8.99f, -2.36f, 2.19f));

	glm::mat4 global15l = model * joint8 * leg8 * joint8l * leg8l;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(global15l));
	glBindVertexArray(vao.lr4);
	glDrawArrays(GL_TRIANGLES, 0, m.leg_8l.mPointCount);
#pragma endregion RIGHT LEGS

}

void draw_tree(GLfloat tx, GLfloat ty, GLfloat tz, scale_tree s, unsigned int texture1, unsigned int texture2, int texnum_loc, int matrix_location, vertex_array_object vao, mesh m) {

	glm::mat4 tree_trunk = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 S1 = glm::mat4(1.0f);
	S1 = glm::scale(glm::mat4(1.0f), glm::vec3(s.scale_x, s.scale_y, s.scale_z));
	T = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
	tree_trunk = tree_trunk * S1 * T;

	glUniform1i(texnum_loc, 1);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(tree_trunk));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glBindVertexArray(vao.t);
	glDrawArrays(GL_TRIANGLES, 0, m.trunk.mPointCount);
	//glBindVertexArray(0);

	glm::mat4 tree_foliage = glm::mat4(1.0f);
	glm::mat4 tree = tree_trunk * tree_foliage;

	glUniform1i(texnum_loc, 2);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(tree));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glBindVertexArray(vao.l);
	glDrawArrays(GL_TRIANGLES, 0, m.leaves.mPointCount);
	//glBindVertexArray(0);
}