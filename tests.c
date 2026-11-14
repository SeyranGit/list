#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "list.h"


static unsigned int tests_run = 0;
static unsigned int tests_failed = 0;
static unsigned int checks_failed_in_test = 0;


static void check(int condition, const char *expression, int line) {
  if (!condition) {
    printf("  FAIL at line %d: %s\n", line, expression);
    checks_failed_in_test += 1;
  }
}


#define CHECK(condition) check((condition) != 0, #condition, __LINE__)


static ValueType int_to_value(int n) {
  return (ValueType)(intptr_t)n;
}


static int value_to_int(ValueType value) {
  return (int)(intptr_t)value;
}


static void print_int(FILE *out, ValueType value) {
  fprintf(out, "%d", value_to_int(value));
}


static unsigned int destructor_calls = 0;


static void counting_destructor(ValueType value) {
  (void)value;
  destructor_calls += 1;
}


static void test_create_empty(void) {
  struct List *list;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_length(list) == 0);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_ERROR_INDEX);
  CHECK(list_free(&list, NULL) == LIST_OK);
  CHECK(list == NULL);
}


static void test_length(void) {
  struct List *list;
  struct RemoveResult result;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_length(list) == 0);
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_length(list) == 1);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_length(list) == 2);
  CHECK(list_insert(list, 1, int_to_value(3)) == LIST_OK);
  CHECK(list_length(list) == 3);
  result = list_pop(list, 0);
  CHECK(result.status == LIST_OK);
  CHECK(list_length(list) == 2);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_append(void) {
  struct List *list;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(10)) == LIST_OK);
  CHECK(list_append(list, int_to_value(20)) == LIST_OK);
  CHECK(list_append(list, int_to_value(30)) == LIST_OK);
  CHECK(list_length(list) == 3);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_OK);
  CHECK(value_to_int(value) == 10);
  CHECK(list_sget(list, 1, &value) == LIST_OK);
  CHECK(value_to_int(value) == 20);
  CHECK(list_sget(list, 2, &value) == LIST_OK);
  CHECK(value_to_int(value) == 30);
  CHECK(list_sget(list, 3, &value) == LIST_ERROR_INDEX);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_append_null(void) {
  CHECK(list_append(NULL, int_to_value(1)) == LIST_ERROR_NULL);
}


static void test_insert_positions(void) {
  struct List *list;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_insert(list, 0, int_to_value(20)) == LIST_OK);
  CHECK(list_insert(list, 0, int_to_value(10)) == LIST_OK);
  CHECK(list_insert(list, 2, int_to_value(30)) == LIST_OK);
  CHECK(list_insert(list, 1, int_to_value(15)) == LIST_OK);
  CHECK(list_insert(list, 4, int_to_value(40)) == LIST_OK);
  CHECK(list_length(list) == 5);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_OK);
  CHECK(value_to_int(value) == 10);
  CHECK(list_sget(list, 1, &value) == LIST_OK);
  CHECK(value_to_int(value) == 15);
  CHECK(list_sget(list, 2, &value) == LIST_OK);
  CHECK(value_to_int(value) == 20);
  CHECK(list_sget(list, 3, &value) == LIST_OK);
  CHECK(value_to_int(value) == 30);
  CHECK(list_sget(list, 4, &value) == LIST_OK);
  CHECK(value_to_int(value) == 40);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_insert_bad_index(void) {
  struct List *list;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_insert(list, 1, int_to_value(1)) == LIST_ERROR_INDEX);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_insert(list, 5, int_to_value(3)) == LIST_ERROR_INDEX);
  CHECK(list_length(list) == 1);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_insert_null(void) {
  CHECK(list_insert(NULL, 0, int_to_value(1)) == LIST_ERROR_NULL);
}


static void test_pop_first(void) {
  struct List *list;
  struct RemoveResult result;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_append(list, int_to_value(3)) == LIST_OK);
  result = list_pop(list, 0);
  CHECK(result.status == LIST_OK);
  CHECK(value_to_int(result.value) == 1);
  CHECK(list_length(list) == 2);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_OK);
  CHECK(value_to_int(value) == 2);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_middle(void) {
  struct List *list;
  struct RemoveResult result;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_append(list, int_to_value(3)) == LIST_OK);
  result = list_pop(list, 1);
  CHECK(result.status == LIST_OK);
  CHECK(value_to_int(result.value) == 2);
  CHECK(list_length(list) == 2);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_OK);
  CHECK(value_to_int(value) == 1);
  CHECK(list_sget(list, 1, &value) == LIST_OK);
  CHECK(value_to_int(value) == 3);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_last(void) {
  struct List *list;
  struct RemoveResult result;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_append(list, int_to_value(3)) == LIST_OK);
  result = list_pop(list, 2);
  CHECK(result.status == LIST_OK);
  CHECK(value_to_int(result.value) == 3);
  CHECK(list_length(list) == 2);
  CHECK(list_append(list, int_to_value(4)) == LIST_OK);
  value = int_to_value(-1);
  CHECK(list_sget(list, 2, &value) == LIST_OK);
  CHECK(value_to_int(value) == 4);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_only(void) {
  struct List *list;
  struct RemoveResult result;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(42)) == LIST_OK);
  result = list_pop(list, 0);
  CHECK(result.status == LIST_OK);
  CHECK(value_to_int(result.value) == 42);
  CHECK(list_length(list) == 0);
  CHECK(list_append(list, int_to_value(7)) == LIST_OK);
  CHECK(list_length(list) == 1);
  value = int_to_value(-1);
  CHECK(list_sget(list, 0, &value) == LIST_OK);
  CHECK(value_to_int(value) == 7);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_empty(void) {
  struct List *list;
  struct RemoveResult result;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  result = list_pop(list, 0);
  CHECK(result.status == LIST_ERROR_INDEX);
  CHECK(list_length(list) == 0);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_bad_index(void) {
  struct List *list;
  struct RemoveResult result;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  result = list_pop(list, 1);
  CHECK(result.status == LIST_ERROR_INDEX);
  CHECK(list_length(list) == 1);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_pop_null(void) {
  struct RemoveResult result;
  result = list_pop(NULL, 0);
  CHECK(result.status == LIST_ERROR_NULL);
}


static void test_sget_null(void) {
  struct List *list;
  ValueType value;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  value = int_to_value(-1);
  CHECK(list_sget(NULL, 0, &value) == LIST_ERROR_NULL);
  CHECK(list_sget(list, 0, NULL) == LIST_ERROR_NULL);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_free_null_ptr(void) {
  CHECK(list_free(NULL, NULL) == LIST_ERROR_NULL);
}


static void test_free_null_list(void) {
  struct List *list;
  list = NULL;
  CHECK(list_free(&list, NULL) == LIST_ERROR_NULL);
}


static void test_free_destructor(void) {
  struct List *list;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_append(list, int_to_value(2)) == LIST_OK);
  CHECK(list_append(list, int_to_value(3)) == LIST_OK);
  destructor_calls = 0;
  CHECK(list_free(&list, counting_destructor) == LIST_OK);
  CHECK(destructor_calls == 3);
  CHECK(list == NULL);
}


static void test_free_double(void) {
  struct List *list;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(1)) == LIST_OK);
  CHECK(list_free(&list, NULL) == LIST_OK);
  CHECK(list == NULL);
  CHECK(list_free(&list, NULL) == LIST_ERROR_NULL);
}


