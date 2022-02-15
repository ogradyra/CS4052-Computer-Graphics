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
#include "animation.h"
#include "lighting.h"
#include "skybox.h"

// GLM includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Meshes to load
#define GROUND "C:/Users/aogra/Desktop/mtass/mtass/ground.obj"
#define TRUNK "C:/Users/aogra/Desktop/mtass/mtass/trunk.obj"
#define LEAVES "C:/Users/aogra/Desktop/mtass/mtass/leaves.obj"
#define MOON "C:/Users/aogra/Desktop/mtass/mtass/moon.obj"
#define FIRE "C:/Users/aogra/Desktop/mtass/mtass/fire.obj"
#define MOUNTAIN "C:/Users/aogra/Desktop/mtass/mtass/mountain.obj"

//spider body
#define SPIDER "C:/Users/aogra/Desktop/mtass/mtass/spiders.obj"
#define BODY "C:/Users/aogra/Desktop/mtass/mtass/spider_body.obj"
#define JOINT "C:/Users/aogra/Desktop/mtass/mtass/small_joint.obj"

//upper legs
#define LEFT_LEG_1 "C:/Users/aogra/Desktop/mtass/mtass/L1u.obj"
#define LEFT_LEG_2 "C:/Users/aogra/Desktop/mtass/mtass/L2u.obj"
#define LEFT_LEG_3 "C:/Users/aogra/Desktop/mtass/mtass/L3u.obj"
#define LEFT_LEG_4 "C:/Users/aogra/Desktop/mtass/mtass/L4u.obj"
#define RIGHT_LEG_1 "C:/Users/aogra/Desktop/mtass/mtass/R1u.obj"
#define RIGHT_LEG_2 "C:/Users/aogra/Desktop/mtass/mtass/R2u.obj"
#define RIGHT_LEG_3 "C:/Users/aogra/Desktop/mtass/mtass/R3u.obj"
#define RIGHT_LEG_4 "C:/Users/aogra/Desktop/mtass/mtass/R4u.obj"

//lower legs
#define LEFT_LEG_1l "C:/Users/aogra/Desktop/mtass/mtass/L1l.obj"
#define LEFT_LEG_2l "C:/Users/aogra/Desktop/mtass/mtass/L2l.obj"
#define LEFT_LEG_3l "C:/Users/aogra/Desktop/mtass/mtass/L3l.obj"
#define LEFT_LEG_4l "C:/Users/aogra/Desktop/mtass/mtass/L4l.obj"
#define RIGHT_LEG_1l "C:/Users/aogra/Desktop/mtass/mtass/R1l.obj"
#define RIGHT_LEG_2l "C:/Users/aogra/Desktop/mtass/mtass/R2l.obj"
#define RIGHT_LEG_3l "C:/Users/aogra/Desktop/mtass/mtass/R3l.obj"
#define RIGHT_LEG_4l "C:/Users/aogra/Desktop/mtass/mtass/R4l.obj"

// Image includes
#include "stb_image.h"

using namespace std;
using std::vector;

//structs
mesh m;
scale_tree s;
rotation r, r_spider_game;
vertex_array_object vao;

spider_data sp1, sp2, sp3, sp4, sp5, sp6, sp7, sp8, sp9, sp10, sp11, sp12;

GLfloat translate_x = 0.0f;
GLfloat translate_y_sg = 0.0f;
GLfloat translate_y = 0.5f;
GLfloat translate_z = 25.0f;
float rotate_body = 180.0f;

unsigned int textures[8];
GLuint skybox_vao, skybox_vbo, floor_vao, particle_vao, particle_vbo;
GLuint loc1, loc2, loc3;
GLuint programID, floor_programID, sky_programID, light_programID, particleID;

int width = 800;
int height = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 23.0f, 56.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -15.0f, -30.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int projType = 0;

bool firstMouse = true;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

unsigned int nr_new_particles = 2;
float dt = 0.25;
glm::vec3 offset = glm::vec3(0.0, 0.5, 0.0);
unsigned int lastUsedParticle = 0;
unsigned int nr_particles = 10;
std::vector<Particle> particles;
Particle particle;

bool turn;

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;

		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));

			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));

			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

