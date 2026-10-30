# Install the Library (if your CMake supports it) 

- (Installing means copy the three things another program needs into one folder (the prefix): the public headers, the static library, and the app. A program elsewhere on the computer can then #include the headers and link the library, without the source code.)

##  NB (To install the library you must build it) 

### Windows Installation

**How to install Cmake**

- Open Command prompt on your pc and enter this code

```bash

winget install -e --id Kitware.CMake

``` 

- Download the repository and save it anywhere on your pc
- Right click on it and select open Git bash here and paste the code below. 
- It should open the folder where the library that can be used it located 

```bash

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -r . "$SAFE_DIR"
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/adams_bashforth_solver.cpp \
src/adams_moulton_solver.cpp \
src/advanced_integrator.cpp \
src/advanced_ode_solver.cpp \
src/advanced_root_finder.cpp \
src/basic_integrator.cpp \
src/basic_ode_solver.cpp \
src/basic_root_finder.cpp \
src/conjugate_gradient_solver.cpp \
src/cubic_spline.cpp \
src/finite_difference_solver.cpp \
src/interpolator.cpp \
src/matrix_solver.cpp \
src/numerical_differentiator.cpp \
src/runge_kutta_fehlberg_solver.cpp"

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        echo "⚠️ Warning: Source file '$file' is missing. Creating a dummy file to prevent CMake errors..."
        touch "$file"
    fi
done

# ==========================================
# 3. CMAKE BUILD & INSTALL
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake with local install directory..."
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="../install" ..

echo ">>> Compiling with CMake..."
cmake --build .

echo ">>> Installing the library..."
cmake --install .

# ==========================================
# 4. FINISH & OPEN EXPLORER
# ==========================================
echo ">>> Opening install folder in Windows File Explorer..."
cd ../install
explorer .

cd "$ORIGINAL_DIR"
echo "✅ Finished! The library has been installed locally inside $SAFE_DIR/install."

```