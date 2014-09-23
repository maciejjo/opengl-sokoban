#include "mesh.h"
#include <png.h>

struct mesh *mesh_create(const char *fn) {

mesh *m = (mesh *) malloc(sizeof(struct mesh));
strcpy(m->filename, fn);
return m;

}

int mesh_load_mesh(struct mesh *mesh) {

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
    points = (GLfloat *) malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vp = &(mesh_ai->mVertices[i]);
      points[i * 3] = (GLfloat) vp->x;
      points[i * 3 + 1] = (GLfloat) vp->y;
      points[i * 3 + 2] = (GLfloat) vp->z;
    }
  }

  if(mesh_ai->mNormals != NULL && mesh_ai->mNumVertices > 0) {
    normals = (GLfloat *) malloc (mesh->v_no * 3 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vn = &(mesh_ai->mNormals[i]);
      normals[i * 3] = (GLfloat) vn->x;
      normals[i * 3 + 1] = (GLfloat) vn->y;
      normals[i * 3 + 2] = (GLfloat) vn->z;
    }
  }

  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    texcoords = (GLfloat *) malloc (mesh->v_no * 2 * sizeof(GLfloat));
    for(unsigned int i = 0; i < mesh->v_no; i++) {
      const struct aiVector3D *vt = &(mesh_ai->mTextureCoords[0][i]);
      texcoords[i * 2] = (GLfloat) vt->x;
      texcoords[i * 2 + 1] = (GLfloat) vt->y;
      printf("tex: %f, %f\n", vt->x, vt->y);

    }
  }

	if(mesh_ai->mVertices != NULL && mesh_ai->mNumVertices > 0) {
    printf("Loading position...\n");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->v_no * sizeof(GLfloat),
        points, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(mesh->shader->id, "position");
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
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->v_no * sizeof(GLfloat),
        normals, GL_STATIC_DRAW);
    GLint normAttrib = glGetAttribLocation(mesh->shader->id, "normal");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normAttrib);
    free(normals);
  }

  if(mesh_ai->mTextureCoords[0] != NULL && mesh_ai->mNumVertices > 0) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    printf("Loading texcoords...\n");
    //glGenBuffers(1, &mesh->vbo_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->v_no * sizeof(GLfloat),
        texcoords, GL_STATIC_DRAW);
    GLint texcoordAttrib = glGetAttribLocation(mesh->shader->id, "texcoord");
    glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texcoordAttrib);
    free(texcoords);
  }

  aiReleaseImport(scene);
  printf("Mesh loaded\n");

  return 0;
}

void mesh_load_shader(mesh *mesh, shader *shader) {

  mesh->shader = shader;

}

void mesh_load_texture(mesh *m, const char *fn) {

  printf("Loading texture...\n");

  glGenTextures(1, &m->tex);

  glBindTexture(GL_TEXTURE_2D, m->tex);

}

GLuint png_texture_load(mesh *m, const char * file_name, int * width, int * height)
{
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    glGenTextures(1, &m->tex);
    glBindTexture(GL_TEXTURE_2D, m->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return 0;
}
