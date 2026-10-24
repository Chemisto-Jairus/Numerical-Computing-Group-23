# BasicRootFinder: Bisection and Newton-Raphson

| | |
|---|---|
| Owner | Chemisto Jairus (Member 1) |
| Header | `include/numerical-computing/basic_root_finder.hpp` |
| Source | `src/basic_root_finder.cpp` |
| Tests | `tests/test_basic_root_finder.cpp` (20 checks) |
| Examples | `examples/example_basic_root_finder.cpp`, `examples/example_basic_root_finder_applied.cpp` |

## 1. What it does

A *root* of a function f is a number x with f(x) = 0. `BasicRootFinder` finds one with two classic methods:

| Method | Needs | Speed | Reliability |
|---|---|---|---|
| `bisection(f, a, b, tol, max_iter)` | an interval [a, b] where f changes sign | slow (linear: one binary digit per step) | always converges if the sign change exists |
| `newton_raphson(f, df, x0, tol, max_iter)` | the derivative f'(x) and a starting guess | very fast (quadratic: digits roughly double per step) | can fail if the guess is poor or f'(x) is close to 0 |

```cpp
#include "numerical-computing/basic_root_finder.hpp"

numcomp::BasicRootFinder finder;
auto f  = [](double x) { return x * x - 2.0; };
auto df = [](double x) { return 2.0 * x; };
double r1 = finder.bisection(f, 0.0, 2.0, 1e-10, 100);      // 1.4142135623...
double r2 = finder.newton_raphson(f, df, 1.0, 1e-12, 50);   // 1.4142135623...
```

## 2. How Bisection works

The idea comes from the **intermediate value theorem**: if f is continuous and f(a), f(b) have opposite signs, f must cross zero somewhere between them.

1. Check the settings: `tol > 0`, `max_iter > 0` (shared helper `checks::check_tolerance`).
2. If a > b, swap them, so that (b − a)/2 is always the real half-width.
3. Evaluate f(a) and f(b) once. Reject NaN. If either is already within `tol` of zero, return that end point.
4. If f(a)·f(b) > 0 there is no guaranteed root: throw `invalid_argument`.
5. Repeat up to `max_iter` times:
   - c = (a + b)/2, evaluate f(c).
   - If |f(c)| < tol **or** (b − a)/2 < tol, return c.
   - Keep the half where the sign still changes: if f(c)·f(a) < 0 then b = c, otherwise a = c (and f(a) = f(c)).
6. If the loop ends, throw `runtime_error("Bisection method failed to converge.")`.

After k steps the interval is (b − a)/2^k wide, so reaching tolerance `tol` needs about log₂((b − a)/tol) steps. For [0, 2] and tol = 1e-12 that is about 41 steps.

## 3. How Newton-Raphson works

Near a point x, f looks like its tangent line: f(x + Δ) ≈ f(x) + f'(x)Δ. Setting this to zero gives the step Δ = −f(x)/f'(x):

  x_new = x − f(x) / f'(x)

1. Check `tol` and `max_iter`.
2. Repeat up to `max_iter` times:
   - Evaluate f'(x). If |f'(x)| < 1e-12 the tangent is flat and never meets the axis: throw `runtime_error`.
   - Compute x_new. If |x_new − x| < tol, return x_new.
3. If the loop ends, throw `runtime_error("Newton-Raphson failed to converge.")`.

For a simple root the error obeys e_(k+1) ≈ C·e_k², so the number of correct digits roughly doubles each step. In the applied example (tol = 1e-12 on x² − 2), Newton has converged by 10 iterations; bisection needs about 41 and is still running at 30.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Stateless class with only public member functions | The methods only need their arguments. No stored state means no stale data between calls, and one object can be reused. |
| `std::function<double(double)>` parameters | Any callable works: a normal function, a lambda, or a function object. |
| Swap a and b when a > b | Without it, (b − a)/2 is negative, so the stopping test is true at once and a wrong midpoint is returned. That was the Week 3 bug B1. |
| `!(tol > 0)` instead of `tol <= 0` (in `checks::check_tolerance`) | Every comparison with NaN is false. `tol <= 0` therefore accepts NaN, while `!(tol > 0)` rejects it. |
| Explicit NaN check on f(a), f(b) | NaN · anything is NaN, and `NaN > 0` is false, so the sign check would let NaN through. |
| Each f value computed once | f may be expensive; the original code evaluated f(a) several times per step. |
| Two stopping tests in bisection (|f(c)| and half-width) | A flat function may have tiny |f| far from the root; a steep one may need a narrow bracket. Either condition is a sensible stop. |
| Derivative threshold 1e-12 | Prevents division by (nearly) zero, which would send the next guess to infinity. |
| Exceptions instead of special return values | A caller cannot mistake an error for a root. The type tells them what went wrong (bad input vs. failed method). |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | `tol <= 0` or NaN, `max_iter <= 0`, f(a) or f(b) is NaN, or f(a) and f(b) have the same sign (bisection) |
| `std::runtime_error` | no convergence within `max_iter`, or |f'(x)| < 1e-12 (Newton) |

## 6. Limitations

- Bisection finds only one root, even when the bracket contains several, and cannot find roots where f touches zero without changing sign (for example x² at 0).
- Newton's result depends on the starting guess. It can jump to a different root or diverge.
- The derivative must be supplied as a function. It can be the exact derivative or a numerical one from `NumericalDifferentiator` (example 2 of that module shows Newton working with a central difference).
- The 1e-12 derivative threshold is absolute, so functions with very small values may be rejected.

## 7. Tests and examples

- **Tests (20 checks):** square, cubic and transcendental roots; a reversed interval; a root on an end point; a negative root; bad tolerance and iteration counts; a missing sign change; a NaN function; a zero derivative; non-convergence.
- **Example 1:** both methods on three equations, how the tolerance affects accuracy, and rejected inputs.
- **Example 2:** a projectile's landing time, Kepler's equation for an orbit, a break-even point, and a table of the iterations needed by each method.
