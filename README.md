# matrix

[![Build Status](https://travis-ci.org/e-k-m/matrix.svg?branch=master)](https://travis-ci.org/e-k-m/matrix)

> simple C matrix implementation

[Installation](#installation) | [Getting Up And Running](#getting-up-and-running) | [Examples](#examples) | [API](#api) | [See Also](#see-also)

matrix is a simple C matrix implementation. The main feature are:

- Currently just the data structure and some simple operators on it.

## Installation

```bash
# using clib
clib install e-k-m/matrix
# else ...
```

## Getting Up and Running

```bash
make <target>
```

## Examples

```c
matrix_t *m = matrix_new(20, 10);
assert(matrix_get(a, 0, 0) == 0.0);
```

## API

see [matrix.h](./src/matrix.h)

## See Also

- [a similar library](https://github.com/jcchurch/C-Linear-Algebra)