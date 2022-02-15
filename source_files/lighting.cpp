#pragma warning(disable : 5208)
#define NOMINMAX

#include <limits>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> 

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 

// Project includes
#include "maths_funcs.h"
#include "draw_spider.h"
#include "structs.h"
#include "lighting.h"

// GLM includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

glm::vec3 spider_ambient = glm::vec3(0.5, 0.5, 0.5);
glm::vec3 spider_diffuse = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 spider_specular = glm::vec3(1.0, 1.0, 1.0);
float spider_exponent = 128.0f;

glm::vec3 moon_ambient = glm::vec3(0.8, 0.8, 0.8);
glm::vec3 moon_diffuse = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 moon_specular = glm::vec3(1.0, 1.0, 1.0);
float moon_exponent = 16.0f;

glm::vec3 fire_ambient = glm::vec3(0.8f, 0.2f, 0.02f);
glm::vec3 fire_diffuse = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 fire_specular = glm::vec3(1.0, 1.0, 1.0);
float fire_exponent = 8.0f;

void spider_lighting(int programID, glm::mat4 view) {

	//lighting for spider
	int light_pos_location = glGetUniformLocation(programID, "light_position");
	int spider_color_location = glGetUniformLocation(programID, "spider_color");
	int view_pos_location = glGetUniformLocation(programID, "view_position");

	int sp_ambient_loc = glGetUniformLocation(programID, "spider.ambient");
	int sp_diffuse_loc = glGetUniformLocation(programID, "spider.diffuse");
	int sp_specular_loc = glGetUniformLocation(programID, "spider.specular");
	int sp_spec_loc = glGetUniformLocation(programID, "spider.shininess");

	glm::vec3 light_pos = glm::vec3(-5.0f, 16.0f, 0.0f);
	glm::vec3 sp_color = glm::vec3(0.25, 0.25, 0.25);

	glUniform3fv(light_pos_location, 1, glm::value_ptr(light_pos));
	glUniform3fv(spider_color_location, 1, glm::value_ptr(sp_color));
	glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, glm::value_ptr(view));

	glUniform3fv(sp_ambient_loc, 1, glm::value_ptr(spider_ambient));
	glUniform3fv(sp_diffuse_loc, 1, glm::value_ptr(spider_diffuse));
	glUniform3fv(sp_specular_loc, 1, glm::value_ptr(spider_specular));
	glUniform1f(sp_spec_loc, spider_exponent);
}

void env_lighting(int programID, glm::mat4 view) {

	//lighting for environment
	int light_pos_location = glGetUniformLocation(programID, "light_position");
	int spider_color_location = glGetUniformLocation(programID, "env_color");
	int view_pos_location = glGetUniformLocation(programID, "view_position");

	int sp_ambient_loc = glGetUniformLocation(programID, "env.ambient");
	int sp_diffuse_loc = glGetUniformLocation(programID, "env.diffuse");
	int sp_specular_loc = glGetUniformLocation(programID, "env.specular");
	int sp_spec_loc = glGetUniformLocation(programID, "env.shininess");

	glm::vec3 light_pos = glm::vec3(-5.0f, 30.0f, 0.0f);
	glm::vec3 sp_color = glm::vec3(0.9, 0.9, 0.9);

	glUniform3fv(light_pos_location, 1, glm::value_ptr(light_pos));
	glUniform3fv(spider_color_location, 1, glm::value_ptr(sp_color));
	glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, glm::value_ptr(view));

	glUniform3fv(sp_ambient_loc, 1, glm::value_ptr(moon_ambient));
	glUniform3fv(sp_diffuse_loc, 1, glm::value_ptr(moon_diffuse));
	glUniform3fv(sp_specular_loc, 1, glm::value_ptr(moon_specular));
	glUniform1f(sp_spec_loc, moon_exponent);
}

void fire_lighting(int programID, glm::mat4 view) {

	//lighting for environment
	int light_pos_location = glGetUniformLocation(programID, "fire_position");
	int spider_color_location = glGetUniformLocation(programID, "fire_color");
	int view_pos_location = glGetUniformLocation(programID, "view_position");

	int sp_ambient_loc = glGetUniformLocation(programID, "fire.ambient");
	int sp_diffuse_loc = glGetUniformLocation(programID, "fire.diffuse");
	int sp_specular_loc = glGetUniformLocation(programID, "fire.specular");
	int sp_spec_loc = glGetUniformLocation(programID, "fire.shininess");

	glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sp_color = glm::vec3(1.0f, 0.25f, 0.02f);

	glm::vec3 lightColor;
	lightColor.x = 0.7f;
	lightColor.y = sin(timeGetTime() * 0.9f);
	lightColor.z = sin(timeGetTime() * 0.02f);

	fire_diffuse = lightColor;
	
	glUniform3fv(light_pos_location, 1, glm::value_ptr(light_pos));
	glUniform3fv(spider_color_location, 1, glm::value_ptr(sp_color));
	glUniformMatrix4fv(view_pos_location, 1, GL_FALSE, glm::value_ptr(view));

	glUniform3fv(sp_ambient_loc, 1, glm::value_ptr(fire_ambient));
	glUniform3fv(sp_diffuse_loc, 1, glm::value_ptr(fire_diffuse));
	glUniform3fv(sp_specular_loc, 1, glm::value_ptr(fire_specular));
	glUniform1f(sp_spec_loc, fire_exponent);
}

void change_light(unsigned char key, int x, int y) {
	/*switch (key) {
	case 'r':
		spider_diffuse += 0.1f;
		break;
	case 't':
		spider_diffuse -= 0.1f;
		break;
	case 'y':
		moon_diffuse += 0.1f;
		break;
	case 'f':
		moon_diffuse -= 0.1f;
		break;
	case 'g':
		fire_diffuse += 0.1f;
		break;
	case 'h':
		fire_diffuse -= 0.1f;
		break;
	case 'c':
		spider_exponent += 5.0f;
		break;
	case 'v':
		fire_exponent += 5.0f;
		break;
	case 'b':
		moon_exponent += 5.0f;
		break;
	}*/
}