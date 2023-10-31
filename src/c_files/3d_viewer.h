#ifndef C8_3DVIEWER_V1_SRC_3D_VIEWER_H
#define C8_3DVIEWER_V1_SRC_3D_VIEWER_H

#define _GNU_SOURCE
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
output S21_PrepareData(char* path, data* data);

// AFFINE
int S21_Translate(matrix_t* vertices, double move_x, double move_y,
                  double move_z);
int S21_Rotate(matrix_t* vertices, axis axis, double angle);
int S21_Scale(matrix_t* vertices, double mult_x, double mult_y, double mult_z);
void S21_RemoveMatrix(matrix_t* matrix);
void S21_RemovePolygons(polygon_t* polygons, int count_of_polygons);

#endif  // C8_3DVIEWER_V1_SRC_3D_VIEWER_H