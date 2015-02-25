#ifndef __ISOMAP_MATRIX
#define __ISOMAP_MATRIX

typedef struct _matrix {
    int height;
    int width;
    double* data;
} matrix;

//============================
// Stuff I wish C had.
//============================
void assert(int assertion, char* message);

//============================
// Catch and release functions
//============================
matrix* readMatrix(char* filename);
matrix* makeMatrix(int width, int height);
matrix* copyMatrix(matrix* m);
void freeMatrix(matrix* m);
void writeMatrix(matrix* m, char* filename);
void printMatrix(matrix* m);

//============================
// Basic Matrix operations
//============================
matrix* eyeMatrix(int n);
double traceMatrix(matrix* m);
matrix* transposeMatrix(matrix* m);
matrix* meanMatrix(matrix* m);
matrix* multiplyMatrix(matrix* a, matrix* b);
matrix* scaleMatrix(matrix* m, double value);
matrix* covarianceMatrix(matrix* m);
void rowSwap(matrix* a, int p, int q); // This method changes the input matrix.
matrix* dotProductMatrix(matrix* a, matrix* b);
matrix* dotDiagonalMatrix(matrix* a, matrix* b);
matrix* L2_distance(matrix* a, matrix* b);

#endif
