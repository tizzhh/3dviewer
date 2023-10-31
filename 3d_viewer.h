#ifndef C8_3DVIEWER_V1_SRC_3D_VIEWER_H
#define C8_3DVIEWER_V1_SRC_3D_VIEWER_H
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

typedef enum output {OK, ERROR} output;

// PARSER
output ParserFirstReadFile(char* path, int* count_of_vertexes, int* count_of_facets);
void ParserSecondReadFile(char* path, data* data);
size_t ParserCountOfChars(char* str);
size_t ParserCountOFVertexesInStr(char* str);

#endif // C8_3DVIEWER_V1_SRC_3D_VIEWER_H