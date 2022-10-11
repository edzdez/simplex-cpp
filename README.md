# simplex-cpp

An implementation of Dantzig's [Simplex Method](https://en.wikipedia.org/wiki/Simplex_algorithm) in C++
with [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
and [toml++](https://marzer.github.io/tomlplusplus/index.html).

## Usage:

```shell
$ ./build/lp-solver
Usage: ./build/lp-solver <filename>

$ ./build/lp-solver example_1.toml
Initial Tableau:
 2  2  1  1  0  0  0 60
 1  1  3  0  1  0  0 40
 4  2  5  0  0  1  0 80
-2 -3 -5  0  0  0  1  0

Pivoting on {1, 2}
New Tableau:
  5   5   0   3  -1   0   0 140
  1   1   3   0   1   0   0  40
  7   1   0   0  -5   3   0  40
 -1  -4   0   0   5   0   3 200

Pivoting on {0, 1}
New Tableau:
   5    5    0    3   -1    0    0  140
   0    0   15   -3    6    0    0   60
  30    0    0   -3  -24   15    0   60
  15    0    0   12   21    0   15 1560

Final Tableau:
   1    1    0  0.6 -0.2    0    0   28
   0    0    1 -0.2  0.4    0    0    4
   2    0    0 -0.2 -1.6    1    0    4
   1    0    0  0.8  1.4    0    1  104

================================================

Answer Report:
Objective Value: 104

Decision Values:
 0 28  4

Slack Values:
0 0 4

Sensitivity Report:
Reduced Cost:
1 0 0

Shadow Price:
0.8 1.4   0

================================================
```

## Dependencies

* eigen3 (3.4.0)
* toml++ (header-only, included in the repo)

## Build

This project was built with C++20 and [meson](https://mesonbuild.com). Only tested on Fedora 36 with gcc 12.2 and Eigen 3.4.

```shell
# clone the repo
$ git clone https://github.com/edzdez/simplex-cpp.git
$ cd simplex-cpp

# Generate build files
$ meson build

# Build
$ meson compile -C build
$ ./build/lp-solver <filename>
```
