#include "3d_viewer.h"

output S21_ParserFirstReadFile(char* path, int* count_of_vertexes,
                               int* count_of_facets) {
  FILE* file = fopen(path, "r");
  char ch;
  output res = OK;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == 'v' || ch == 'f' || ch == ' ' || ch == '.' || ch == '-' ||
        ch == '\n' || ch == '/' || (ch >= '0' && ch <= '9') || ch == '\r') {
      if (ch == 'v')
        ++(*count_of_vertexes);
      else if (ch == 'f')
        ++(*count_of_facets);
    } else {
      res = ERROR;
      break;
    }
  }
  fclose(file);
  return res;
}

size_t S21_ParserCountOFVertexesInStr(char* str) {
  size_t count_of_vertexes = 1;
  while (*str != '\0')
    if (*(str)++ == ' ') ++count_of_vertexes;
  return count_of_vertexes;
}

// const char* ParserCreateFormat(int vertex_num) {
//   char* str;
//   size_t iter = 0;
//   for(int i=0;i != vertex_num;++i) {
//     str[iter++] = '%';
//     str[iter++] = 'l';
//     str[iter++] = 'f';
//   }
//   str[iter] = '\0';
//   return str;
// }

size_t S21_ParserCountOfChars(char* str) {
  size_t result = 0;
  while (*str >= '0' && *str <= '9') {
    ++result;
    ++str;
  }
  return result + 1;
}

void S21_ParserSecondReadFile(char* path, data* data) {
  FILE* file = fopen(path, "r");
  char* getline_str = NULL;
  size_t len = 0, count_of_vertexes = 0, count_of_polygons = 0;
  while (getline(&getline_str, &len, file) != -1) {
    char* str = getline_str;
    char mode;
    sscanf(str, "%c", &mode);
    if (mode == 'v') {
      str += 2;
      sscanf(str, "%lf%lf%lf", &data->matrix_3d.matrix[count_of_vertexes][0],
             &data->matrix_3d.matrix[count_of_vertexes][1],
             &data->matrix_3d.matrix[count_of_vertexes][2]);
      ++count_of_vertexes;
    } else {
#define vert_in_fac \
  data->polygons[count_of_polygons].numbers_of_vertexes_in_facets
      str += 2;
      vert_in_fac = S21_ParserCountOFVertexesInStr(str);
      data->polygons[count_of_polygons].vertexes =
          calloc(vert_in_fac, sizeof(int));
      for (int i = 0; i != vert_in_fac; ++i) {
        sscanf(str, "%d", &data->polygons[count_of_polygons].vertexes[i]);
        str += S21_ParserCountOfChars(str);
      }
      ++count_of_polygons;
    }
  }
  free(getline_str);
  fclose(file);
}

output S21_PrepareData(char* path, data* data) {
  output status = S21_ParserFirstReadFile(path, &data->count_of_vertexes,
                                          &data->count_of_facets);
  if (status == OK) {
    data->matrix_3d.rows = data->count_of_vertexes;
    data->matrix_3d.cols = 3;
    data->matrix_3d.matrix = calloc(data->matrix_3d.rows, sizeof(double*));
    for (int i = 0; i < data->matrix_3d.rows; ++i) {
      data->matrix_3d.matrix[i] = calloc(data->matrix_3d.cols, sizeof(double));
    }

    data->polygons = calloc(data->count_of_facets, sizeof(polygon_t));
    S21_ParserSecondReadFile(path, data);
  }
  return status;
}

int S21_Translate(matrix_t* vertices, double move_x, double move_y,
                  double move_z) {
  if (!vertices) {
    return ERROR;
  }
  for (int i = 0; i < vertices->rows; ++i) {
    vertices->matrix[i][OX] = vertices->matrix[i][OX] + move_x;
    vertices->matrix[i][OY] = vertices->matrix[i][OY] + move_y;
    vertices->matrix[i][OZ] = vertices->matrix[i][OZ] + move_z;
  }
  return OK;
}

int S21_Rotate(matrix_t* vertices, axis axis, double angle) {
  double temp_x;
  double temp_y;
  double temp_z;
  if (!vertices) {
    return ERROR;
  }
  double sin_val = sin(angle);
  double cos_val = cos(angle);
  for (int i = 0; i < vertices->rows; ++i) {
    if (axis == OX) {
      temp_y = vertices->matrix[i][OY];
      temp_z = vertices->matrix[i][OZ];
      vertices->matrix[i][OY] = temp_y * cos_val - temp_z * sin_val;
      vertices->matrix[i][OZ] = temp_y * sin_val + temp_z * cos_val;
    } else if (axis == OY) {
      temp_x = vertices->matrix[i][OX];
      temp_z = vertices->matrix[i][OZ];
      vertices->matrix[i][OX] = temp_x * cos_val + temp_z * sin_val;
      vertices->matrix[i][OZ] = -temp_x * sin_val + temp_z * cos_val;
    } else if (axis == OZ) {
      temp_x = vertices->matrix[i][OX];
      temp_y = vertices->matrix[i][OY];
      vertices->matrix[i][OX] = temp_x * cos_val - temp_y * sin_val;
      vertices->matrix[i][OY] = temp_x * sin_val + temp_y * cos_val;
    }
  }
  return OK;
}

int S21_Scale(matrix_t* vertices, double mult_x, double mult_y, double mult_z) {
  if (!vertices) {
    return ERROR;
  }
  for (int i = 0; i < vertices->rows; ++i) {
    vertices->matrix[i][OX] = vertices->matrix[i][OX] * mult_x;
    vertices->matrix[i][OY] = vertices->matrix[i][OY] * mult_y;
    vertices->matrix[i][OZ] = vertices->matrix[i][OZ] * mult_z;
  }
  return OK;
}

void S21_RemoveMatrix(matrix_t* matrix) {
  if (matrix && matrix->matrix) {
    for (int i = 0; i < matrix->rows; ++i) {
      free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    matrix->matrix = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
  }
}

void S21_RemovePolygons(polygon_t* polygons, int count_of_polygons) {
  if (polygons && polygons->vertexes) {
    for (int i = 0; i < count_of_polygons; ++i) {
      free(polygons[i].vertexes);
    }
    free(polygons);
  }
}

// int main() {}