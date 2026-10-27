# Test framework and test programs

| | |
|---|---|
| Owner | Kyeyune Praise Pauline (Member 8); each module owner wrote the tests for their module |
| Files | `tests/test_helpers.hpp`, `tests/test_*.cpp`, `tests/CMakeLists.txt`, `tests/run_all_tests.sh` |

## 1. How a test program works

Each module has one test program with its own `main()`. It includes `test_helpers.hpp`, a small header-only framework in namespace `test`:

| Helper | Job |
|---|---|
| `test::check(name, ok)` | records one result and prints `[PASS] name` or `[FAIL] name` |
| `test::close(a, b, tol)` | true if \|a − b\| < tol |
| `test::expect_throw<E>(name, code)` | passes only if `code` throws exception type E |
| `test::expect_no_throw(name, code)` | passes only if `code` throws nothing |
| `test::finish(module)` | prints the summary and returns 0 (all passed) or 1 (something failed) |

```cpp
test::check("Bisection: x^2 - 4 on [0, 3] gives 2", test::close(finder.bisection(f, 0, 3, 1e-8, 100), 2.0, 1e-6));
test::expect_throw<std::invalid_argument>("Bisection: tol = 0 throws", [&] { finder.bisection(f, 0, 3, 0.0, 100); });
return test::finish("Basic Root Finder");
```

## 2. Why not assert() or an external framework

| Choice | Reason |
|---|---|
| No `assert()` | In Release builds `NDEBUG` disables assert, so the tests would silently check nothing. The first failing assert also stops all later checks. |
| No GoogleTest or Catch2 | The README promises "no outside libraries", and graders must be able to build from a clean machine without downloads. |
| Exit code 0/1 | ctest and the runner script judge a program by its exit code. |
| `expect_throw<E>` template | One helper replaces about 50 hand-written try/catch blocks and checks the **exact** exception type. |

## 3. What is tested

Every test file covers the three groups required by the project instructions (Section 4): **normal use, edge cases, and invalid input**.

| Program | Checks |
|---|---|
| test_interpolator | 37 |
| test_cubic_spline | 18 |
| test_matrix_solver | 21 |
| test_basic_root_finder | 20 |
| test_basic_integrator | 20 |
| test_advanced_root_finder | 15 |
| test_advanced_integrator | 17 |
| test_basic_ode_solver | 27 |
| test_advanced_ode_solver | 25 |
| test_numerical_differentiator | 26 |
| test_conjugate_gradient_solver | 20 |
| test_adams_bashforth_solver | 26 |
| test_adams_moulton_solver | 25 |
| test_runge_kutta_fehlberg_solver | 18 |
| test_finite_difference_solver | 20 |
| **Total** | **335** |

## 4. Running

- `ctest` in the build folder runs the 15 test programs and the 30 examples (45 entries).
- `bash tests/run_all_tests.sh` from the repository root does the same, with a per-program PASSED / FAILED / SKIPPED summary. It also works for a g++ build: `bash tests/run_all_tests.sh build-gpp`.
- Full instructions for g++ and CMake are in README file. (Pauline_guide was given by AI to help me understand how to run the file)
