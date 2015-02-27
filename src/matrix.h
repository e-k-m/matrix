//
// matrix.h
//
// Copyright (c) 2015 e-k-m
//

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Library version

#define MATRIX_VERSION "0.0.1"

/*
 * matrix_t struct.
 */

typedef struct matrix {
    int height;
    int width;
    double* data;
} matrix_t;

#define ARRAY(...) __VA_ARGS__

#define MATRIX_SET_ARRAY(M, A) { double _[] = A; matrix_set_from_array(M, _, sizeof(_) / sizeof(*_)); }

/*
 * matrix_t prototypes
 */

matrix_t *
matrix_new(int height, int width);

matrix_t *
matrix_copy(matrix_t *self);

void
matrix_destroy(matrix_t *self);

void
matrix_print(matrix_t *self);

matrix_t *
matrix_eye(int n);

void
matrix_set_from_array(matrix_t *self, double *data, size_t size);

/*
 * martix_t functions
 */

double 
matrix_trace(matrix_t *self);

matrix_t *
matrix_transpose(matrix_t *self);

matrix_t *
matrix_mean(matrix_t *self);

matrix_t *
matrix_multiply(matrix_t *a, matrix_t *b);

matrix_t *
matrix_scale(matrix_t *self, double value);

void
matrix_swap_row(matrix_t *self, int p, int q);

matrix_t *
matrix_covariance(matrix_t *self);

matrix_t *
matrix_dot_product(matrix_t *a, matrix_t *b);

matrix_t *
matrix_dot_diagonal(matrix_t *a, matrix_t *b);

#endif /* MATRIX_H */
