# 1. Numerical Computing 2 - C++ Library (Group 23)

The objective of this project is to build a reusable numerical computing library from scratch using C++. It provides a set of mathematical tools for finding roots, interpolating data, numerical integration, numerical differentiation, and solving ordinary differential equations (ODEs). We built this to demonstrate object-oriented programming concepts and collaborative software development.

## Table of Contents

- [2. Requirements and Dependencies](#2-requirements-and-dependencies)
- [3. The library at a glance](#3-the-library-at-a-glance-to-be-finalized)
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

## 3. The library at a glance to be finalized

| Class | Methods | Module guide |
|---|---|---|
| `BasicRootFinder` | Bisection, Newton-Raphson | [modules/basic_root_finder.md](modules/basic_root_finder.md) |
| `AdvancedRootFinder` | Secant, Fixed-point iteration | [modules/advanced_root_finder.md](modules/advanced_root_finder.md) |
| `Interpolator` | Linear interpolation, Chebyshev nodes, Chebyshev approximation | [modules/interpolator.md](modules/interpolator.md) |
| `CubicSpline` | Natural cubic spline | [modules/cubic_spline.md](modules/cubic_spline.md) |
| `NumericalDifferentiator` | Forward, backward, central differences, second derivative, Richardson | [modules/numerical_differentiator.md](modules/numerical_differentiator.md) |
| `BasicIntegrator` | Trapezoidal, Midpoint | [modules/basic_integrator.md](modules/basic_integrator.md) |
| `AdvancedIntegrator` | Simpson's 1/3, Romberg | [modules/advanced_integrator.md](modules/advanced_integrator.md) |
| `BasicODESolver` | Euler, Heun | [modules/basic_ode_solver.md](modules/basic_ode_solver.md) |
| `AdvancedODESolver` | RK3, RK4 | [modules/advanced_ode_solver.md](modules/advanced_ode_solver.md) |
| `AdamsBashforthSolver` | Adams-Bashforth 2 and 4 | [modules/adams_bashforth_solver.md](modules/adams_bashforth_solver.md) |
| `AdamsMoultonSolver` | Adams-Moulton 2 and 4 | [modules/adams_moulton_solver.md](modules/adams_moulton_solver.md) |
| `RungeKuttaFehlbergSolver` | Adaptive RKF45 | [modules/runge_kutta_fehlberg_solver.md](modules/runge_kutta_fehlberg_solver.md) |
| `FiniteDifferenceSolver` | Finite difference method for boundary value problems | [modules/finite_difference_solver.md](modules/finite_difference_solver.md) |
| `MatrixSolver` | Gaussian elimination, LU decomposition | [modules/matrix_solver.md](modules/matrix_solver.md) |
| `ConjugateGradientSolver` | Conjugate gradient | [modules/conjugate_gradient_solver.md](modules/conjugate_gradient_solver.md) |

## 4. Project Structure

We used the following structure for our project:

```text
Numerical-Computing-Group-23/
├── documentation/
│   ├── helpers-report.md (to be added)
│   │   
│   │   
│   ├── modules/ (to be added)
│   │   ├── adams_bashforth_solver.md
│   │   ├── adams_moulton_solver.md
│   │   ├── advanced_integrator.md
│   │   ├── advanced_ode_solver.md
│   │   ├── advanced_root_finder.md
│   │   ├── basic_integrator.md
│   │   ├── basic_ode_solver.md
│   │   ├── basic_root_finder.md
│   │   ├── command_line_app.md
│   │   ├── conjugate_gradient_solver.md
│   │   ├── cubic_spline.md
│   │   ├── examples.md
│   │   ├── finite_difference_solver.md
│   │   ├── interpolator.md
│   │   ├── matrix_solver.md
│   │   ├── numerical_differentiator.md
│   │   ├── runge_kutta_fehlberg_solver.md
│   │   ├── shared_helpers.md
│   │   └── test_framework.md
│   ├── Doxyfile (to be added)
│   ├── README.md (to be added)
│   ├── algorithm-report.md (to be added)
│   └── testing.md (to be added)
├── examples/
│   ├── CMakeLists.txt
│   ├── example_adams_bashforth_solver.cpp
│   ├── example_adams_bashforth_solver_applied.cpp
│   ├── example_adams_moulton_solver.cpp
│   ├── example_adams_moulton_solver_applied.cpp
│   ├── example_advanced_integrator.cpp
│   ├── example_advanced_integrator_applied.cpp
│   ├── example_advanced_ode_solver.cpp
│   ├── example_advanced_ode_solver_applied.cpp
│   ├── example_advanced_root_finder.cpp
│   ├── example_advanced_root_finder_applied.cpp
│   ├── example_basic_integrator.cpp
│   ├── example_basic_integrator_applied.cpp
│   ├── example_basic_ode_solver.cpp
│   ├── example_basic_ode_solver_applied.cpp
│   ├── example_basic_root_finder.cpp
│   ├── example_basic_root_finder_applied.cpp
│   ├── example_conjugate_gradient_solver.cpp
│   ├── example_conjugate_gradient_solver_applied.cpp
│   ├── example_cubic_spline.cpp
│   ├── example_cubic_spline_applied.cpp
│   ├── example_finite_difference_solver.cpp
│   ├── example_finite_difference_solver_applied.cpp
│   ├── example_helpers.hpp
│   ├── example_interpolator.cpp
│   ├── example_interpolator_applied.cpp
│   ├── example_matrix_solver.cpp
│   ├── example_matrix_solver_applied.cpp
│   ├── example_numerical_differentiator.cpp
│   ├── example_numerical_differentiator_applied.cpp
│   ├── example_runge_kutta_fehlberg_solver.cpp
│   └── example_runge_kutta_fehlberg_solver_applied.cpp
├── include/
│   └── numerical-computing/
│       ├── adams_bashforth_solver.hpp (to be added)
│       ├── adams_moulton_solver.hpp (to be added)
│       ├── advanced_integrator.hpp
│       ├── advanced_ode_solver.hpp
│       ├── advanced_root_finder.hpp
│       ├── basic_integrator.hpp
│       ├── basic_ode_solver.hpp
│       ├── basic_root_finder.hpp
│       ├── conjugate_gradient_solver.hpp (to be added)
│       ├── cubic_spline.hpp
│       ├── finite_difference_solver.hpp (to be added)
│       ├── interpolator.hpp
│       ├── matrix_solver.hpp
│       ├── numcomp.hpp (to be added)
│       ├── numerical_differentiator.hpp (to be added)
│       └── runge_kutta_fehlberg_solver.hpp (to be added)
├── reports/
│   ├── week-01.md
│   ├── week-02.md
│   ├── week-03.md
│   ├── week-04.md
│   └── final report (to be added)
├── src/
│   ├── app/ (to be added)
│   │   ├── catalog.cpp
│   │   ├── catalog.hpp
│   │   ├── console_ui.cpp
│   │   ├── console_ui.hpp
│   │   ├── module_menus.cpp
│   │   └── module_menus.hpp
│   ├── internal/
│   │   ├── input_checks.cpp
│   │   ├── input_checks.hpp
│   │   ├── multistep.cpp (to be added)
│   │   ├── multistep.hpp (to be added)
│   │   ├── ode_stepper.cpp
│   │   ├── ode_stepper.hpp
│   │   ├── tridiagonal.cpp (to be added)
│   │   └── tridiagonal.hpp (to be added)
│   ├── adams_bashforth_solver.cpp (to be added)
│   ├── adams_moulton_solver.cpp (to be added)
│   ├── advanced_integrator.cpp 
│   ├── advanced_ode_solver.cpp
│   ├── advanced_root_finder.cpp
│   ├── basic_integrator.cpp
│   ├── basic_ode_solver.cpp
│   ├── basic_root_finder.cpp
│   ├── conjugate_gradient_solver.cpp (to be added)
│   ├── cubic_spline.cpp
│   ├── finite_difference_solver.cpp (to be added)
│   ├── interpolator.cpp
│   ├── main.cpp
│   ├── matrix_solver.cpp
│   ├── numerical_differentiator.cpp (to be added)
│   └── runge_kutta_fehlberg_solver.cpp (to be added)
├── tests/
│   ├── CMakeLists.txt
│   ├── run_all_tests.sh (to be updated by Pauline)
│   ├── test_adams_bashforth_solver.cpp (to be added)
│   ├── test_adams_moulton_solver.cpp (to be added)
│   ├── test_advanced_integrator.cpp
│   ├── test_advanced_ode_solver.cpp
│   ├── test_advanced_root_finder.cpp
│   ├── test_basic_integrator.cpp
│   ├── test_basic_ode_solver.cpp
│   ├── test_basic_root_finder.cpp
│   ├── test_conjugate_gradient_solver.cpp (to be added)
│   ├── test_cubic_spline.cpp
│   ├── test_finite_difference_solver.cpp (to be added)
│   ├── test_helpers.hpp
│   ├── test_interpolator.cpp
│   ├── test_matrix_solver.cpp
│   ├── test_numerical_differentiator.cpp (to be added)
│   └── test_runge_kutta_fehlberg_solver.cpp (to be added)
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
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

- (Installing means copy the three things another program needs into one folder (the prefix): the public headers, the static library, and the app. A program elsewhere on the computer can then #include the headers and link the library, without the source code.)

##    NB (To install the library you must build it) 

### Windows

**How to install Cmake**

- Open Command prompt on your pc and enter this code

```text

winget install -e --id Kitware.CMake

``` 

- Download the repository and save it anywhere on your pc
- Right click on it and select open Git bash here and paste the code below. 


```text

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"
INSTALL_DIR="$SAFE_DIR/numcomp2"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -r . "$SAFE_DIR"
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES TO PREVENT COMPILER CRASHES
# ==========================================
LIB="src/basic_root_finder.cpp src/advanced_root_finder.cpp src/interpolator.cpp src/cubic_spline.cpp src/basic_integrator.cpp src/advanced_integrator.cpp src/basic_ode_solver.cpp src/advanced_ode_solver.cpp src/matrix_solver.cpp"
MISSING_FILES=()

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        base_name=$(basename "$file" .cpp)
        MISSING_FILES+=("$base_name")
        echo "⚠️ Warning: Source file '$file' is missing. Creating dummy file..."
        touch "$file"
    fi
done

echo ">>> Removing broken tests so compilation succeeds..."
for t in tests/test_*.cpp; do
    [ -e "$t" ] || continue 
    name=$(basename "$t" .cpp)
    
    for missing in "${MISSING_FILES[@]}"; do
        core_test_name="${name#test_}"
        if [[ "$core_test_name" == *"$missing"* || "$missing" == *"$core_test_name"* ]]; then
            rm -f "$t"
            break
        fi
    done
done

# ==========================================
# 3. CMAKE BUILD & INSTALL
# ==========================================
echo ">>> Configuring CMake (Release Mode)..."
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

echo ">>> Compiling with CMake..."
cmake --build build

echo ">>> Installing library..."
cmake --install build --prefix "$INSTALL_DIR"

# ==========================================
# 4. FINISH & OPEN EXPLORER
# ==========================================
echo ">>> Opening the INSTALLED folder in Windows File Explorer..."
# Convert the Git Bash path to a Windows path so Explorer can read it
WIN_PATH=$(cygpath -w "$INSTALL_DIR")
explorer "$WIN_PATH"

# Return to original directory in the terminal
cd "$ORIGINAL_DIR"
echo "✅ Finished! Your library is built and installed inside the Safe Build folder."

```

- It should open the folder where the library that can be used it located 


### Linux and macOS

*(Placeholder - will be added later)*

### Windows

*(Placeholder - will be added later)*

## 7. Use the Library in Your Own Program

*(Placeholder - we will add a quick start guide here showing how to link the compiled library once it is fully built)*

## 8. Examples

### Basic root finder

**A. Standard Equations**

[Equation and values tested](<documentation/examples results/basic_root_finder.md#equations-and-values-tested>)

[Error Handling & Edge Cases](<documentation/examples results/basic_root_finder.md#error-handling--edge-cases>)

**B. Applied Problems**

[Equation and values tested](<documentation/examples results/basic_root_finder.md#equations-and-values-tested-applied>)

### Advanced_root_finder

### Basic integrator

### Advanced integrator

### Basic_ode_solver

### Advanced ode solver

### Cubic_spline

### Interpolator

### Matrix_solver


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

## 11. Contributions by Group Members

| Name | Student Number | Project Role / Assigned Module |
| :--- | :--- | :--- |
| CHEMISTO JAIRUS | 25/U/07928/PS | Member 1 (Lead) / Basic Root Finding / Numerical Differentiation|
| BATTE DERRICK CALVIN | 25/U/0717 | Member 2 / Interpolation (Linear, Chebyshev, Spline) / Matrix Solvers |
| Aogon Sharon | 25/U/08690/PS | Member 3 / Basic Numerical Integration / Finite Difference Method |
| BAKALU VALERIA | 25/U/07924/PS | Member 4 / Advanced Root Finding / Conjugate Gradient |
| WASSWA GEORGE MPANGA | 25/U/08793/PS | Member 5 / Advanced Numerical Integration / Runge-Kutta-Fehlberg (RKF45) |
| MWANJE IAN | 25/U/08757/PS | Member 6 / Basic ODE Solvers / Adams-Bashforth (AB2, AB4) |
| MUWANGUZI SAMUEL | 25/U/0843 | Member 7 / Advanced ODE Solvers / Adams-Moulton (AM2, AM4) |
| KYEYUNE PRAISE PAULINE | 25/U/07943/PS | Member 8 / Linear Algebra & Unit Testing |
| Nabakooza Bridget Maria | 25/U/07592/PS | Member 9 / Demo Examples & Final Documentation |

## 12. How We Collaborated as a Team

Because we have a large group of 9 people, we had to be very organized so we didn't overwrite each other's code. Here is how we divided the work and collaborated:

- **Division of Work:** We broke the project into logical modules and divided the workload across all 9 group members. We assigned 7 members to write the core C++ math modules, 1 member to build the unit testing framework, and 1 member to handle the final examples and documentation.
- **Version Control Strategy:** We used Git and GitHub. Our Project Lead (Chemisto Jairus) set up the initial repository, the empty folder structure (`include`, `src`, `tests`, `examples`, `reports`), and the basic `CMakeLists.txt` on the `main` branch. 
- **Branching and Pull Requests:** The rest of the team cloned the repository to their local machines. Nobody is allowed to push code directly to `main`. Instead, everyone creates a new branch for their specific assigned module (for example, `feature-interp` for interpolation). Once their C++ code works locally, they push their branch to GitHub and open a Pull Request (PR).
- **Code Review:** Before a PR is merged into the main branch, at least one other group member reviews the code on GitHub, leaves a comment confirming it looks correct, and then hits the merge button. This ensures everyone understands how the whole library is coming together, not just their own part.