#pragma region READ SHADER
char* parseShader(const char* shaderFile) {
	FILE* sh;
	fopen_s(&sh, shaderFile, "rb");

	if (sh == NULL) { return NULL; }

	fseek(sh, 0L, SEEK_END);
	long size = ftell(sh);

	fseek(sh, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, sh);
	buf[size] = '\0';

	fclose(sh);

	return buf;
}
#pragma endregion READ SHADER

#pragma region SHADER FCNS
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = parseShader(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint compileShaders(int shaderID) {
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	GLuint progID = glCreateProgram();
	if (progID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	if (shaderID == 0) {
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/vertexShader.txt", GL_VERTEX_SHADER);
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/fragmentShader.txt", GL_FRAGMENT_SHADER);
	}

	else if (shaderID == 1) {
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/floor_vs.txt", GL_VERTEX_SHADER);
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/floor_fs.txt", GL_FRAGMENT_SHADER);
	}

	else if (shaderID == 2) {
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/sky_vs.txt", GL_VERTEX_SHADER);
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/sky_fs.txt", GL_FRAGMENT_SHADER);
	}

	else if (shaderID == 3) {
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/light_source_vs.txt", GL_VERTEX_SHADER);
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/light_source_fs.txt", GL_FRAGMENT_SHADER);
	}

	else {
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/particle_vs.txt", GL_VERTEX_SHADER);
		AddShader(progID, "C:/Users/aogra/Desktop/mtass/mtass/particle_fs.txt", GL_FRAGMENT_SHADER);
	}

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(progID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(progID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(progID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(progID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(progID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(progID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(progID);
	return progID;
}
#pragma endregion SHADER FCNS

#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh1(GLuint vao, ModelData mesh_data, GLuint programID) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	unsigned int vp_vbo = 0;
	unsigned int vn_vbo = 0;
	unsigned int vt_vbo = 0;

	loc1 = glGetAttribLocation(programID, "vertex_position");
	loc2 = glGetAttribLocation(programID, "vertex_normal");
	loc3 = glGetAttribLocation(programID, "vertex_texture");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vt_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}
#pragma endregion VBO_FUNCTIONS

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 persp_proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f);
	glm::mat4 view = glm::mat4(1.0f);

	// draw spider

	glUseProgram(programID);

	int matrix_location = glGetUniformLocation(programID, "model");
	int view_mat_location = glGetUniformLocation(programID, "view");
	int proj_mat_location = glGetUniformLocation(programID, "proj");
	int tex_loc = glGetUniformLocation(programID, "tex_num");

	if (projType == 0) {
		persp_proj = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 100.0f);
	}

	else if (projType == 1) {
		persp_proj = glm::ortho(-16.0f, 16.0f, -12.0f, 12.0f, 1.0f, 100.0f);
	}

	//setting up camera
	//lookAt(position, target, up vector);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(persp_proj));
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));

	s.scale_x = 0.6f;
	s.scale_y = 0.6f;
	s.scale_z = 0.6f;

	draw_spider(sp1.tx, translate_y, sp1.tz, sp1.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp2.tx, translate_y, sp2.tz, sp2.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp3.tx, translate_y, sp3.tz, sp3.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);

	draw_spider(sp4.tx, translate_y, sp4.tz, sp4.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp5.tx, translate_y, sp5.tz, sp5.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp6.tx, translate_y, sp6.tz, sp6.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);

	draw_spider(sp7.tx, translate_y, sp7.tz, sp7.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp8.tx, translate_y, sp8.tz, sp8.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp9.tx, translate_y, sp9.tz, sp9.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);

	draw_spider(sp10.tx, translate_y, sp10.tz, sp10.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp11.tx, translate_y, sp11.tz, sp11.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);
	draw_spider(sp12.tx, translate_y, sp12.tz, sp12.rotate_body, s, textures[6], 6, tex_loc, matrix_location, vao, r, m);

	s.scale_x = 1.0f;
	s.scale_y = 1.0f;
	s.scale_z = 1.0f;

	draw_spider(translate_x, translate_y, translate_z, rotate_body, s, textures[7], 7, tex_loc, matrix_location, vao, r, m);

	fire_lighting(programID, view);
	spider_lighting(programID, view);


	// draw ground plane

	glUseProgram(floor_programID);

	glm::mat4 gnd = glm::mat4(1.0f);
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);
	S = glm::scale(glm::mat4(1.0f), glm::vec3(2.5, 1, 2.5));
	T = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 0));
	gnd = gnd * T * S;

	int model_loc = glGetUniformLocation(floor_programID, "model");
	int view_loc = glGetUniformLocation(floor_programID, "view");
	int proj_loc = glGetUniformLocation(floor_programID, "proj");
	int texnum_loc = glGetUniformLocation(floor_programID, "tex_num");

	glUniform1i(texnum_loc, 0);

	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(persp_proj));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(gnd));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glBindVertexArray(floor_vao);
	glDrawArrays(GL_TRIANGLES, 0, m.ground.mPointCount);
	glBindVertexArray(0);


	// draw trees

	s.scale_x = 1.0f;
	s.scale_y = 1.0f;
	s.scale_z = 1.0f;


	draw_tree(-12.0f, 0.0f, -24.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(-24.0f, 0.0f, -12.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(12.0f, 0.0f, -24.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(-24.0f, 0.0f, 12.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(24.0f, 0.0f, 12.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(24.0f, 0.0f, -12.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);

	s.scale_x = 1.5f;
	s.scale_y = 1.5f;
	s.scale_z = 1.5f;

	draw_tree(-24.0f, 0.0f, -24.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(24.0f, 0.0f, -24.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(-24.0f, 0.0f, 18.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(24.0f, 0.0f, 18.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(0.0f, 0.0f, -24.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(24.0f, 0.0f, 0.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);
	draw_tree(-24.0f, 0.0f, 0.0f, s, textures[1], textures[2], texnum_loc, model_loc, vao, m);

	env_lighting(floor_programID, view);
	fire_lighting(floor_programID, view);


	// draw light sources - fire and moon

	glUseProgram(light_programID);

	int light_loc = glGetUniformLocation(light_programID, "model");
	int light_proj = glGetUniformLocation(light_programID, "proj");
	int light_view = glGetUniformLocation(light_programID, "view");
	int light_colour_loc = glGetUniformLocation(light_programID, "lightColor");
	int texnum = glGetUniformLocation(light_programID, "tex_num");

	// moon 
	glm::vec3 moon_color = glm::vec3(0.9f, 0.9f, 0.9f);

	glUniformMatrix4fv(light_proj, 1, GL_FALSE, glm::value_ptr(persp_proj));
	glUniformMatrix4fv(light_view, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(light_colour_loc, 1, glm::value_ptr(moon_color));

	glm::mat4 moon_obj = glm::mat4(1.0f);
	glm::mat4 trans_moon = glm::mat4(1.0f);
	glm::mat4 scale_moon = glm::mat4(1.0f);
	trans_moon = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 16.0f, 0.0f));
	scale_moon = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	moon_obj = moon_obj * scale_moon * trans_moon;

	glUniform1i(texnum, 4);
	glUniformMatrix4fv(light_loc, 1, GL_FALSE, glm::value_ptr(moon_obj));

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[4]);

	glBindVertexArray(vao.mn);
	glDrawArrays(GL_TRIANGLES, 0, m.moon.mPointCount);
	glBindVertexArray(0);

	//fire
	glm::mat4 fire_obj = glm::mat4(1.0f);
	glm::mat4 trans_fire = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 scale_fire = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
	fire_obj = fire_obj * scale_fire * trans_fire;

	glm::vec3 lightColor;
	lightColor.y = sin(timeGetTime() * 0.9f);
	lightColor.z = sin(timeGetTime() * 0.02f);

	glm::vec3 fire_color = glm::vec3(0.7f, lightColor.y, lightColor.z);

	glUniform3fv(light_colour_loc, 1, glm::value_ptr(fire_color));

	glUniform1i(texnum, 5);
	glUniformMatrix4fv(light_loc, 1, GL_FALSE, glm::value_ptr(fire_obj));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[5]);

	glBindVertexArray(vao.fr);
	glDrawArrays(GL_TRIANGLES, 0, m.fire.mPointCount);
	glBindVertexArray(0);


	// draw skybox

	glDepthFunc(GL_LEQUAL);
	glUseProgram(sky_programID);

	int projSkyLoc = glGetUniformLocation(sky_programID, "proj");
	int viewSKyLoc = glGetUniformLocation(sky_programID, "view");

	glm::mat4 skybox_view = glm::mat4(glm::mat3(view));

	glUniformMatrix4fv(projSkyLoc, 1, GL_FALSE, glm::value_ptr(persp_proj));
	glUniformMatrix4fv(viewSKyLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));

	glBindVertexArray(skybox_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[3]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	glutSwapBuffers();
}

static void updateScene() {

	// animates spider legs
	animate(r, translate_y);

	// spider movement with collision detection

	// spider 1
	avoid_spider(translate_x, translate_z, sp1.tx, sp1.tz, sp1.rotate_body);
	avoid_fire(sp1.tx, sp1.tz, sp1.rotate_body);

	// spider 2
	avoid_spider(translate_x, translate_z, sp2.tx, sp2.tz, sp2.rotate_body);
	avoid_fire(sp2.tx, sp2.tz, sp2.rotate_body);

	// spider 3
	avoid_spider(translate_x, translate_z, sp3.tx, sp3.tz, sp3.rotate_body);
	avoid_fire(sp3.tx, sp3.tz, sp3.rotate_body);

	// spider 4
	avoid_spider(translate_x, translate_z, sp4.tx, sp4.tz, sp4.rotate_body);
	avoid_fire(sp4.tx, sp4.tz, sp4.rotate_body);

	// spider 5
	avoid_spider(translate_x, translate_z, sp5.tx, sp5.tz, sp5.rotate_body);
	avoid_fire(sp5.tx, sp5.tz, sp5.rotate_body);

	// spider 6
	avoid_spider(translate_x, translate_z, sp6.tx, sp6.tz, sp6.rotate_body);
	avoid_fire(sp6.tx, sp6.tz, sp6.rotate_body);

	// spider 7
	avoid_spider(translate_x, translate_z, sp7.tx, sp7.tz, sp7.rotate_body);
	avoid_fire(sp7.tx, sp7.tz, sp7.rotate_body);

	// spider 8
	avoid_spider(translate_x, translate_z, sp8.tx, sp8.tz, sp8.rotate_body);
	avoid_fire(sp8.tx, sp8.tz, sp8.rotate_body);

	// spider 9
	avoid_spider(translate_x, translate_z, sp9.tx, sp9.tz, sp9.rotate_body);
	avoid_fire(sp9.tx, sp9.tz, sp9.rotate_body);

	// spider 10
	avoid_spider(translate_x, translate_z, sp10.tx, sp10.tz, sp10.rotate_body);
	avoid_fire(sp10.tx, sp10.tz, sp10.rotate_body);

	// spider 11
	avoid_spider(translate_x, translate_z, sp11.tx, sp11.tz, sp11.rotate_body);
	avoid_fire(sp11.tx, sp11.tz, sp11.rotate_body);

	// spider 12
	avoid_spider(translate_x, translate_z, sp12.tx, sp12.tz, sp12.rotate_body);
	avoid_fire(sp12.tx, sp12.tz, sp12.rotate_body);

	glutPostRedisplay();
}

void keypress(unsigned char key, int x, int y) {

	switch (key) {

	case 'p':
		projType = 0;
		break;
	case 'o':
		projType = 1;
		break;


	case 'z':
		// move camera backwards
		cameraPos += glm::vec3(0.0f, 0.0f, 2.0f);
		break;
	case 'x':
		// move camera forewards
		cameraPos -= glm::vec3(0.0f, 0.0f, 2.0f);
		break;
	case 'w':
		// move camera upwards
		cameraPos += glm::vec3(0.0f, 2.0f, 0.0f);
		break;
	case 's':
		// move camera downwards
		cameraPos -= glm::vec3(0.0f, 2.0f, 0.0f);
		break;
	case 'a':
		// move camera left
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case 'd':
		// move camera right
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp));
		break;

		// chaser spider controls
	case 'i':
		// walk forwards
		walk_spider(translate_x, translate_z, rotate_body);
		break;
	case 'j':
		// turn left
		rotate_body += 10.0f;
		break;
	case 'l':
		// turn right
		rotate_body -= 10.0f;
		break;
	case 'k':
		// face opposite direction
		rotate_body += 180.0f;
		break;
	}

	glutPostRedisplay();
}

void mouseclick(int xpos, int ypos) {

	//button = GLUT_LEFT_BUTTON;
	//state = GLUT_DOWN;

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

void init() {

	// Set up the shaders
	programID = compileShaders(0);
	floor_programID = compileShaders(1);
	sky_programID = compileShaders(2);
	light_programID = compileShaders(3);
	particleID = compileShaders(4);

	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skybox_vao);
	glGenBuffers(1, &skybox_vbo);
	glBindVertexArray(skybox_vao);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	vector<const char*> faces;

	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");
	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");
	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");
	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");
	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");
	faces.push_back("C:/Users/aogra/Desktop/mtass/mtass/starry_sky.jpg");


	m.spider = load_mesh(SPIDER);
	glGenVertexArrays(1, &vao.spd);
	generateObjectBufferMesh1(vao.spd, m.spider, floor_programID);

	m.moon = load_mesh(MOON);
	glGenVertexArrays(1, &vao.mn);
	generateObjectBufferMesh1(vao.mn, m.moon, light_programID);

	m.fire = load_mesh(FIRE);
	glGenVertexArrays(1, &vao.fr);
	generateObjectBufferMesh1(vao.fr, m.fire, light_programID);

	m.trunk = load_mesh(TRUNK);
	glGenVertexArrays(1, &vao.t);
	generateObjectBufferMesh1(vao.t, m.trunk, floor_programID);

	m.leaves = load_mesh(LEAVES);
	glGenVertexArrays(1, &vao.l);
	generateObjectBufferMesh1(vao.l, m.leaves, floor_programID);

	m.ground = load_mesh(GROUND);
	glGenVertexArrays(1, &floor_vao);
	generateObjectBufferMesh1(floor_vao, m.ground, floor_programID);

	m.joint = load_mesh(JOINT);
	glGenVertexArrays(1, &vao.j);
	generateObjectBufferMesh1(vao.j, m.joint, programID);

	m.body = load_mesh(BODY);
	glGenVertexArrays(1, &vao.b);
	generateObjectBufferMesh1(vao.b, m.body, programID);


	m.leg_1 = load_mesh(LEFT_LEG_1);
	glGenVertexArrays(1, &vao.ul1);
	generateObjectBufferMesh1(vao.ul1, m.leg_1, programID);

	m.leg_1l = load_mesh(LEFT_LEG_1l);
	glGenVertexArrays(1, &vao.ll1);
	generateObjectBufferMesh1(vao.ll1, m.leg_1l, programID);

	m.leg_2 = load_mesh(LEFT_LEG_2);
	glGenVertexArrays(1, &vao.ul2);
	generateObjectBufferMesh1(vao.ul2, m.leg_2, programID);

	m.leg_2l = load_mesh(LEFT_LEG_2l);
	glGenVertexArrays(1, &vao.ll2);
	generateObjectBufferMesh1(vao.ll2, m.leg_2l, programID);

	m.leg_3 = load_mesh(LEFT_LEG_3);
	glGenVertexArrays(1, &vao.ul3);
	generateObjectBufferMesh1(vao.ul3, m.leg_3, programID);

	m.leg_3l = load_mesh(LEFT_LEG_3l);
	glGenVertexArrays(1, &vao.ll3);
	generateObjectBufferMesh1(vao.ll3, m.leg_3l, programID);

	m.leg_4 = load_mesh(LEFT_LEG_4);
	glGenVertexArrays(1, &vao.ul4);
	generateObjectBufferMesh1(vao.ul4, m.leg_4, programID);

	m.leg_4l = load_mesh(LEFT_LEG_4l);
	glGenVertexArrays(1, &vao.ll4);
	generateObjectBufferMesh1(vao.ll4, m.leg_4l, programID);

	m.leg_5 = load_mesh(RIGHT_LEG_1);
	glGenVertexArrays(1, &vao.ur1);
	generateObjectBufferMesh1(vao.ur1, m.leg_5, programID);

	m.leg_5l = load_mesh(RIGHT_LEG_1l);
	glGenVertexArrays(1, &vao.lr1);
	generateObjectBufferMesh1(vao.lr1, m.leg_5l, programID);

	m.leg_6 = load_mesh(RIGHT_LEG_2);
	glGenVertexArrays(1, &vao.ur2);
	generateObjectBufferMesh1(vao.ur2, m.leg_6, programID);

	m.leg_6l = load_mesh(RIGHT_LEG_2l);
	glGenVertexArrays(1, &vao.lr2);
	generateObjectBufferMesh1(vao.lr2, m.leg_6l, programID);

	m.leg_7 = load_mesh(RIGHT_LEG_3);
	glGenVertexArrays(1, &vao.ur3);
	generateObjectBufferMesh1(vao.ur3, m.leg_7, programID);

	m.leg_7l = load_mesh(RIGHT_LEG_3l);
	glGenVertexArrays(1, &vao.lr3);
	generateObjectBufferMesh1(vao.lr3, m.leg_7l, programID);

	m.leg_8 = load_mesh(RIGHT_LEG_4);
	glGenVertexArrays(1, &vao.ur4);
	generateObjectBufferMesh1(vao.ur4, m.leg_8, programID);

	m.leg_8l = load_mesh(RIGHT_LEG_4l);
	glGenVertexArrays(1, &vao.lr4);
	generateObjectBufferMesh1(vao.lr4, m.leg_8l, programID);

	textures[3] = load_skybox(faces);

	textures[0] = load_textures(floor_programID, "C:/Users/aogra/Desktop/mtass/mtass/foliage.jpg", "grass", 0, GL_TEXTURE0);
	textures[1] = load_textures(floor_programID, "C:/Users/aogra/Desktop/mtass/mtass/bark.jpg", "bark", 1, GL_TEXTURE1);
	textures[2] = load_textures(floor_programID, "C:/Users/aogra/Desktop/mtass/mtass/foliage.jpg", "foliage", 2, GL_TEXTURE2);
	textures[4] = load_textures(light_programID, "C:/Users/aogra/Desktop/mtass/mtass/moon_tex.jpg", "moon_tex", 4, GL_TEXTURE4);
	textures[5] = load_textures(light_programID, "C:/Users/aogra/Desktop/mtass/mtass/fire.jpg", "fire_tex", 5, GL_TEXTURE5);
	textures[6] = load_textures(programID, "C:/Users/aogra/Desktop/mtass/mtass/spider_tex1.jpg", "spider_tex1", 6, GL_TEXTURE6);
	textures[7] = load_textures(programID, "C:/Users/aogra/Desktop/mtass/mtass/spider_tex.jpg", "spider_tex2", 7, GL_TEXTURE7);

#pragma region SPIDERDATA
	sp1.tx = 8.0f;
	sp1.tz = 8.0f;

	sp2.tx = 12.0f;
	sp2.tz = 4.0f;

	sp3.tx = 12.0f;
	sp3.tz = -4.0f;

	sp4.tx = 8.0f;
	sp4.tz = -8.0f;

	sp5.tx = 4.0f;
	sp5.tz = -12.0f;

	sp6.tx = -4.0f;
	sp6.tz = -12.0f;

	sp7.tx = -8.0f;
	sp7.tz = -8.0f;

	sp8.tx = -12.0f;
	sp8.tz = -4.0f;

	sp9.tx = -12.0f;
	sp9.tz = 4.0f;

	sp10.tx = -8.0f;
	sp10.tz = 8.0f;

	sp11.tx = -4.0f;
	sp11.tz = 12.0f;

	sp12.tx = 4.0f;
	sp12.tz = 12.0f;

	sp1.rotate_body = 0.0f;
	sp2.rotate_body = 150.0f;
	sp3.rotate_body = 180.0f;
	sp4.rotate_body = 210.0f;
	sp5.rotate_body = 240.0f;
	sp6.rotate_body = 270.0f;
	sp7.rotate_body = 300.0f;
	sp8.rotate_body = 330.0f;
	sp9.rotate_body = 0.0f;
	sp10.rotate_body = 30.0f;
	sp11.rotate_body = 60.0f;
	sp12.rotate_body = 90.0f;


#pragma endregion SPIDERDATA

}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	//glutKeyboardFunc(change_light);
	glutMotionFunc(mouseclick);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Set up your objects and shaders
	init();

	//updateScene(0);
	glutMainLoop();

	return 0;
}