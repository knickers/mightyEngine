#pragma once

#include <iostream>
using namespace std;
#include "tga.h" // image loader code

// Texture constants
const int num_textures = 1;
static GLuint texName[num_textures];
//int whichTex = 1;

// Function prototypes
void LoadTextures();
gliGenericImage *readTgaImage(char *filename);
void SetBorder(gliGenericImage * image);
bool PowerOf2(int h);
