#include "mesh.h"

struct mesh *mesh_create(const char *fn) {

  mesh *m = (mesh *) malloc(sizeof(struct mesh));
  strcpy(m->filename, fn);
  return m;

}

int mesh_load_mesh(struct mesh *m) {

  /*
  const struct aiScene *scene = aiImportFile(mesh->filename, aiProcess_Triangulate);

  if(!scene) {
    fprintf(stderr, "ERROR: reading mesh %s", mesh->filename);
    return -1;
  }

  const struct aiMesh *mesh_ai = scene->mMeshes[0];
  printf("%i vertices in mesh[0]\n", mesh_ai->mNumVertices);
  mesh->v_no = mesh_ai->mNumVertices;
  mesh->i_no = mesh_ai->mNumFaces;
  */

  FILE *objfile = fopen(m->filename, "r");

  const char *abbrevs[4] = { "v", "vn", "vt", "f" };
  int counts[4] = { 0, 0, 0, 0 };


  glGenVertexArrays(1, &m->vao);
  glBindVertexArray(m->vao);

  GLfloat *points = NULL;
  GLfloat *normals = NULL;
  GLfloat *texcoords = NULL;
  GLint   *faces = NULL;

  char line[256];
  char str[32];

  while(fgets(line, 256, objfile) != NULL) {

    sscanf(line, "%s", str);

      int i;

      for(i = 0; i < 4; i++) {

        if(strlen(str) == strlen(abbrevs[i]) && strcmp(str, abbrevs[i]) == 0)
          counts[i]++;

      }

  }

  printf("%d (%d) vertices, %d (%d) normals, %d (%d) texcoords, %d (%d) faces\n",
      counts[0], counts[0] * 3,
      counts[1], counts[1] * 3,
      counts[2], counts[2] * 2,
      counts[3], counts[3] * 3 * 3);

  points    = (float *) malloc(counts[0] * 3 * sizeof(float));
  normals   = (float *) malloc(counts[1] * 3 * sizeof(float));
  texcoords = (float *) malloc(counts[2] * 2 * sizeof(float));
  faces     = (int *)   malloc(counts[3] * 3 * 3 * sizeof(int));

  GLfloat *final_points    = (float *) malloc(counts[3] * 3 * sizeof(float));
  GLfloat *final_normals   = (float *) malloc(counts[3] * 3 * sizeof(float));
  GLfloat *final_texcoords = (float *) malloc(counts[3] * 2 * sizeof(float));

  fseek(objfile, 0, SEEK_SET);

  int points_iter = 0;
  int normals_iter = 0;
  int texcoords_iter = 0;
  int faces_iter = 0;
  while(fgets(line, 256, objfile) != NULL) {


    sscanf(line, "%s", str);

    char tmp[10];

    if(strlen(str) == 1 && strcmp(str, "v") == 0) {
      sscanf(line, "%s %f %f %f", tmp, &points[points_iter * 3], &points[points_iter * 3 + 1], &points[points_iter * 3 + 2]);
      printf("vertex poczytano: %d: %f %d: %f %d: %f\n",
          points_iter * 3, points[points_iter * 3], points_iter * 3 +1, points[points_iter * 3 + 1], points_iter * 3 +2, points[ points_iter * 3 + 2]);
      points_iter++;

    }

    if(strlen(str) == 2 && strcmp(str, "vn") == 0) {
      sscanf(line, "%s %f %f %f", tmp, &normals[normals_iter * 3], &normals[normals_iter * 3 + 1], &normals[normals_iter * 3 + 2]);
      printf("normal poczytano: %d: %f %d: %f %d: %f\n",
          normals_iter * 3, normals[normals_iter * 3], normals_iter * 3 +1, normals[normals_iter * 3 + 1], normals_iter * 3 +2, normals[ normals_iter * 3 + 2]);
      normals_iter++;

    }

    if(strlen(str) == 2 && strcmp(str, "vt") == 0) {
      sscanf(line, "%s %f %f", tmp, &texcoords[texcoords_iter* 2], &texcoords[texcoords_iter* 2 + 1]);
      printf("texcoord poczytano: %d: %f %d: %f\n",
          texcoords_iter * 2, texcoords[texcoords_iter* 2], texcoords_iter * 2 +1, texcoords[texcoords_iter * 2 + 1]);
      texcoords_iter++;

    }

    if(strlen(str) == 1 && strcmp(str, "f") == 0) {
      sscanf(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d", tmp,

        &faces[faces_iter* 9],
        &faces[faces_iter* 9 + 1],
        &faces[faces_iter* 9 + 2],

        &faces[faces_iter* 9 + 3],
        &faces[faces_iter* 9 + 4],
        &faces[faces_iter* 9 + 5],

        &faces[faces_iter* 9 + 6],
        &faces[faces_iter* 9 + 7],
        &faces[faces_iter* 9 + 8]

      );


      faces[faces_iter* 9]--;
      faces[faces_iter* 9 + 3]--;
      faces[faces_iter* 9 + 6]--;

      final_points[faces_iter * 9]      = points[faces[faces_iter * 9] * 3];
      printf("POINT %d. %f\n", faces_iter * 9, final_points[faces_iter * 9]);
      final_points[faces_iter * 9 + 1]  = points[faces[faces_iter * 9] * 3 + 1];
      printf("POINT %d. %f\n", faces_iter * 9 + 1, final_points[faces_iter * 9 + 1]);
      final_points[faces_iter * 9 + 2]  = points[faces[faces_iter * 9] * 3 + 2];
      printf("POINT %d. %f\n", faces_iter * 9 + 2, final_points[faces_iter * 9 + 2]);

      final_points[faces_iter * 9 + 3]  = points[faces[faces_iter * 9 + 3] * 3];
      printf("POINT %d. %f\n", faces_iter * 9 + 3, final_points[faces_iter * 9 + 3]);
      final_points[faces_iter * 9 + 4]  = points[faces[faces_iter * 9 + 3] * 3 + 1];
      printf("POINT %d. %f\n", faces_iter * 9 + 4, final_points[faces_iter * 9 + 4]);
      final_points[faces_iter * 9 + 5]  = points[faces[faces_iter * 9 + 3] * 3 + 2];
      printf("POINT %d. %f\n", faces_iter * 9 + 5, final_points[faces_iter * 9 + 5]);

      final_points[faces_iter * 9 + 6]  = points[faces[faces_iter * 9 + 6] * 3];
      printf("POINT %d. %f\n", faces_iter * 9 + 6, final_points[faces_iter * 9 + 6]);
      final_points[faces_iter * 9 + 7]  = points[faces[faces_iter * 9 + 6] * 3 + 1];
      printf("POINT %d. %f\n", faces_iter * 9 + 7, final_points[faces_iter * 9 + 7]);
      final_points[faces_iter * 9 + 8]  = points[faces[faces_iter * 9 + 6] * 3 + 2];
      printf("POINT %d. %f\n", faces_iter * 9 + 8, final_points[faces_iter * 9 + 8]);

      faces[faces_iter* 9 + 1]--;
      faces[faces_iter* 9 + 4]--;
      faces[faces_iter* 9 + 7]--;

      final_texcoords[faces_iter * 9]      = texcoords[faces[faces_iter * 9 + 1] * 2];
      final_texcoords[faces_iter * 9 + 1]  = texcoords[faces[faces_iter * 9 + 1] * 2 + 1];

      final_texcoords[faces_iter * 9 + 2]  = texcoords[faces[faces_iter * 9 + 4] * 2];
      final_texcoords[faces_iter * 9 + 3]  = texcoords[faces[faces_iter * 9 + 4] * 2 + 1];

      final_texcoords[faces_iter * 9 + 4]  = texcoords[faces[faces_iter * 9 + 7] * 2];
      final_texcoords[faces_iter * 9 + 5]  = texcoords[faces[faces_iter * 9 + 7] * 2 + 1];

      faces[faces_iter* 9 + 2]--;
      faces[faces_iter* 9 + 5]--;
      faces[faces_iter* 9 + 8]--;

      final_normals[faces_iter * 9]      = normals[faces[faces_iter * 9 + 2] * 3];
      final_normals[faces_iter * 9 + 1]  = normals[faces[faces_iter * 9 + 2] * 3 + 1];
      final_normals[faces_iter * 9 + 2]  = normals[faces[faces_iter * 9 + 2] * 3 + 2];

      final_normals[faces_iter * 9 + 3]  = normals[faces[faces_iter * 9 + 5] * 3];
      final_normals[faces_iter * 9 + 4]  = normals[faces[faces_iter * 9 + 5] * 3 + 1];
      final_normals[faces_iter * 9 + 5]  = normals[faces[faces_iter * 9 + 5] * 3 + 2];

      final_normals[faces_iter * 9 + 6]  = normals[faces[faces_iter * 9 + 8] * 3];
      final_normals[faces_iter * 9 + 7]  = normals[faces[faces_iter * 9 + 8] * 3 + 1];
      final_normals[faces_iter * 9 + 8]  = normals[faces[faces_iter * 9 + 8] * 3 + 2];

      faces_iter++;

    }
  }

  /*
  int i = 0;
  for(i = 0; i < counts[3] * 3; i++)
    printf("%d/%d. %f\n", i, counts[3] * 3, final_points[i]);
  */

  /*
	if(mesh_ai->mVertices != NULL && mesh_ai->mNumVertices > 0) {
    points = (float *) malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vp = &(mesh_ai->mVertices[i]);
      points[i * 3] = (GLfloat) vp->x;
      points[i * 3 + 1] = (GLfloat) vp->y;
      points[i * 3 + 2] = (GLfloat) vp->z;
    }
  }
  */

  /*
  if(mesh_ai->mNormals != NULL && mesh_ai->mNumVertices > 0) {
    normals = (float *) malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vn = &(mesh_ai->mNormals[i]);
      //printf("Loading %f, %f, %f\n", vn->x, vn->y, vn->z);
      normals[i * 3] = (GLfloat) vn->x;
      normals[i * 3 + 1] = (GLfloat) vn->y;
      normals[i * 3 + 2] = (GLfloat) vn->z;
    }
  }
  */

  /*
  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    texcoords = (float *) malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vt = &(mesh_ai->mTextureCoords[0][i]);
      texcoords[i * 2] = (GLfloat) vt->x;
      texcoords[i * 2 + 1] = (GLfloat) vt->y;

    }
  }
  */

  /*
    printf("Loading position...\n");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * counts[3] * 3 * sizeof(GLfloat),
        points, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(m->shader->id, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    free(points);

    printf("Loading normals...\n");
    glGenBuffers(1, &vbo);
    //glGenBuffers(1, &mesh->vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * counts[3] * 3 * sizeof(GLuint),
        points, GL_STATIC_DRAW);
    GLint normAttrib = glGetAttribLocation(m->shader->id, "normal");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(normAttrib);
    free(normals);
    */

  /*
  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    printf("Loading texcoords...\n");
    //glGenBuffers(1, &mesh->vbo_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->v_no, points, GL_STATIC_DRAW);
    GLint texcoordAttrib = glGetAttribLocation(mesh->shader->id, "texcoord");
    glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texcoordAttrib);
    free(texcoords);
  }
  */

  /*
  aiReleaseImport(scene);
  */
  printf("Mesh loaded\n");

  return 0;
}

void mesh_load_shader(mesh *mesh, shader *shader) {

  mesh->shader = shader;

}

void mesh_draw_mesh(mesh *m) {

  glBindVertexArray(m->vao);


}



void mesh_load_texture() {

}
