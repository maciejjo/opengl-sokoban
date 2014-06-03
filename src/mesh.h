#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct mesh {

  char filename[64];
  GLuint vbo_vert;
  GLuint vbo_norm;
  GLuint vbo_texcoord;
  GLuint ibo;
  GLuint vao;
  GLuint shader_program;

  unsigned int v_no, i_no;

};

struct mesh *create_mesh();
int mesh_load(struct mesh *mesh);

#endif

