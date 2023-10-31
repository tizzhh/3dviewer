#include <check.h>

#include "3d_viewer.h"

#define TOLERANCE 1e-6

START_TEST(test_parser_1) {
  char *file_name = "test_objs/cube.obj";
  data cube_data = {0};
  output first_parse = S21_PrepareData(file_name, &cube_data);
  // output first_parse = S21_ParserFirstReadFile(
  //     file_name, &cube_data.count_of_vertexes, &cube_data.count_of_facets);
  ck_assert_int_eq(first_parse, OK);
  // S21_ParserSecondReadFile(file_name, &cube_data);
  matrix_t parsed_data = cube_data.matrix_3d;
  float vertexes[] = {1.0, -1.0, -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0,
                      1.0, -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,
                      1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0};

  for (int i = 0; i < (sizeof(vertexes) / sizeof(vertexes[0]) / 3); ++i) {
    ck_assert_double_eq_tol(parsed_data.matrix[i][OX], vertexes[i * 3],
                            TOLERANCE);
    ck_assert_double_eq_tol(parsed_data.matrix[i][OY], vertexes[i * 3 + 1],
                            TOLERANCE);
    ck_assert_double_eq_tol(parsed_data.matrix[i][OZ], vertexes[i * 3 + 2],
                            TOLERANCE);
  }

  int facets[] = {2, 3, 4, 8, 7, 6, 5, 6, 2, 6, 7, 3, 3, 7, 8, 1, 4, 8,
                  1, 2, 4, 5, 8, 6, 1, 5, 2, 2, 6, 3, 4, 3, 8, 5, 1, 8};

  polygon_t *facetes_data = cube_data.polygons;
  for (int i = 0; i < (sizeof(facets) / sizeof(facets[0]) / 3); ++i) {
    for (int j = 0; j < facetes_data[i].numbers_of_vertexes_in_facets; ++j) {
      ck_assert_int_eq(facetes_data[i].vertexes[j], facets[i * 3 + j]);
    }
  }

  S21_RemoveMatrix(&cube_data.matrix_3d);
  S21_RemovePolygons(cube_data.polygons, cube_data.count_of_facets);
}

int main(void) {
  Suite *s = suite_create("Core");
  SRunner *runner = srunner_create(s);
  TCase *tc_core = tcase_create("Core");

  int no_failed = 0;
  tcase_add_test(tc_core, test_parser_1);

  suite_add_tcase(s, tc_core);
  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}