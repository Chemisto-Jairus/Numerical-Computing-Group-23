# Templates used in our library

## 1. Templates we have

| Template | File | Parameter | What it does |
| :--- | :--- | :--- | :--- |
| `run_safely<Code>` | `src/app/module_menus.cpp` | Code = any callable | Runs any lambda inside one try/catch, so 18 menu calls share one error handler |
| `names<Entry>` | `src/app/catalog.hpp` | Entry = any struct with a name | Builds menu labels for all eight problem tables |
| `demo::section<Code>` | `examples/example_helpers.hpp` | Code = any callable | Runs the body of one demo section |
| `demo::show_rejected<Code>` | `examples/example_helpers.hpp` | Code = any callable | Runs a call that should throw and prints the error |
| `test::expect_throw<Expected, Code>` | `tests/test_helpers.hpp` | Expected = exception type, Code = callable | One function checks every exception type |
| `test::expect_no_throw<Code>` | `tests/test_helpers.hpp` | Code = any callable | Checks that a call does not throw |
| `max_linear_error<F>` | `examples/example_interpolator_applied.cpp` | F = any callable | A local helper that measures an interpolation error for any function |

## 2. Why we use templates here

* **Code Reusability:** Instead of rewriting the same `try/catch` error-handling blocks or testing assertions for every single menu item or test case, a single template (like `run_safely` or `test::expect_throw`) handles all of them.
* **Flexibility with Callables:** By using template parameters like `Code` or `F`, the helper functions can accept any type of callable. This means we can pass in raw function pointers, lambdas (even with captured state), or functors without restriction.
* **Compile-Time Type Safety:** Templates like `test::expect_throw<Expected, Code>` allow us to specify the exact exception type (e.g., `std::invalid_argument`) at compile time. The compiler ensures we are checking for the precise error type we expect.
* **Performance without Overhead:** Passing a mathematical function or test execution as a template parameter allows the compiler to fully inline the code. This is much faster and requires less memory overhead than wrapping the code in a standard `std::function`.