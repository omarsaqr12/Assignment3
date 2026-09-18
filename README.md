# City distance with a binary search tree (C++ course assignment)

A C++ data-structures exercise: load city names and geographic coordinates from the supplied CSV into a binary search tree, look up two cities, and compute their approximate **great-circle (straight-line) distance** with the haversine formula. It does **not** compute driving routes or travel time. This repository preserves the original assignment data and improves portability, coordinate correctness, and tree ownership.

## Build and run

With a C++17 compiler, from the repository root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -I. main.cpp -o city_distance
./city_distance MCity.csv
```

On Windows, run `city_distance.exe`. You may omit the file argument to use `MCity.csv` in the current working directory. Enter each city on a separate line; spaces in names are supported. For a non-interactive example:

```sh
printf 'Kandahar\nKabul\n' | ./city_distance MCity.csv
```

The program reports an error and returns a non-zero status if a file cannot be read, a requested city is absent, or its coordinate text is invalid.

## Test

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/city_tests.cpp -o city_tests
./city_tests
```

The regression tests exercise hemisphere signs, fractional minutes and seconds, invalid coordinates, haversine reference cases, empty-tree traversals, duplicate insertion, lookup, root/child deletion and traversal of more than the legacy 128-node stack capacity. GitHub Actions also compiles and runs these tests on pull requests and pushes to `main`, checks a multiword city lookup in the supplied CSV, and runs the tests under an undefined-behavior sanitizer. These are targeted checks, not independent verification of every row in the supplied dataset.

## How it works

1. [`main.cpp`](main.cpp) reads [`MCity.csv`](MCity.csv) and indexes each city name in `binaryTree<string,string>`; the stored value is the original latitude, longitude and country text.
2. [`BinaryTree.h`](BinaryTree.h) and [`binaryTree.cpp`](binaryTree.cpp) implement insertion, lookup, deletion, iteration and cleanup. Template definitions are included directly by the entry point; **compile `main.cpp`**, not `binaryTree.cpp` as an independent source file.
3. [`Coordinates.h`](Coordinates.h) parses the dataset's degrees/minutes/hemisphere notation (such as `31d37lN` and `65d43lE`) using floating-point arithmetic and correct N/S/E/W signs, then applies the spherical haversine calculation. The original radius of 6371.137 km is retained for comparison with the original exercise.
4. [`tests/city_tests.cpp`](tests/city_tests.cpp) provides repeatable assertions.

## Scope and limitations

- City lookup is case-sensitive. Duplicate city names retain the **first** CSV occurrence; the tool cannot disambiguate homonymous cities by country.
- The CSV reader expects the supplied simple five-column layout and does not implement full RFC-style quoted-field parsing. Coordinates are validated when a requested pair is evaluated, not by exhaustively auditing the entire dataset.
- A plain unbalanced BST can have O(n) insertion and lookup on unfortunate input orders. The distance is an approximate spherical estimate, not road, flight or geodesic-on-ellipsoid distance.
- `preorder()` and `levelorder()` safely write to standard output rather than to an absolute Windows path. The committed [`output.txt`](output.txt) is a historical output sample, **not** regenerated automatically.
- [`Stackt.h`](Stackt.h), [`Queuet.h`](Queuet.h) and their `.cpp` files are legacy course helpers; the current tree traversals use standard-library containers instead. The committed Windows `.exe` files, VS Code debugger settings and temporary runner file are historical artifacts; use the documented source build instead.

## Attribution and provenance

The project includes course-provided data and data-structure helper scaffolding alongside student-authored exercise code. The exact original authorship of every helper is not independently established. Original files and Git history are retained rather than inventing a production claim, independent benchmark or course-grade result.
