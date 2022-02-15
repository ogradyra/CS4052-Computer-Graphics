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
#include "interactive_camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

glm::vec3 cameraPos = glm::vec3(0.0f, 15.0f, 58.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -15.0f, -30.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int projType = 0;

bool firstMouse = true;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

int w = 1000;
int h = 800;

void keypress(unsigned char key, int x, int y) {

	switch (key) {

	case 'p':
		projType = 0;
		break;
	case 'o':
		projType = 1;
		break;


	case 'z':
		cameraPos += glm::vec3(0.0f, 0.0f, 2.0f);
		break;
	case 'x':
		cameraPos -= glm::vec3(0.0f, 0.0f, 2.0f);
		break;
	case 'w':
		cameraPos += glm::vec3(0.0f, 2.0f, 0.0f);
		break;
	case 's':
		cameraPos -= glm::vec3(0.0f, 2.0f, 0.0f);
		break;
	case 'a':
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case 'd':
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	}

	glutPostRedisplay();
}

void mouseclick(int xpos, int ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void view_proj_matrix(glm::mat4 &persp_proj, glm::mat4 &view) {

	//setting up projection matrix
	if (projType == 0) {
		persp_proj = glm::perspective(45.0f, (float)w / (float)h, 1.0f, 100.0f);
	}

	else if (projType == 1) {
		persp_proj = glm::ortho(-16.0f, 16.0f, -12.0f, 12.0f, 1.0f, 100.0f);
	}

	//setting up camera
	//lookAt(position, target, up vector);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}