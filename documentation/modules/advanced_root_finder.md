# AdvancedRootFinder: Secant method and Fixed-Point Iteration

| | |
|---|---|
| Owner | Bakalu Valeria (Member 4) |
| Header | `include/numerical-computing/advanced_root_finder.hpp` |
| Source | `src/advanced_root_finder.cpp` |
| Tests | `tests/test_advanced_root_finder.cpp` (15 checks) |
| Examples | `examples/example_advanced_root_finder.cpp`, `examples/example_advanced_root_finder_applied.cpp` |

## 1. What it does

Both methods find a root **without the derivative**, which is useful when $f'(x)$ is hard or impossible to write down.

| Method | Solves | Needs | Convergence |
|---|---|---|---|
| `secant_method(f, x0, x1, tol, max_iter)` | $f(x) = 0$ | two starting guesses | superlinear, order $\approx 1.618$ (the golden ratio) |
| `fixed_point_iteration(g, x0, tol, max_iter)` | $x = g(x)$ | one starting guess and a "contracting" $g$ | linear, rate = $|g'(x^{\ast})|$ |

```cpp
numcomp::AdvancedRootFinder finder;
double r = finder.secant_method([](double x) { return x * x - 4.0; }, 1.0, 3.0, 1e-10, 100);        // 2
double p = finder.fixed_point_iteration([](double x) { return std::cos(x); }, 1.0, 1e-12, 1000);    // 0.7390851332
```

## 2. How the Secant method works

Newton's method uses the tangent line. The secant method replaces it with the line through the **last two points** $(x_0, f(x_0))$ and $(x_1, f(x_1))$. Where that line crosses zero is the next guess:

$$x_2 = x_1 - \frac{f(x_1)(x_1 - x_0)}{f(x_1) - f(x_0)}$$

1. Check `tol` and `max_iter` (shared helper `checks::check_tolerance`).
2. Repeat up to `max_iter` times:
   - Evaluate $f(x_0), f(x_1)$. If $|f(x_1) - f(x_0)| < 10^{-12}$ the line is horizontal (no crossing): throw `runtime_error`.
   - Compute $x_2$. If $|x_2 - x_1| < \text{tol}$, return $x_2$.
   - Shift: $x_0 = x_1$, $x_1 = x_2$.
3. Throw `runtime_error` if the loop ends.

It costs one new $f$ evaluation per step after the first. Newton costs one $f$ and one $f'$ evaluation, so the secant method is often faster in practice.

## 3. How Fixed-Point Iteration works

Rewrite $f(x) = 0$ as $x = g(x)$. For example, $x^2 - x - 2 = 0$ can become $x = \sqrt{x + 2}$. Then repeat $x_{\text{next}} = g(x)$.

1. Check `tol` and `max_iter`.
2. Repeat up to `max_iter` times: compute $x_{\text{next}} = g(x)$. If $|x_{\text{next}} - x| < \text{tol}$, return $x_{\text{next}}$.
3. Throw `runtime_error` if the loop ends.

**When it converges:** near the answer $x^{\ast}$, the error is multiplied by about $|g'(x^{\ast})|$ each step. So the iteration converges if $|g'(x^{\ast})| < 1$ and diverges if it is $> 1$. The way $f$ is rearranged matters:

| Rearrangement of $x^2 - x - 2 = 0$ | $g'(2)$ | Result |
|---|---|---|
| $g(x) = \sqrt{x + 2}$ | $1/4$ | converges |
| $g(x) = x^2 - 2$ | 4 | diverges (shown in example 1) |

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Shared `checks::check_tolerance` | The same rule and message as the basic root finder; also rejects NaN. |
| $10^{-12}$ threshold on $f(x_1) - f(x_0)$ | Dividing by a tiny difference sends the next guess far away; equal guesses ($x_0 = x_1$) would divide by zero. |
| Stop on $|x_{\text{next}} - x| < \text{tol}$ | Works for both methods and needs no extra function evaluations. |
| `runtime_error` for divergence | A diverging iteration (growing values, inf or NaN) never meets the stopping test, so it ends at `max_iter` and is reported as a failure, not returned as an answer. |
| Stateless class, `std::function` parameters | Same group-wide pattern as every other module. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | `tol` $\le 0$ or NaN, `max_iter` $\le 0$ |
| `std::runtime_error` | the secant line is flat ($|f(x_1) - f(x_0)| < 10^{-12}$), or no convergence within `max_iter` |

## 6. Limitations

- No bracketing: the secant method can jump away from the wanted root.
- Fixed-point iteration only works for a contracting rearrangement $x = g(x)$, and choosing $g$ is up to the user.
- The $10^{-12}$ threshold is absolute, not relative to the size of $f$.

## 7. Tests and examples

- **Tests (15 checks):** standard and transcendental equations; guesses in either order; starting on the answer; `max_iter` $= 2$ throwing `runtime_error`; `tol` $= 0$, `max_iter` $= 0$ and NaN tolerance throwing `invalid_argument`; equal secant guesses; a diverging $g$.
- **Example 1:** secant on three equations, three fixed-point problems, why the choice of $g$ matters, and rejected inputs.
- **Example 2:** a loan's interest rate from its monthly payment, Babylonian square roots, and the water depth in a spherical tank.