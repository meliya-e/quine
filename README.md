# Quine-McCluskey Algorithm Implementation

This project implements the Quine-McCluskey algorithm for minimizing partially defined Boolean functions. The algorithm finds the minimal sum-of-products form of a Boolean function.

## Building the Project

The project uses CMake for building. To build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Program

After building, run the program:

```bash
./QuineMcCluskey
```

The program will prompt you for:
1. Number of variables in the Boolean function
2. Number of minterms
3. The minterms (space-separated)
4. Number of don't cares
5. The don't cares (space-separated)

## Example

For a Boolean function with:
- 3 variables (A, B, C)
- Minterms: 0, 1, 2, 5, 6, 7
- Don't cares: 3, 4

Input would be:
```
Enter number of variables: 3
Enter number of minterms: 6
Enter minterms (space-separated): 0 1 2 5 6 7
Enter number of don't cares: 2
Enter don't cares (space-separated): 3 4
```

The program will output:
1. All prime implicants
2. Essential prime implicants
3. The minimized Boolean expression

## Features

- Handles partially defined Boolean functions (with don't cares)
- Finds all prime implicants
- Identifies essential prime implicants
- Generates minimized Boolean expressions
- Supports any number of variables 