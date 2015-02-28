
//
// matrix.c
//
// Copyright (c) 2015 e-k-m
//

#include "matrix.h"

/*
 * Allocate a new matrix_t. NULL on failure.
 */

matrix_t * 
matrix_new(int height, int width) {
  matrix_t  *self;
  if (!(self = (matrix_t *) malloc(sizeof(matrix_t))))
    return NULL;

  self->width = width;
  self->height = height;
  if(!(self->data = (double *) malloc(sizeof(double) * width * height)))
    return NULL;

  memset(self->data, 0.0, width * height * sizeof(double));

  return self;
}


/*
 * Copy a matrix.
 */

matrix_t * 
matrix_copy(matrix_t *self) {
  return matrix_scale(self, 1);
}

/*
 * Free the matrix.
 */

void 
matrix_destroy(matrix_t *self) {
  if (self) {
    if (self->data) free(self->data);
    free(self);
  }
  return;
}

/*
 * Print the matrix
 */

void
matrix_print(matrix_t *self) {
  int i, j;
  double *ptr = self->data;
  printf("%d %d\n", self->height, self->width);
  for (i = 0; i < self->height; i++) {
    for (j = 0; j < self->width; j++) {
      printf(" %9.6f", *(ptr++));
    }
    printf("\n");
  }
  return;
}

/*
 * Allocate a new identity matrix_t.
 */

matrix_t *
matrix_eye(int n) {
  int i;
  matrix_t *self;
  double *ptr;

  self = matrix_new(n, n);
  ptr = self->data;
  for (i = 0; i < n; i++) {
    *ptr = 1.0;
    ptr += n + 1;
  }
  return self;
}

/*
 * Get the value at an index.
 */

double
matrix_get(matrix_t *self, int i, int j) {
  return *(self->data + (i * self->width) + j);
}

/*
 * Set the value at an index.
 */

void
matrix_set(matrix_t *self, int i, int j, double value) {
  *(self->data + (i * self->width) + j) = value;
}

/*
 * Set the values of the matrix from an array.
 */

void
matrix_set_from_array(matrix_t *self, double *data, size_t size) {
  double *ptr = self->data;
  double *ptr_d = data;
  for (size_t i = 0; i < size; i++) {
    *ptr = *ptr_d;
    ptr++;
    ptr_d++;
  }
}

/*
 * Given an "m rows by n columns" matrix, return the sum of the 
 * elements along the diagonal.
 */

double 
matrix_trace(matrix_t *self) {
  int i;
  int size;
  double *ptr = self->data;
  double sum = 0.0;

  if (self->height < self->width) {
    size = self->height;
  }
  else {
    size = self->width;
  }

  for (i = 0; i < size; i++) {
    sum += *ptr;
    ptr += self->width + 1;
  }

  return sum;
}

/*
 * Given an "m rows by n columns" matrix, return a matrix with 
 * 1 row and n columns, where each element represents the mean of 
 * that that full column.
 */

matrix_t * 
matrix_mean(matrix_t *self) {
    int i, j;
    double *ptr;
    matrix_t *out;

    out = matrix_new(1, self->width);

    for (i = 0; i < self->width; i++) {
        out->data[i] = 0.0;
        ptr = &self->data[i];
        for (j = 0; j < self->height; j++) {
            out->data[i] += *ptr;
            ptr += out->width;
        }
        out->data[i] /= (double) self->height;
    }
    return out;
}

/*
 * Given an "m rows by n columns" matrix, returns a matrix with 
 * n row and n columns, where each element represents covariance of 
 * 2 columns. TODO(how does this work?)
 */

matrix_t *
matrix_covariance(matrix_t *self) {
  int i, j, k = 0;
  matrix_t* out;
  matrix_t* mean;
  double *ptr_a;
  double *ptr_b;
  double *ptr_out;

  mean = matrix_mean(self);
  out = matrix_new(self->width, self->width);
  ptr_out = out->data;

  for (i = 0; i < self->width; i++) {
    for (j = 0; j < self->width; j++) {
      ptr_a = &self->data[i];
      ptr_b = &self->data[j];
      *ptr_out = 0.0;
      for (k = 0; k < self->height; k++) {
	*ptr_out += (*ptr_a - mean->data[i]) * (*ptr_b - mean->data[j]);
	ptr_a += self->width;
	ptr_b += self->width;
      }
      *ptr_out /= self->height - 1;
      ptr_out++;
    }
  }
  matrix_destroy(mean);
  return out;
}

/*
 * Given an matrix, returns the transpose.
 */

