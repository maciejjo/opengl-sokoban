#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"


typedef struct mesh {

  char filename[64];
  GLuint vbo_vert;
  GLuint vbo_norm;
  GLuint vbo_texcoord;
  GLuint ibo;
  GLuint vao;
  shader *shader;

  unsigned int v_no, i_no;

} mesh;

mesh *mesh_create();
int mesh_load_mesh(mesh *mesh);
void mesh_load_shader(mesh *mesh, shader *shader);

#endif

