# Command-line app: numcomp_app

| | |
|---|---|
| Owners | Nabakooza Bridget Maria (Member 9), with Chemisto Jairus (Member 1) |
| Files | `src/main.cpp`, `src/app/console_ui.*`, `src/app/catalog.*`, `src/app/module_menus.*` |
| Build target | `numcomp_app` (linked against the `numcomp2` library) |

## 1. What it does

An interactive console program that demonstrates every method of the library. It opens with a large **GROUP 23** banner, then guides the user in three steps:

1. **Topic:** Root finding, Interpolation (and Chebyshev approximation), Differentiation, Integration, Initial value ODEs, Boundary value ODEs, Linear systems, or About. Every one of the 15 library classes is reachable.
2. **Method:** for example Bisection, Newton-Raphson, Secant, Fixed-point, or *Compare all*. The ODE topic first asks for a **family** (one-step, multistep, adaptive, or compare all nine methods), then for the method in that family.
3. **Problem:** a preset equation, function, integrand, model, boundary value problem, data set or matrix from the catalog, or the user's own (a custom cubic, custom data points, a custom linear ODE, a custom constant-coefficient BVP, a custom matrix up to 6×6).

Suggested parameters are shown in brackets, and **Enter accepts them**. Results appear next to the exact answer with the absolute error. Root finders also show the residual f(root) and the number of function evaluations.

## 2. Structure

| File | Role |
|---|---|
| `src/main.cpp` | Banner and main menu; dispatches to a topic menu; ends cleanly when the input ends. |
| `src/app/console_ui.*` | All input and output: line-based input with defaults and validation, numbered menus, framed headers, aligned results and tables, the banner. ASCII only, 78 columns wide. |
| `src/app/catalog.*` | Preset problems with their exact answers: 7 root equations, 5 fixed-point forms, 8 integrals (with antiderivatives, also used for Chebyshev approximation), 7 functions with exact first and second derivatives, 7 ODE models (with exact solutions), 5 boundary value problems (with exact solutions), 5 data sets, 8 linear systems (2 of them symmetric positive definite, for conjugate gradient). |
| `src/app/module_menus.*` | One menu per topic (seven topics and the About screen). Each calculation runs inside `run_safely`, which turns library exceptions into messages. |

## 3. How input is handled

- Input is read **one whole line at a time** (`std::getline`). A number must use the whole line: "2abc" is rejected, not read as 2.
- An empty line accepts the default shown in brackets.
- Menu choices must be whole numbers in range. Anything else gets a clear message and the question is asked again.
- A trailing `\r` (from Windows line endings) is removed.
- When input ends (Ctrl+D / Ctrl+Z, or the end of a piped file), a special `InputEnded` object is thrown. It is **not** derived from `std::exception`, so the calculation handlers do not catch it. `main()` catches it and exits with a goodbye message.

## 4. How errors are handled

Every library call runs inside `run_safely`, which catches from the most specific type to the most general:

| Caught | Message prefix |
|---|---|
| `std::invalid_argument` | `[!] Invalid input:` |
| `std::out_of_range` | `[!] Out of range:` |
| `std::runtime_error` | `[!] Calculation failed:` |
| `std::exception` (anything else, e.g. `bad_alloc`) | `[!] Unexpected error:` |

Results are computed **before** anything is printed, so an error never leaves a half-printed line. In the "compare" screens every method runs in its own handler. For example, conjugate gradient can reject a non-symmetric matrix while Gaussian elimination and LU still show their answers.

## 4a. What each topic offers

| Topic | Methods | Extra screens |
|---|---|---|
| Root finding | Bisection, Newton-Raphson, Secant, Fixed-point | compare three methods (with function-evaluation counts) |
| Interpolation | Linear, cubic spline, Chebyshev nodes, Chebyshev approximation | compare linear and spline; approximation error table |
| Differentiation | Forward, backward, central, second derivative, Richardson | error table for h = 0.1 … 1e-6 (shows rounding taking over) |
| Integration | Trapezoidal, Midpoint, Simpson, Romberg | compare all four |
| Initial value ODEs | One-step: Euler, Heun, RK3, RK4; multistep: AB2, AB4, AM2, AM4; adaptive: RKF45 | compare all nine at t_end; RKF45 table shows the step used |
| Boundary value ODEs | Finite difference method | convergence study n = 10 … 80 with error ratios |
| Linear systems | Gaussian elimination, LU, conjugate gradient | compare all three, with residuals |

## 5. Why it is written this way

| Decision | Reason |
|---|---|
| Separate `app/` files instead of one long main.cpp | Each file has one job (screen, data, menus), so each is short enough to read and test. |
| Catalog entries carry exact answers | The app can show the error for every method, which is the point of a numerical-methods demo. |
| "Compare all" options | Shows side by side why one method is preferred over another (accuracy, evaluations). |
| ODE methods grouped by family | Nine methods in one list is hard to read; the family question also teaches how the methods differ. |
| Tables with five or more columns use narrower columns; long error messages are wrapped | Every line stays within 78 characters. |
| ASCII-only output | Box-drawing characters show as garbage in the default Windows console. |
| Defaults for every parameter | A first-time user can press Enter through a whole demo. |

## 6. Building and running

```bash
cmake -S . -B build
cmake --build build
./build/numcomp_app          # Windows: build\numcomp_app.exe
```

With g++ only, see `docs/guides/build-guide.html`.
