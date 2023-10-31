#ifndef C8_3DVIEWER_V1_SRC_3D_VIEWER_H
#define C8_3DVIEWER_V1_SRC_3D_VIEWER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
  double** matrix;
  int rows;
  int cols;
} matrix_t;

typedef struct facets {
  int* vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct data {
  int count_of_vertexes;
  int count_of_facets;
  matrix_t matrix_3d;
  polygon_t* polygons;
} data;

typedef enum output { OK, ERROR } output;
typedef enum axis { OX, OY, OZ } axis;
// PARSER
output S21_ParserFirstReadFile(char* path, int* count_of_vertexes,
                               int* count_of_facets);
void S21_ParserSecondReadFile(char* path, data* data);
size_t S21_ParserCountOfChars(char* str);
size_t S21_ParserCountOFVertexesInStr(char* str);

// AFFINE
void S21_Translation(matrix_t* vertices, double move_x, double move_y,
                     double move_z);
void S21_Rotation(matrix_t* vertices, axis axis, double angle);
void S21_Scaling(matrix_t* vertices, double mult_x, double mult_y,
                 double mult_z);

#endif  // C8_3DVIEWER_V1_SRC_3D_VIEWER_H