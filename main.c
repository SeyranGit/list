#include <stdio.h>
#include "list.h"
#include "sort.h"


void dis(int value) {
  printf("Deleted: %d\n", value);
}


struct Object {
  void *value;
  int type;
};


struct Sum { int a, b; };


void print(FILE *out, void *value) {
  struct Object *obj = (struct Object*)value;
  switch (obj->type) {
    case 0: fprintf(out, "%d", *(int*)obj->value); break;
    case 1: fprintf(out, "%.2f", *(float*)obj->value); break;
    case 2:
      fprintf(
        out,
        "<struct Sum(a=%d, b=%d)>",
        (*(struct Sum*)obj->value).a,
        (*(struct Sum*)obj->value).b
      );
      break;
  }
}


void print_int_ptr(FILE *out, void *value) {
  fprintf(out, "%d", *(int*)value);
}


void print_float_ptr(FILE *out, void *value) {
  fprintf(out, "%f", *(float*)value);
}


void print_int(FILE *out, int value) {
  fprintf(out, "%d", value);
}


void print_float(FILE *out, float value) {
  fprintf(out, "%f", value);
}


#if !VOID_PTR
void memory_test(void) {
  struct List *list = list_create();
  list_append(list, 10);
  list_append(list, 10);
  list_append(list, 30);
  list_pop(list, list_length(list) - 1);
  for (unsigned int i = 0; i < 10000000; ++i)
    list_append(list, i);
  list_free(list, NULL);
  printf("Memory test complete!\n");
}


int cmp(int a, int b) {
  return (int)(a > b);
}


void sort_test(void) {
  struct List *list = list_create();
  list_append(list, 5);
  list_append(list, 4);
  list_append(list, 2);
  list_append(list, 3);
  list_append(list, 1);
  printf("Sort test start!\n");
  list_print(list, stdout, print_int, "\n");
  sort(list, cmp);
  list_print(list, stdout, print_int, "\n");
  list_free(list, NULL);
  printf("Sort test complete!\n"); 
}
#endif


void print_int_and_float(FILE *out, void *value) {
  struct Object obj = *(struct Object*)value;
  switch (obj.type) {
    case 0: fprintf(out, "%d", *(int*)obj.value); break;
    case 1: fprintf(out, "%.2f", *(float*)obj.value); break;
  }
}


int if_cmp(void *a, void *b) {
  struct Object obj0 = *(struct Object*)a;
  struct Object obj1 = *(struct Object*)b;
  float a0, b0;
  if (obj0.type == 0) {
    int tmp = *(int*)obj0.value;
    a0 = (float)tmp;
  } else {
    a0 = *(float*)obj0.value;
  }
  if (obj1.type == 0) {
    int tmp = *(int*)obj1.value;
    b0 = (float)tmp;
  } else {
    b0 = *(float*)obj1.value;
  }
  return a0 > b0;
}


int main(void) {
  struct List *list = list_create();
  if (list == NULL) {
    fprintf(stderr, "list_create: out of memory\n");
    return 1;
  }
#if VOID_PTR
  float a, b; 
  a = 12.56;
  b = 20.93;
  int c, d;
  c = 10;
  d = 50;

  struct Sum sum = {1, 2};

  struct Object obj1 = {&a, 1};
  struct Object obj2 = {&b, 1};
  struct Object obj3 = {&c, 0};
  struct Object obj4 = {&d, 0};
  struct Object obj5 = {&sum, 2};


  list_append(list, &obj1); 
  list_append(list, &obj2); 
  list_append(list, &obj3); 
  list_append(list, &obj4);
  list_append(list, &obj5);
 
  list_print(list, stdout, print, "\n");
  list_free(list, NULL);
  // sort
  float a0, b0, c0, d0;
  int e0, f0, g0;
  a0 = 34.57; 
  b0 = 22.3; 
  c0 = 324.6; 
  d0 = 334.57; 
  e0 = 45;
  f0 = 38;
  g0 = 1230;

  struct Object obj6 = {&a0, 1};
  struct Object obj7 = {&b0, 1};
  struct Object obj8 = {&c0, 1};
  struct Object obj9 = {&d0, 1};
  struct Object obj10 = {&e0, 0};
  struct Object obj11 = {&f0, 0};
  struct Object obj12 = {&g0, 0};

  list = list_create();
  list_append(list, &obj6); 
  list_append(list, &obj7); 
  list_append(list, &obj8); 
  list_append(list, &obj9);
  list_append(list, &obj10);
  list_append(list, &obj11);
  list_append(list, &obj12);
  list_print(list, stdout, print_int_and_float, "\n");
  sort(list, if_cmp);
  list_print(list, stdout, print_int_and_float, "\n");
  list_free(list, NULL); 
#endif

#if !VOID_PTR
  list_append(list, 15);
  list_append(list, 100);
  list_append(list, 30);
 
  list_print(list, stdout, print_int, "\n"); 
  list_pop(list, 1); 
  list_print(list, stdout, print_int, "\n");
  list_append(list, 96); 
  list_print(list, stdout, print_int, "\n");
 
  list_free(list, NULL);
  list = list_create();
  if (list == NULL) {
    fprintf(stderr, "list_create: out of memory\n");
    return 1;
  }
  list_print(list, stdout, print_int, "\n");
  memory_test();
  list_append(list, 300);
  list_print(list, stdout, print_int, "\n");
  list_free(list, NULL);
  list = list_create();
  if (list == NULL) {
    fprintf(stderr, "list_create: out of memory\n");
    return 1;
  }
  list_print(list, stdout, print_int, "\n");
  list_append(list, 15);
  list_append(list, 100);
  list_append(list, 30);
  list_print(list, stdout, print_int, "\n");
  list_insert(list, 0, 200); 
  list_insert(list, 2, 200); 
  list_insert(list, 4, 200); 
  list_insert(list, 6, 200);
  list_print(list, stdout, print_int, "\n");
  list_print(list, stdout, print_int, " |end|\n");
  list_free(list, dis);
  sort_test();
#endif
  return 0;
}

