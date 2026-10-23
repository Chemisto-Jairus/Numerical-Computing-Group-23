# 1. Numerical Computing 2 - C++ Library (Group 23)

The objective of this project is to build a reusable numerical computing library from scratch using C++. It provides a set of mathematical tools for finding roots, interpolating data, numerical integration, numerical differentiation, and solving ordinary differential equations (ODEs). We built this to demonstrate object-oriented programming concepts and collaborative software development.

## Table of Contents

- [2. Requirements and Dependencies](#2-requirements-and-dependencies)
- [3. Main Features](#3-main-features)
- [4. Project Structure](#4-project-structure)
- [5. Build the Library](#5-build-the-library)
- [6. Install the Library](#6-install-the-library-if-your-cmake-supports-it)
- [7. Use the Library in Your Own Program](#7-use-the-library-in-your-own-program)
- [8. Examples](#8-examples)
- [9. Run the Tests](#9-run-the-tests)
- [10. Limitations](#10-limitations)
- [11. Contributions by Group Members](#11-contributions-by-group-members)
- [12. How We Collaborated as a Team](#12-how-we-collaborated-as-a-team)

## 2. Requirements and Dependencies

- **Compiler:** needs C++17 support. 
- **CMake:** version 3.10 or newer. 
- **Git:** only to clone the repository. 
- **Outside libraries:** None, everything is written by us from scratch.

## 3. Main Features

- **Root Finding:** Find roots of equations using Bisection, Newton-Raphson, Secant, and Fixed-Point Iteration methods.
- **Interpolation & Approximation:** Estimate values between known data points using Linear Interpolation and Cubic Splines, plus Chebyshev Approximation for generating optimal nodes.
- **Numerical Integration:** Approximate definite integrals using the Trapezoidal Rule, Midpoint Rule, Simpson's Rule, and Romberg Integration.
- **Numerical Differentiation:** Approximate derivatives of given functions.
- **ODE Solvers:** Solve ordinary differential equations using Euler's Method, Runge-Kutta-Fehlberg, Adams-Bashforth, Adams-Moulton, and the Finite Difference Method.
- **Linear Algebra:** Solve systems of linear equations using the Conjugate Gradient method.

## 4. Project Structure

Use the following structure for your project:

```text
Numerical-Computing-Group-23/
│
├── README.md
├── CMakeLists.txt
├── LICENSE
├── .gitignore
│
├── include/
│   └── numerical-computing/
│       ├── advanced_integrator.hpp
│       ├── advanced_root_finder.hpp
│       ├── advanced_ode_solver.hpp
│       ├── basic_integrator.hpp
│       ├── basic_ode.hpp
│       ├── interpolator.hpp
│       ├── basic_root_finder.hpp
│       ├── cubic_spline.hpp
│       └── matrix_solver.hpp
│
├── src/
│   ├── advanced_integrator.cpp
│   ├── advanced_root_finder.cpp
│   ├── advanced_ode_solver.cpp
│   ├── basic_integrator.cpp
│   ├── basic_ode_solver.cpp
│   ├── interpolator.cpp
│   ├── main.cpp
│   ├── basic_root_finder.cpp
│   ├── cubic_spline.cpp
│   └── matrix_solver.cpp
│
├── tests/
│   ├── CMakeLists.txt
│   ├── run_all_tests.sh
│   ├── test_advanced_integrator.cpp
│   ├── test_advanced_ode.cpp (to be added in week 4)
│   ├── test_advanced_root_finder.cpp
│   ├── test_basic_integrator.cpp
│   ├── test_basic_interpolator.cpp
│   ├── test_basic_ode_solver.cpp (to be added in week 4)
│   ├── test_basic_root_finder.cpp (to be added in week 4)
│   ├── test_cubic_spline.cpp 
│   └── test_matrix-solver.cpp (to be added in week 4)
│
├── examples/ (to be added in week 4)
│   ├── example_advanced_integrator.cpp
│   ├── example_advanced_ode_solver.cpp
│   ├── example_advanced_roots_solver.cpp
│   ├── example_basic_integration.cpp
│   ├── example_basic_ode_solver.cpp
│   ├── example_interpolation.cpp
│   ├── example_cubic_spline.cpp 
│   ├── example_matrix_solver.cpp 
│   ├── example_basic_roots_solver.cpp
│   └── example_basic_root_finder.cpp  
│
└── reports/
    ├── documentation/
    │       ├──oop_concepts GROUP 23.md
    │       ├──Group 23 hand drawn classes layout .pdf
    │       └──Group 23 Inheritance documented.pdf
    │
    ├── week-01.md
    ├── week-02.md
    ├── week-03.md
    ├── week-04.md
    └── final-report.md (to be added in week 5)
```

## 5. Build the Library

### Windows

## OPTION A: (USING Cmake/Git bash)

**1. How to install Cmake**

- Open Command prompt on your pc and enter this code

```text

winget install -e --id Kitware.CMake

``` 

**2. Building the library**

- Run Git bash as administrator and right click on the repository folder and select open git bash here
- Paste the code below in your git bash terminal

```text

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
# Remove any old remnants and create a fresh copy
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"

# Copy all project contents to the safe folder
cp -r . "$SAFE_DIR"

# Jump into the safe folder to carry out operations
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/basic_root_finder.cpp src/advanced_root_finder.cpp src/interpolator.cpp src/cubic_spline.cpp src/basic_integrator.cpp src/advanced_integrator.cpp src/basic_ode_solver.cpp src/advanced_ode_solver.cpp src/matrix_solver.cpp"
MISSING_FILES=()

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        base_name=$(basename "$file" .cpp)
        MISSING_FILES+=("$base_name")
        echo "⚠️ Warning: Source file '$file' is missing. Skipping..."
        
        # TRICK: Create an empty dummy file in the safe folder so CMake 
        # doesn't crash if this file is hardcoded in CMakeLists.txt!
        touch "$file"
    fi
done

echo ">>> Filtering tests..."
for t in tests/test_*.cpp; do
    [ -e "$t" ] || continue 
    name=$(basename "$t" .cpp)
    
    for missing in "${MISSING_FILES[@]}"; do
        core_test_name="${name#test_}"
        if [[ "$core_test_name" == *"$missing"* || "$missing" == *"$core_test_name"* ]]; then
            echo "⏭️ Skipping test: $name (dependent source file is missing)"
            # Remove the test from the safe folder so CMake ignores it completely
            rm -f "$t"
            break
        fi
    done
done

# ==========================================
# 3. CMAKE BUILD
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake..."
cmake -G "MinGW Makefiles" ..

echo ">>> Compiling with CMake..."
cmake --build .

# ==========================================
# 4. RUN TESTS & OPEN EXPLORER
# ==========================================
echo ">>> Running automated tests..."
cd tests

# Check if any .exe tests were actually compiled before trying to run the script
if ls *.exe 1> /dev/null 2>&1; then
    bash ../../../Group23_Safe_Build/tests/run_all_tests.sh
else
    echo "❌ No tests were compiled. Skipping test runner."
fi

# Step back out of tests into the build folder
cd ..

echo ">>> Opening build folder in Windows File Explorer..."
explorer .

# Return to original directory in the terminal
cd "$ORIGINAL_DIR"
echo "✅ Finished! Your root directory remained entirely untouched."

```

- It should open the folder where the build was created and you can run the numcomp_app.


### Linux and macOS (NOT YET TESTED)



## 6. Install the Library (if your CMake supports it)

### Linux and macOS

*(Placeholder - will be added later)*

### Windows

*(Placeholder - will be added later)*

## 7. Use the Library in Your Own Program

*(Placeholder - we will add a quick start guide here showing how to link the compiled library once it is fully built)*

## 8. Examples

*(Placeholder - in Week 4, we will add instructions here on how to run the demo files from the `examples/` folder)*

## 9. Run the Tests

## OPTION A: (USING Cmake/Git bash)

**1. How to install Cmake**

- Open Command prompt on your pc and enter this code

```text

winget install -e --id Kitware.CMake

``` 

**2. Building the library**

- Run Git bash as administrator and right click on the repository folder and select open git bash here
- Paste the code below in your git bash terminal

```text

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
# Remove any old remnants and create a fresh copy
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"

# Copy all project contents to the safe folder
cp -r . "$SAFE_DIR"

# Jump into the safe folder to carry out operations
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/basic_root_finder.cpp src/advanced_root_finder.cpp src/interpolator.cpp src/cubic_spline.cpp src/basic_integrator.cpp src/advanced_integrator.cpp src/basic_ode_solver.cpp src/advanced_ode_solver.cpp src/matrix_solver.cpp"
MISSING_FILES=()

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        base_name=$(basename "$file" .cpp)
        MISSING_FILES+=("$base_name")
        echo "⚠️ Warning: Source file '$file' is missing. Skipping..."
        
        # TRICK: Create an empty dummy file in the safe folder so CMake 
        # doesn't crash if this file is hardcoded in CMakeLists.txt!
        touch "$file"
    fi
done

echo ">>> Filtering tests..."
for t in tests/test_*.cpp; do
    [ -e "$t" ] || continue 
    name=$(basename "$t" .cpp)
    
    for missing in "${MISSING_FILES[@]}"; do
        core_test_name="${name#test_}"
        if [[ "$core_test_name" == *"$missing"* || "$missing" == *"$core_test_name"* ]]; then
            echo "⏭️ Skipping test: $name (dependent source file is missing)"
            # Remove the test from the safe folder so CMake ignores it completely
            rm -f "$t"
            break
        fi
    done
done

# ==========================================
# 3. CMAKE BUILD
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake..."
cmake -G "MinGW Makefiles" ..

echo ">>> Compiling with CMake..."
cmake --build .

# ==========================================
# 4. RUN TESTS & OPEN EXPLORER
# ==========================================
echo ">>> Running automated tests..."
cd tests

# Check if any .exe tests were actually compiled before trying to run the script
if ls *.exe 1> /dev/null 2>&1; then
    bash ../../../Group23_Safe_Build/tests/run_all_tests.sh
else
    echo "❌ No tests were compiled. Skipping test runner."
fi

# Step back out of tests into the build folder
cd ..

echo ">>> Opening build folder in Windows File Explorer..."
explorer .

# Return to original directory in the terminal
cd "$ORIGINAL_DIR"
echo "✅ Finished! Your root directory remained entirely untouched."

```

- It should open the folder where the build was created and you can run the numcomp_app.

## OPTION B : (USING g++/git bash)

**Follow these steps**

**1. Check your tools (once)**

```text

g++ --version        # needs C++17 support: g++ 8 or newer
cmake --version      # needs 3.10 or newer
git --version

```

**NB**

- Make sure controlled folder access in windows security is turned off for the directory you put the folder or else git bash will throw errors

- Run git bash as administrator and open the root repository folder
- Once you are inside the root folder. paste this code in your git bash terminal

```text

# ==========================================
# 1. SETUP SANDBOX ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SANDBOX_DIR="$HOME/git_bash_sandbox"

echo "📦 Creating restriction-free sandbox at: $SANDBOX_DIR"
# Remove any old sandbox remnants and create a fresh copy
rm -rf "$SANDBOX_DIR"
mkdir -p "$SANDBOX_DIR"

# Copy all project contents to the sandbox
cp -r . "$SANDBOX_DIR"

# Jump into the sandbox to carry out operations
cd "$SANDBOX_DIR"

# ==========================================
# 2. GENERALIZED COMPILE AND TEST LOGIC
# ==========================================
LIB="src/basic_root_finder.cpp src/advanced_root_finder.cpp src/interpolator.cpp src/cubic_spline.cpp src/basic_integrator.cpp src/advanced_integrator.cpp src/basic_ode_solver.cpp src/advanced_ode_solver.cpp src/matrix_solver.cpp"

VALID_LIB=""
MISSING_FILES=()

# Filter out missing files dynamically
for file in $LIB; do
    if [ -f "$file" ]; then
        VALID_LIB="$VALID_LIB $file"
    else
        base_name=$(basename "$file" .cpp)
        MISSING_FILES+=("$base_name")
        echo "⚠️ Warning: Source file '$file' is missing. Skipping..."
    fi
done

COMPILED_EXES=()

# Compile tests into the sandbox's tests/ folder
for t in tests/test_*.cpp; do
    name=$(basename "$t" .cpp)
    
    skip_test=false
    for missing in "${MISSING_FILES[@]}"; do
        core_test_name="${name#test_}"
        if [[ "$core_test_name" == *"$missing"* || "$missing" == *"$core_test_name"* ]]; then
            skip_test=true
            break
        fi
    done

    if [ "$skip_test" = true ]; then
        echo "⏭️ Skipping test: $name (dependent source file is missing)"
        continue
    fi

    echo "🔨 Building executable: tests/$name"
    if g++ -std=c++17 -Wall -Wextra -I include "$t" $VALID_LIB -o "tests/$name"; then
        COMPILED_EXES+=("tests/$name")
    fi
done

# Run the test runner script inside the sandbox environment
if [ ${#COMPILED_EXES[@]} -gt 0 ]; then
    cd tests
    bash run_all_tests.sh
    cd ..
else
    echo "❌ No tests were compiled. Skipping test runner."
fi

# ==========================================
# 3. TEARDOWN AND SANITIZATION
# ==========================================
echo "🧹 Tearing down sandbox and restoring focus..."
cd "$ORIGINAL_DIR"
rm -rf "$SANDBOX_DIR"
echo "✅ Finished! Your root directory remained entirely untouched."


```

- It should display all tests passed

## 10. Limitations

- Romberg is capped at 25 iterations
- Linear interpolation needs strictly increasing x and does not extrapolate
- Simpson needs an even n
- Root finders can fail to converge
- ODE solvers use a fixed step and a 1,000,000-step cap
- Gaussian elimination is for small dense matrices and rejects singular ones
- Differentiation, Adams methods and conjugate gradient were not yet built

## 11. Contributions by Group Members

| Name | Student Number | Project Role / Assigned Module |
| :--- | :--- | :--- |
| CHEMISTO JAIRUS | 25/U/07928/PS | Member 1 (Lead) / Basic Root Finding |
| BATTE DERRICK CALVIN | 25/U/0717 | Member 2 / Interpolation (Linear, Chebyshev, Spline) |
| Aogon Sharon | 25/U/08690/PS | Member 3 / Basic Numerical Integration |
| BAKALU VALERIA | 25/U/07924/PS | Member 4 / Advanced Root Finding |
| WASSWA GEORGE MPANGA | 25/U/08793/PS | Member 5 / Advanced Numerical Integration |
| MWANJE IAN | 25/U/08757/PS | Member 6 / Basic ODE Solvers |
| MUWANGUZI SAMUEL | 25/U/0843 | Member 7 / Advanced ODE Solvers |
| KYEYUNE PRAISE PAULINE | 25/U/07943/PS | Member 8 / Linear Algebra & Unit Testing |
| Nabakooza Bridget Maria | 25/U/07592/PS | Member 9 / Demo Examples & Final Documentation |

## 12. How We Collaborated as a Team

Because we have a large group of 9 people, we had to be very organized so we didn't overwrite each other's code. Here is how we divided the work and collaborated:

- **Division of Work:** We broke the project into logical modules and divided the workload across all 9 group members. We assigned 7 members to write the core C++ math modules, 1 member to build the unit testing framework, and 1 member to handle the final examples and documentation.
- **Version Control Strategy:** We used Git and GitHub. Our Project Lead (Chemisto Jairus) set up the initial repository, the empty folder structure (`include`, `src`, `tests`, `examples`, `reports`), and the basic `CMakeLists.txt` on the `main` branch. 
- **Branching and Pull Requests:** The rest of the team cloned the repository to their local machines. Nobody is allowed to push code directly to `main`. Instead, everyone creates a new branch for their specific assigned module (for example, `feature-interp` for interpolation). Once their C++ code works locally, they push their branch to GitHub and open a Pull Request (PR).
- **Code Review:** Before a PR is merged into the main branch, at least one other group member reviews the code on GitHub, leaves a comment confirming it looks correct, and then hits the merge button. This ensures everyone understands how the whole library is coming together, not just their own part.