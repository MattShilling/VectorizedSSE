# VectorizedSSE

Vectorized Array Multiplication/Reduction using SSE

# Usage:

Running VectorizedSSE is fairly straightforward.

- `./VectorizedSSE <array size> [-s]`
    - The option `-s` will append the array size and speedup to a CSV file, `records.csv`.
    
- `./test.sh`: Run this to generate a full test, with array sizes varying from 1K to 1M floats.

# Requirements
- OpenMP
- An Intel chip with SSE (xmmintrin.h)

# To Build
- `make`

Or if any changes were made to dependencies:

- `cmake CMakeLists.txt`
- `make` or `cmake --build .`