static void test_print(void) {
  struct List *list;
  FILE *file;
  char buffer[64];
  size_t read_count;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  CHECK(list_append(list, int_to_value(10)) == LIST_OK);
  CHECK(list_append(list, int_to_value(20)) == LIST_OK);
  CHECK(list_append(list, int_to_value(30)) == LIST_OK);
  file = tmpfile();
  CHECK(file != NULL);
  if (file == NULL) {
    CHECK(list_free(&list, NULL) == LIST_OK);
    return;
  }
  CHECK(list_print(list, file, print_int) == LIST_OK);
  rewind(file);
  read_count = fread(buffer, 1, sizeof(buffer) - 1, file);
  buffer[read_count] = '\0';
  CHECK(strcmp(buffer, "[10, 20, 30]\n") == 0);
  fclose(file);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_print_null_list(void) {
  FILE *file;
  file = tmpfile();
  CHECK(file != NULL);
  if (file == NULL)
    return;
  CHECK(list_print(NULL, file, print_int) == LIST_ERROR_NULL);
  fclose(file);
}


static void test_print_null_printer(void) {
  struct List *list;
  FILE *file;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  file = tmpfile();
  CHECK(file != NULL);
  if (file == NULL) {
    CHECK(list_free(&list, NULL) == LIST_OK);
    return;
  }
  CHECK(list_print(list, file, NULL) == LIST_ERROR_NULL);
  fclose(file);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


static void test_append_many(void) {
  struct List *list;
  ValueType value;
  size_t i;
  int ok;
  list = list_create();
  CHECK(list != NULL);
  if (list == NULL)
    return;
  ok = 1;
  for (i = 0; i != 1000; ++i) {
    if (list_append(list, int_to_value((int)i)) != LIST_OK) {
      ok = 0;
      break;
    }
  }
  CHECK(ok);
  CHECK(list_length(list) == 1000);
  ok = 1;
  for (i = 0; i != 1000; ++i) {
    value = int_to_value(-1);
    if (list_sget(list, i, &value) != LIST_OK) {
      ok = 0;
      break;
    }
    if (value_to_int(value) != (int)i) {
      ok = 0;
      break;
    }
  }
  CHECK(ok);
  CHECK(list_free(&list, NULL) == LIST_OK);
}


struct TestCase {
  const char *name;
  void (*function)(void);
};


static void run_test(const struct TestCase *test) {
  checks_failed_in_test = 0;
  tests_run += 1;
  test->function();
  if (checks_failed_in_test == 0) {
    printf("PASS: %s\n", test->name);
  } else {
    printf("FAIL: %s (%u checks failed)\n", test->name, checks_failed_in_test);
    tests_failed += 1;
  }
}


int main(void) {
  struct TestCase tests[] = {
    {"create_empty",       test_create_empty},
    {"length",             test_length},
    {"append",             test_append},
    {"append_null",        test_append_null},
    {"insert_positions",   test_insert_positions},
    {"insert_bad_index",   test_insert_bad_index},
    {"insert_null",        test_insert_null},
    {"pop_first",          test_pop_first},
    {"pop_middle",         test_pop_middle},
    {"pop_last",           test_pop_last},
    {"pop_only",           test_pop_only},
    {"pop_empty",          test_pop_empty},
    {"pop_bad_index",      test_pop_bad_index},
    {"pop_null",           test_pop_null},
    {"sget_null",          test_sget_null},
    {"free_null_ptr",      test_free_null_ptr},
    {"free_null_list",     test_free_null_list},
    {"free_destructor",    test_free_destructor},
    {"free_double",        test_free_double},
    {"print",              test_print},
    {"print_null_list",    test_print_null_list},
    {"print_null_printer", test_print_null_printer},
    {"append_many",        test_append_many},
  };
  size_t tests_count = sizeof(tests) / sizeof(tests[0]);
  size_t i;
  for (i = 0; i != tests_count; ++i) {
    run_test(&tests[i]);
  }
  printf("\n%u/%u tests passed\n", tests_run - tests_failed, tests_run);
  return tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
