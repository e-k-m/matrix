#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "src/matrix.h"


/*
 * Some lovely helpers
 */

#define test(fn)				\
  puts("... \x1b[33m" # fn "\x1b[0m");		\
  test_##fn();

static int matrix_equal(matrix_t *a, matrix_t *b) {
  if ((a->height == b->height) && (a->width == b->width)) {
    double *ptr_a = a->data;
    double *ptr_b = b->data;
    int length = a->height * a->width;

    for (int i = 0; i < length; i++) {
      if (*(ptr_a + i) != *(ptr_b + i)) {
	return 0;
	break;
      }
    }
    return 1;
  } else {
    return 0;
  }
};

/*
 * Some lovely tests
 */ 

static void
test_matrix_new() {
  int width = 10;
  int height = 20;
  matrix_t *m = matrix_new(height, width);
  assert(m->width == width);
  assert(m->height == height);
  assert(m->data[0] == 0.0);
  matrix_destroy(m);
}

static void
test_matrix_copy() {
  // Setup
  int width = 10;
  int height = 20;
  matrix_t *a = matrix_new(height, width);
  matrix_t *b = matrix_copy(a);

  // Assertions
  assert(matrix_equal(a, b) == 1);
  matrix_destroy(a);
  matrix_destroy(b); 
}

static void
test_matrix_destroy() {
  // Setup
  int width = 10;
  int height = 20;
  matrix_t *m = matrix_new(height, width);
  matrix_destroy(m);

  // Assertions TODO(how to assert frees, 
  // and problem with test currently not beeing independent)
  //assert(!m);
}

static void
test_matrix_print() {
  // TODO(how to test print, sdtout)
}

static void
test_matrix_get() {
  // Setup
  matrix_t *a = matrix_eye(2);
  // Assertions
  assert(matrix_get(a, 0, 0) == 1.0);
  assert(matrix_get(a, 0, 1) == 0.0);
  assert(matrix_get(a, 1, 0) == 0.0);
  assert(matrix_get(a, 1, 1) == 1.0);
  matrix_destroy(a);
}

static void
test_matrix_set() {
  // Setup
  matrix_t *a = matrix_new(2, 2);
  matrix_set(a, 0, 0, 1.0);
  matrix_set(a, 0, 1, 2.0);
  matrix_set(a, 1, 0, 3.0);
  matrix_set(a, 1, 1, 4.0);
  // Assertions
  assert(matrix_get(a, 0, 0) == 1.0);
  assert(matrix_get(a, 0, 1) == 2.0);
  assert(matrix_get(a, 1, 0) == 3.0);
  assert(matrix_get(a, 1, 1) == 4.0);
  matrix_destroy(a);
}

static void
test_matrix_set_from_array() {
  // Setup
  matrix_t *a = matrix_new(3, 3);
  matrix_t *b = matrix_new(3, 3);

  double data[] = {1.0, 2.0, 3.0, 
		   4.0, 5.0, 6.0,
		   7.0, 8.0, 9.0};

  matrix_set_from_array(a, data, sizeof(data) / sizeof(*data));
  
  double *ptr = b->data;
  b->data = data;

  // Assertions
  assert(matrix_equal(a, b) == 1);
  matrix_destroy(a);
  b->data = ptr;
  matrix_destroy(b);  
}

static void
test_matrix_eye() {
  // Setup
  matrix_t *a = matrix_eye(3);
  matrix_t *b = matrix_new(3, 3);

  MATRIX_SET_ARRAY(b, ARRAY({1.0, 0.0, 0.0,
	                     0.0, 1.0, 0.0,
	                     0.0, 0.0, 1.0}));

  // Assertions
  assert(matrix_equal(a, b) == 1);
  matrix_destroy(a);
  matrix_destroy(b);
}

static void 
test_matrix_trace() {
  // Setup
  matrix_t *m = matrix_eye(3);
  
  // Assertions
  assert(matrix_trace(m) == 3);
  matrix_destroy(m);  
}

static void 
test_matrix_transpose() {
  // Setup
  matrix_t *a = matrix_new(3, 3);
  MATRIX_SET_ARRAY(a, ARRAY({1.0, 2.0, 3.0,
	                     4.0, 5.0, 6.0, 
	                     7.0, 8.0, 9.0}));

  matrix_t *b = matrix_new(3, 3);
  MATRIX_SET_ARRAY(b, ARRAY({1.0, 4.0, 7.0,
	                     2.0, 5.0, 8.0, 
	                     3.0, 6.0, 9.0}));

  matrix_t *c = matrix_transpose(a);

  // Assertions
  assert(matrix_equal(c, b) == 1);
  matrix_destroy(a);
  matrix_destroy(b);
  matrix_destroy(c);
}

static void 
test_matrix_mean() {
  // Setup
  matrix_t *a = matrix_new(3, 3);
  MATRIX_SET_ARRAY(a, ARRAY({1.0, 2.0, 3.0,
            	             1.0, 2.0, 3.0,
	                     1.0, 2.0, 3.0}));

  matrix_t *b = matrix_new(1, 3);
  MATRIX_SET_ARRAY(b, ARRAY({1.0, 2.0, 3.0}));

  matrix_t *c = matrix_mean(a);

  // Assertions
  assert(matrix_equal(c, b) == 1);
  matrix_destroy(a);
  matrix_destroy(b);
  matrix_destroy(c);
}

static void
test_matrix_multiply(){
  
  // Setup for matrix * vector
  matrix_t *a = matrix_new(3, 3);
  MATRIX_SET_ARRAY(a, ARRAY({1.0, 2.0, 3.0,
	                     4.0, 5.0, 6.0,
	                     7.0, 8.0, 9.0}));

  matrix_t *b = matrix_new(3, 1);
  MATRIX_SET_ARRAY(b, ARRAY({1.0, 2.0, 3.0}));
  
  matrix_t *c = matrix_new(3, 1);
  MATRIX_SET_ARRAY(c, ARRAY({14.0, 32.0, 50.0}));

   // Setup for matrix * matrix

   matrix_t *d = matrix_new(3, 2);
   MATRIX_SET_ARRAY(d, ARRAY({1.0, 1.0, 
	                      2.0, 2.0,
                              3.0, 3.0}));

  matrix_t *e = matrix_new(3, 2);
  MATRIX_SET_ARRAY(e, ARRAY({14.0, 14.0, 
	                     32.0, 32.0,
                             50.0, 50.0}));

  matrix_t *f = matrix_multiply(a, b);  
  matrix_t *g = matrix_multiply(a, d);  

  // Assertions
  assert(matrix_equal(c, f) == 1);
  assert(matrix_equal(e, g) == 1);
  
  matrix_destroy(a);
  matrix_destroy(b);
  matrix_destroy(c);
  matrix_destroy(d);
  matrix_destroy(e);
  matrix_destroy(f);
  matrix_destroy(g);
}

static void 
test_matrix_scale() {

  // Setup
  matrix_t *a = matrix_new(3, 3);
  MATRIX_SET_ARRAY(a, ARRAY({1.0, 2.0, 3.0,
	                     4.0, 5.0, 6.0,
	                     7.0, 8.0, 9.0}));

  matrix_t *b = matrix_scale(a, 2.0);

  matrix_t *c = matrix_new(3, 3);
  MATRIX_SET_ARRAY(c, ARRAY({2.0, 4.0, 6.0,
	                     8.0, 10.0, 12.0,
	                     14.0, 16.0, 18.0}));
  
  // Assertions
  assert(matrix_equal(b, c) == 1);

  matrix_destroy(a);
  matrix_destroy(b);
  matrix_destroy(c);
}



static void
test_matrix_swap_row() {
  // Setup
  matrix_t *a = matrix_new(3, 3);
  MATRIX_SET_ARRAY(a, ARRAY({1.0, 2.0, 3.0,
	                     4.0, 5.0, 6.0,
	                     7.0, 8.0, 9.0}));
  
  matrix_t *b = matrix_new(3, 3);
  MATRIX_SET_ARRAY(b, ARRAY({4.0, 5.0, 6.0,
	                     1.0, 2.0, 3.0,
	                     7.0, 8.0, 9.0}));

  matrix_swap_row(a, 0, 1);
  
  // Assertions
  assert(matrix_equal(a, b) == 1);
  matrix_destroy(a);
  matrix_destroy(b);  
}

// And run them

int
main(void) {
  // Prototypes tests
  test(matrix_new);
  test(matrix_copy);
  test(matrix_destroy);
  test(matrix_print);
  test(matrix_eye);
  test(matrix_set_from_array);
  test(matrix_get);
  test(matrix_set);

  // Matrix function tests
  test(matrix_trace);
  test(matrix_transpose);
  test(matrix_mean);
  test(matrix_multiply);
  test(matrix_scale);
  test(matrix_swap_row);

  puts("... \x1b[32m100%\x1b[0m\n");
  return 0;
}
