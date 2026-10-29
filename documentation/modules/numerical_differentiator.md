# NumericalDifferentiator: finite difference formulas for derivatives

| | |
|---|---|
| Owner | Chemisto Jairus (Member 1) |
| Header | `include/numerical-computing/numerical_differentiator.hpp` |
| Source | `src/numerical_differentiator.cpp` |
| Tests | `tests/test_numerical_differentiator.cpp` (26 checks) |
| Examples | `examples/example_numerical_differentiator.cpp`, `examples/example_numerical_differentiator_applied.cpp` |

## 1. What it does

| Method | Formula | Error |
|---|---|---|
| `forward_difference(f, x, h)` | (f(x + h) − f(x)) / h | ~ h |
| `backward_difference(f, x, h)` | (f(x) − f(x − h)) / h | ~ h |
| `central_difference(f, x, h)` | (f(x + h) − f(x − h)) / (2h) | ~ h² |
| `second_derivative(f, x, h)` | (f(x + h) − 2f(x) + f(x − h)) / h² | ~ h² |
| `richardson_extrapolation(f, x, h)` | (4·D(h/2) − D(h)) / 3, with D = central difference | ~ h⁴ |

```cpp
numcomp::NumericalDifferentiator d;
double v = d.central_difference([](double x) { return std::sin(x); }, 1.0, 1e-5);   // cos(1) to about 1e-11
```

## 2. How it works

The derivative is a limit: f'(x) = lim (f(x + h) − f(x)) / h as h → 0. A computer cannot take a limit, so every formula uses a small fixed h. Taylor's theorem tells us how large the error is:

  f(x ± h) = f(x) ± h f'(x) + h²/2 f''(x) ± h³/6 f'''(x) + …

1. **Forward / backward.** Rearranging one series gives f'(x) plus an error (h/2)·f''(ξ). The error halves when h halves (first order).
2. **Central.** Subtracting the two series cancels the h² terms. What is left is f'(x) + (h²/6)·f'''(ξ). This is second order for the same two evaluations of f.
3. **Second derivative.** Adding the two series cancels the odd terms. Dividing by h² gives f''(x) + (h²/12)·f⁽⁴⁾(ξ).
4. **Richardson extrapolation.** The central difference is D(h) = f' + c·h² + O(h⁴). Then D(h/2) has error c·h²/4, so (4·D(h/2) − D(h))/3 cancels the h² term and leaves O(h⁴). This is the same trick Romberg integration applies to the trapezoidal rule.

Each method first calls `checks::check_difference_inputs(x, h)`. At the end it calls `checks::check_finite_result(result)`, so a NaN or infinite value is never returned silently.

## 3. Choosing h: truncation against rounding

A smaller h reduces the truncation error, but f(x + h) and f(x − h) then share more and more leading digits. Subtracting them loses those digits (cancellation). Example 1 measures the central difference of eˣ at 0:

| h | 1e-2 | 1e-4 | 1e-6 | 1e-8 | 1e-10 | 1e-12 |
|---|---|---|---|---|---|---|
| error | 1.7e-05 | 1.7e-09 | 2.7e-11 | 6.1e-09 | 8.3e-08 | 3.3e-05 |

The best h for a first-derivative formula in double precision is around 1e-5 to 1e-6. For the second derivative it is around 1e-3 to 1e-4, because it divides by h².

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Five separate methods, not one method with a "kind" flag | Each formula is one line and has its own error order; separate names make the order clear at the call site. |
| `richardson_extrapolation` calls `central_difference` twice | No formula is repeated, and both calls are validated and checked. |
| h is chosen by the caller | The best h depends on f and on x. A fixed default would be wrong for some functions; the demos show how to choose. |
| Finite-result check | f may be undefined next to x (for example log x near 0). Throwing `runtime_error` is clearer than returning NaN. |
| Shared `check_difference_inputs` | All five formulas accept exactly the same x and h, with the same messages. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | x is NaN or infinite; h ≤ 0, NaN or infinite |
| `std::runtime_error` | f returns NaN or infinity at x or x ± h (overflow, singularity, outside its domain) |

## 6. Limitations

- Only functions given as formulas: the class does not differentiate tabulated data.
- Equally spaced points only, and no automatic choice of h.
- The one-sided formulas are first order. Near a boundary where f is not defined on the other side, use them with a small h.

## 7. Tests and examples

- **Tests (26 checks):** sin, eˣ, polynomials (central exact for quadratics, second derivative exact for cubics); observed orders 2, 4, 4 and 16 when h is halved; the corner of |x| (forward +1, backward −1, central 0); invalid h and x; NaN and overflow results.
- **Example 1:** all five formulas at x = 1, the order table, the rounding-error table and the |x| corner.
- **Example 2:** velocity and acceleration of a car, marginal cost, and Newton-Raphson using a numerical derivative (it reaches the same root as with the exact derivative).
