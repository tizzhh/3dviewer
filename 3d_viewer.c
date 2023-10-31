#include "3d_viewer.h"

output ParserFirstReadFile(char* path, int* count_of_vertexes, int* count_of_facets) {
  FILE* file = fopen(path, "r");
  char ch;
  output res = OK;
  while ((ch = fgetc(file)) != EOF) {
    if(ch == 'v' || ch == 'f' || ch == ' ' || ch == '.' 
    || ch == '-' || ch == '\n' || ch == '/' || (ch >= '0' && ch <= '9')) {
      if(ch == 'v') ++(*count_of_vertexes);
      else if(ch == 'f') ++(*count_of_facets);
    }
    else {
      res = ERROR;
      break;
    }
  }
  fclose(file);
  return res;
}

size_t ParserCountOFVertexesInStr(char* str) {
  size_t count_of_vertexes = 0, move = 0;
  while(*str != '\0') {
    ++move;
    if(*(str)++ == ' ') ++count_of_vertexes;
  }
  str-= move;
  return count_of_vertexes + 1;
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

size_t ParserCountOfChars(char* str) {
  size_t result = 0;
  while(*(str)++ != ' ' || *(str)++ != '\0') ++result;
  return result;
}

void ParserSecondReadFile(char* path, data* data) {
  FILE* file = fopen(path, "r");
  char* str;
  size_t len = 0, count_of_vertexes = 0, count_of_polygons = 0;
  while(getline(&str, &len, file) != -1) {
    char mode;
    sscanf(str, "%c", &mode);
    if(mode == 'v') {
      str+=2;
      for(size_t i = 0;i != 3;++i) sscanf(str, "%lf", &data->matrix_3d.matrix[count_of_vertexes][i]);
      ++count_of_vertexes;
    }
    else {
      #define vert_in_fac data->polygons[count_of_polygons].numbers_of_vertexes_in_facets
      str+=2;
      vert_in_fac = ParserCountOFVertexesInStr(str);
      data->polygons[count_of_polygons].vertexes = calloc(vert_in_fac, sizeof(int));
      for(int i=0;i != vert_in_fac;++i) {
        sscanf(str, "%d", &data->polygons[count_of_polygons].vertexes[i]);
        str+= ParserCountOfChars(str);
      }
      ++count_of_polygons;
    }
  }
}

int main() {

}