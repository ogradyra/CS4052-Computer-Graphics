#pragma once

#include <limits>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector>

using std::vector;

unsigned int load_skybox(vector<const char*> faces);
unsigned int load_textures(GLuint programID, const char* filepath, const GLchar* texString, int texNum, int active_arg);