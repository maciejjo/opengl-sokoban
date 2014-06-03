#include "mesh.h"

struct mesh *create_mesh(char *fn) {

struct mesh *m = malloc(sizeof(struct mesh));
strcpy(m->filename, fn);
return m;

}

int mesh_load(struct mesh *mesh) {

  const struct aiScene *scene = aiImportFile(mesh->filename, aiProcess_Triangulate);
  if(!scene) {
    fprintf(stderr, "ERROR: reading mesh %s", mesh->filename);
    return -1;
  }

  const struct aiMesh *mesh_ai = scene->mMeshes[0];
  printf("%i vertices in mesh[0]\n", mesh_ai->mNumVertices);
  mesh->v_no = mesh_ai->mNumVertices;
  mesh->i_no = mesh_ai->mNumFaces;

  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);

  GLfloat *points = NULL;
  GLfloat *normals = NULL;
  GLfloat *texcoords = NULL;

	if(mesh_ai->mVertices != NULL && mesh_ai->mNumVertices > 0) {
    points = malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vp = &(mesh_ai->mVertices[i]);
      points[i * 3] = (GLfloat) vp->x;
      points[i * 3 + 1] = (GLfloat) vp->y;
      points[i * 3 + 2] = (GLfloat) vp->z;
    }
  }

  if(mesh_ai->mNormals != NULL && mesh_ai->mNumVertices > 0) {
    normals = malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vn = &(mesh_ai->mNormals[i]);
      printf("Loading %f, %f, %f\n", vn->x, vn->y, vn->z);
      normals[i * 3] = (GLfloat) vn->x;
      normals[i * 3 + 1] = (GLfloat) vn->y;
      normals[i * 3 + 2] = (GLfloat) vn->z;
    }
  }

  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    texcoords = malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vt = &(mesh_ai->mTextureCoords[0][i]);
      texcoords[i * 2] = (GLfloat) vt->x;
      texcoords[i * 2 + 1] = (GLfloat) vt->y;

    }
  }

	if(mesh_ai->mVertices != NULL && mesh_ai->mNumVertices > 0) {
    printf("Loading position...\n");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->v_no * sizeof(GLfloat),
        points, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(mesh->shader_program, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    free(points);
  }

  if(mesh_ai->mNormals != NULL && mesh_ai->mNumVertices > 0) {
    printf("Loading normals...\n");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    //glGenBuffers(1, &mesh->vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->v_no * sizeof(GLuint),
        points, GL_STATIC_DRAW);
    GLint normAttrib = glGetAttribLocation(mesh->shader_program, "normal");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(normAttrib);
    free(normals);
  }

  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    printf("Loading texcoords...\n");
    //glGenBuffers(1, &mesh->vbo_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->v_no, points, GL_STATIC_DRAW);
    GLint texcoordAttrib = glGetAttribLocation(mesh->shader_program, "texcoord");
    glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texcoordAttrib);
    free(texcoords);
  }

  aiReleaseImport(scene);
  printf("Mesh loaded\n");

  return 0;
}