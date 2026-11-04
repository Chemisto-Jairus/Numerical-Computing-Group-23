# Use the Library in Your Own Program

- **Headers**: include/numerical-computing/; one include for everything: #include "numerical-computing/numcomp.hpp".
- **Namespace numcomp**; link the library numcomp2 (target_link_libraries(... numcomp2) or -lnumcomp2).

- **Exceptions**: invalid_argument (bad parameter), out_of_range (point outside the data or interval), runtime_error (method failed: no convergence, singular or non-SPD matrix, non-finite result). Wrap calls in try/catch.

## Small example

```cpp 

/**
 * @file my_program.cpp
 * @brief An independent test program for the numcomp2 library.
 */

#include <iostream>
#include <cmath>

// Include the headers from your installed library
#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/numerical_differentiator.hpp"

int main() 
{
    numcomp::BasicRootFinder finder;
    numcomp::BasicIntegrator integrator;
    numcomp::NumericalDifferentiator diff;

    // 1. Find sqrt(2) by solving x^2 - 2 = 0
    auto f1 = [](double x) { return x * x - 2.0; };
    double root = finder.bisection(f1, 0.0, 2.0, 1e-6, 100);

    // 2. Integrate sin(x) from 0 to pi (exact area is 2)
    const double pi = std::acos(-1.0);
    auto f2 = [](double x) { return std::sin(x); };
    double integral = integrator.trapezoidal_rule(f2, 0.0, pi, 100);

    // 3. Find the slope of sin(x) at x = 0 (exact slope is 1)
    double slope = diff.central_difference(f2, 0.0, 1e-4);

    // Print the results
    std::cout << "sqrt(2)  = " << root << "\n";
    std::cout << "integral = " << integral << "\n";
    std::cout << "slope    = " << slope << "\n";

    return 0;
}

```

- To tell your compiler where to find the library;

### With g++

**For macOS/Linux(or git bash)**

```bash

g++ -std=c++17 -I "$HOME/Group23_Safe_Build/install/include" my_program.cpp -L "$HOME/Group23_Safe_Build/install/lib" -lnumcomp2 -o my_program
./my_program

```

**Windows Command Prompt**

```bash

g++ -std=c++17 -I "%USERPROFILE%\Group23_Safe_Build\install\include" my_program.cpp -L "%USERPROFILE%\Group23_Safe_Build\install\lib" -lnumcomp2 -o my_program.exe
.\my_program.exe

```