# Doxygen comments used in the project

Every style and tag, where each is used, with real examples, and how to generate the docs.

## Project Stats

* **93 / 93**: source files with a Doxygen file header
* **0**: Doxygen warnings, with every "undocumented" warning switched on
* **371**: `@param` lines (every parameter documented)
* **82**: `@throws` lines (every exception documented)

> **Why:** Section 5 of the project instructions says "each major module should also be sufficiently documented so that another C++ programmer can understand how to use it". Doxygen turns these comments into browsable HTML documentation (`docs/api/html`).

---

## 1. Comment styles used

| Style | Used for | Count |
| :--- | :--- | :--- |
| `/** … */` | Block comment before a file, class or function. Used for anything with tags. | 283 |
| `/// …` | One-line comment, for short items (constants, small helpers, test functions). | 196 |
| `///< …` | Comment after a struct member, on the same line. | 46 |

## 2. Every tag used

| Tag | Meaning | Where | Count |
| :--- | :--- | :--- | :--- |
| `@file` | Names the file and starts its description. Every file has one. | every file | 93 |
| `@brief` | One-sentence summary shown in lists and tooltips. | files, classes, functions, namespaces | 311 |
| `@details` | Longer explanation: the algorithm or formula. | headers: the maths of each method | 34 |
| `@author` | Who owns the file (the member who wrote it). | every file | 93 |
| `@param name` | One line per parameter: what it is and its allowed range. | every function with parameters | 371 |
| `@tparam Name` | A template parameter (for example the exception type in expect_throw). | templates in tests, examples, app | 8 |
| `@return` | What the function gives back. | every non-void function | 163 |
| `@throws Type` | Which exception is thrown, and exactly when. | every public method that validates input | 82 |
| `@code … @endcode` | A small usage example, shown formatted in the docs. | class headers, numcomp.hpp, test_helpers.hpp | 19 |
| `@note` | An extra remark the user should know. | Romberg's 25-row cap | 1 |
| `@see` | Cross-reference to a related class. | Interpolator ↔ CubicSpline, MatrixSolver ↔ ConjugateGradientSolver, Adams-Bashforth ↔ Adams-Moulton | 5 |
| `@namespace` | Documents a namespace once. | numcomp, checks, ode, linalg, test, demo, app::ui, app::catalog | 8 |

## 3. Coverage by area

| Area | Files | @brief | @param | @return | @throws | @tparam |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Public headers (`include/`) | 16 | 67 | 147 | 35 | 60 | 0 |
| Library sources (`src/*.cpp`) | 15 | 31 | 50 | 14 | 3 | 0 |
| Internal helpers (`src/internal/`) | 8 | 26 | 40 | 5 | 14 | 0 |
| Command-line app (`src/main.cpp`, `src/app/`) | 7 | 44 | 59 | 28 | 5 | 2 |
| Tests (`tests/`) | 16 | 70 | 52 | 49 | 0 | 3 |
| Examples (`examples/`) | 31 | 73 | 23 | 32 | 0 | 3 |

*Note: `@throws` appears mainly in the public headers and internal helpers: those are where the rules are defined. The `.cpp` files implement those declarations and refer back to them.*

## 4. The patterns, with real examples from the code

### File header (every file)

```cpp
/**
 * @file cubic_spline.hpp
 * @brief Natural cubic spline interpolation.
 * @author Batte Derrick Calvin (Member 2)
 */
```

### Public method (headers in include/)

```cpp
/**
 * @brief Finds a root of f inside [a, b] by repeatedly halving the interval.
 *
 * @details f(a) and f(b) must have opposite signs, so by the intermediate value
 * theorem a root lies between them. ...
 *
 * @param f         The function whose root is wanted.
 * @param a         One end of the interval.
 * @param b         The other end of the interval.
 * @param tol       Stop when |f(c)| < tol or the half-width is below tol. Must be > 0.
 * @param max_iter  Maximum number of halvings. Must be > 0.
 * @return An x with f(x) close to 0.
 *
 * @throws std::invalid_argument If tol <= 0 or is NaN, max_iter <= 0, ...
 * @throws std::runtime_error If no root is found within max_iter halvings.
 */
double bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter);
```

### File-private helper (static functions in .cpp files)

```cpp
/**
 * @brief One classic RK4 step: weighted average of four slopes (weights 1, 2, 2, 1).
 * @param f  Right-hand side f(t, y).
 * @param t  Current time.
 * @param y  Current value.
 * @param h  Step size.
 * @return y at time t + h.
 */
static double rk4_step(const ode::ODEFunction& f, double t, double y, double h)
```

### Struct members (app catalog)

```cpp
struct ODEProblem
{
    std::string name;    ///< Model as shown to the user.
    Function2D f;        ///< Right-hand side f(t, y).
    double t0;           ///< Start time.
    ...
};
```

### Template (test, example and app helpers)

```cpp
/**
 * @brief Passes only if running @p code throws an exception of type @p Expected.
 * @tparam Expected  Exception type that must be thrown, for example std::invalid_argument.
 * @tparam Code      Any callable taking no arguments (usually a lambda).
 * @param name  Description of what is being checked.
 * @param code  The code that should throw.
 */
template <typename Expected, typename Code>
void expect_throw(const std::string& name, Code code)
```

## 5. Generating the HTML documentation

### Installation

```bash
# Linux (Debian/Ubuntu):   sudo apt install doxygen
# macOS (Homebrew):        brew install doxygen
# Windows:                 installer from https://www.doxygen.nl/download.html, then reopen the terminal
```

### Generation

```bash
doxygen --version              # check it is installed (1.9 or newer recommended)
doxygen docs/Doxyfile          # run from the repository root
# open docs/api/html/index.html in a browser
```

### Doxyfile settings explained

| Doxyfile setting | Why |
| :--- | :--- |
| `INPUT = include src tests examples docs/…` | Documents the library, the internals, the app, the tests, the examples and the module guides. |
| `USE_MDFILE_AS_MAINPAGE = docs/README.md` | The documentation index becomes the front page. |
| `EXTRACT_ALL = NO` | Only documented items appear, so missing documentation shows up as a warning instead of an empty entry. |
| `EXTRACT_STATIC = YES` | Includes the file-private helpers (for example `rk4_step`, `find_pivot_row`, `solve_corrector`, `fehlberg_step`). |
| `WARN_IF_UNDOCUMENTED`, `WARN_NO_PARAMDOC`, `WARN_IF_INCOMPLETE_DOC` | Warn for any undocumented item, parameter or return value. The current code produces **0 warnings**. |
| `OUTPUT_DIRECTORY = docs/api` | Generated files go to `docs/api/`, which `.gitignore` excludes from Git. |

## 6. Documenting new code (checklist)

```cpp
/**
 * @file my_new_module.hpp
 * @brief One sentence: what this module does.
 * @author Your Name (Member N)
 */

/**
 * @brief One sentence: what the method computes.
 * @details The idea or formula, in two or three lines.
 * @param x  What it is, and its allowed range.
 * @return What comes back.
 * @throws std::invalid_argument When x is outside its range.
 */
double my_method(double x);
```

1. A `@file` block at the top, with `@brief` and `@author`.
2. Every class: `@brief`, and a short `@code` example for public classes.
3. Every function: `@brief`, one `@param` per parameter, `@return` if not void, one `@throws` per exception.
4. Templates: one `@tparam` per template parameter.
5. Run `doxygen docs/Doxyfile`: it must still print no warnings.
6. Comments explain the code (what and why), not its history ("fixed in week 3" belongs in the commit message).

---