matrix_t *
matrix_transpose(matrix_t *self) {
  matrix_t *out = matrix_new(self->height, self->width);
  double* ptr_out;
  double* ptr = self->data;
  int i, j;

  for (i = 0; i < self->height; i++) {
    ptr_out = &out->data[i];
    for (j = 0; j < self->width; j++) {
      *ptr_out = *ptr;
      ptr++;
      ptr_out += out->width;
    }
  }
  return out;
}

/*
 * Given two matrices, returns the multiplication of the two.
 */

matrix_t *
matrix_multiply(matrix_t *a, matrix_t *b) {
  int i, j, k;
  matrix_t *out;
  double *ptr_out;
  double *ptr_a;
  double *ptr_b;

  out = matrix_new(a->height, b->width);
  ptr_out = out->data;

  for (i = 0; i < a->height; i++) {

    for (j = 0; j < b->width; j++) {
      ptr_a = &a->data[ i * a->width ];
      ptr_b = &b->data[ j ];

      *ptr_out = 0;
      for (k = 0; k < a->width; k++) {
	*ptr_out += *ptr_a * *ptr_b;
	ptr_a++;
	ptr_b += b->width;
      }
      ptr_out++;
    }
  }
  return out;
}

/*
 * Given a matrix and a double value, this returns a new matrix where 
 * each element in the input matrix is multiplied by the double value
 */

matrix_t * 
matrix_scale(matrix_t *self, double value) {
  int i, elements = self->width * self->height;
  matrix_t *out = matrix_new(self->height, self->width);
  double *ptr_m = self->data;
  double *ptr_out = out->data;

  for (i = 0; i < elements; i++) {
    *(ptr_out++) = *(ptr_m++) * value;
  }

  return out;
}

/*
 * Given a matrix, this algorithm will swap rows p and q, provided
 * that p and q are less than or equal to the height of matrix A and p
 * and q are different values. TODO(return new matrix)
 */

void
matrix_swap_row(matrix_t *self, int p, int q) {
  int i;
  double temp;
  double *p_row;
  double *q_row;

  // If p and q are equal, do nothing.
  if (p == q) {
    return;
  }

  p_row = self->data + (p * self->width);
  q_row = self->data + (q * self->width);

  // Swap!
  for (i = 0; i < self->width; i++) {
    temp = *p_row;
    *p_row = *q_row;
    *q_row = temp;
    p_row++;
    q_row++;
  }
  return;
}

/*
 * Given a two matrices (or the same matrix twice) with identical widths and
 * different heights, this method returns a a->height by b->height matrix of
 * the cross product of each matrix.
 *
 * Dot product is essentially the sum-of-squares of two vectors.
 *
 * Also, if the second paramter is NULL, it is assumed that we
 * are performing a cross product with itself.
 */

matrix_t *
matrix_dot_product(matrix_t *a, matrix_t *b) {
  matrix_t *out;
  double *ptr_out;
  double *ptr_a;
  double *ptr_b;
  int i, j, k;
    
  // Are we computing the sum of squares of the same matrix?
  if (a == b || b == NULL) {
    b = a;
  }

  out = matrix_new(b->height, a->height);
  ptr_out = out->data;

  for (i = 0; i < a->height; i++) {
    ptr_b = b->data;

    for (j = 0; j < b->height; j++) {
      ptr_a = &a->data[ i * a->width ];

      *ptr_out = 0;
      for (k = 0; k < a->width; k++) {
	*ptr_out += *ptr_a * *ptr_b;
	ptr_a++;
	ptr_b++;
      }
      ptr_out++;
    }
  }
  return out;
}

/*
 * Given a two matrices (or the same matrix twice) with identical widths and
 * heights, this method returns a 1 by a->height matrix of the cross
 * product of each matrix along the diagonal.
 *
 * Dot product is essentially the sum-of-squares of two vectors.
 *
 * If the second paramter is NULL, it is assumed that we
 * are performing a cross product with itself.
 */

matrix_t * 
matrix_dot_diagonal(matrix_t* a, matrix_t* b) {
  matrix_t *out;
  double *ptr_out;
  double *ptr_a;
  double *ptr_b;
  int i, j;

  // Are we computing the sum of squares of the same matrix?
  if (a == b || b == NULL) {
    b = a; // May not appear safe, but we can do this without risk of losing b.
  }

  out = matrix_new(a->height, 1);
  ptr_out = out->data;
  ptr_a = a->data;
  ptr_b = b->data;

  for (i = 0; i < a->height; i++) {
    *ptr_out = 0;
    for (j = 0; j < a->width; j++) {
      *ptr_out += *ptr_a * *ptr_b;
      ptr_a++;
      ptr_b++;
    }
    ptr_out++;
  }
  return out;
}
