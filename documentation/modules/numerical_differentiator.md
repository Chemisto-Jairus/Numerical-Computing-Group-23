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
| `forward_difference(f, x, h)` | $\frac{f(x + h) - f(x)}{h}$ | $\sim h$ |
| `backward_difference(f, x, h)` | $\frac{f(x) - f(x - h)}{h}$ | $\sim h$ |
| `central_difference(f, x, h)` | $\frac{f(x + h) - f(x - h)}{2h}$ | $\sim h^2$ |
| `second_derivative(f, x, h)` | $\frac{f(x + h) - 2f(x) + f(x - h)}{h^2}$ | $\sim h^2$ |
| `richardson_extrapolation(f, x, h)` | $\frac{4D(h/2) - D(h)}{3}$, with $D =$ central difference | $\sim h^4$ |

```cpp
numcomp::NumericalDifferentiator d;
double v = d.central_difference([](double x) { return std::sin(x); }, 1.0, 1e-5);   // cos(1) to about 1e-11
```

## 2. How it works

The derivative is a limit: $f'(x) = \lim_{h \to 0} \frac{f(x + h) - f(x)}{h}$. A computer cannot take a limit, so every formula uses a small fixed $h$. Taylor's theorem tells us how large the error is:

  $f(x \pm h) = f(x) \pm h f'(x) + \frac{h^2}{2} f''(x) \pm \frac{h^3}{6} f'''(x) + \dots$

1. **Forward / backward.** Rearranging one series gives $f'(x)$ plus an error $\frac{h}{2} f''(\xi)$. The error halves when $h$ halves (first order).
2. **Central.** Subtracting the two series cancels the $h^2$ terms. What is left is $f'(x) + \frac{h^2}{6} f'''(\xi)$. This is second order for the same two evaluations of $f$.
3. **Second derivative.** Adding the two series cancels the odd terms. Dividing by $h^2$ gives $f''(x) + \frac{h^2}{12} f^{(4)}(\xi)$.
4. **Richardson extrapolation.** The central difference is $D(h) = f' + c h^2 + O(h^4)$. Then $D(h/2)$ has error $\frac{c h^2}{4}$, so $\frac{4D(h/2) - D(h)}{3}$ cancels the $h^2$ term and leaves $O(h^4)$. This is the same trick Romberg integration applies to the trapezoidal rule.

Each method first calls `checks::check_difference_inputs(x, h)`. At the end it calls `checks::check_finite_result(result)`, so a NaN or infinite value is never returned silently.

## 3. Choosing h: truncation against rounding

A smaller $h$ reduces the truncation error, but $f(x + h)$ and $f(x - h)$ then share more and more leading digits. Subtracting them loses those digits (cancellation). Example 1 measures the central difference of $e^x$ at 0:

| h | $10^{-2}$ | $10^{-4}$ | $10^{-6}$ | $10^{-8}$ | $10^{-10}$ | $10^{-12}$ |
|---|---|---|---|---|---|---|
| error | 1.7e-05 | 1.7e-09 | 2.7e-11 | 6.1e-09 | 8.3e-08 | 3.3e-05 |

The best $h$ for a first-derivative formula in double precision is around $10^{-5}$ to $10^{-6}$. For the second derivative it is around $10^{-3}$ to $10^{-4}$, because it divides by $h^2$.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Five separate methods, not one method with a "kind" flag | Each formula is one line and has its own error order; separate names make the order clear at the call site. |
| `richardson_extrapolation` calls `central_difference` twice | No formula is repeated, and both calls are validated and checked. |
| $h$ is chosen by the caller | The best $h$ depends on $f$ and on $x$. A fixed default would be wrong for some functions; the demos show how to choose. |
| Finite-result check | $f$ may be undefined next to $x$ (for example $\log x$ near 0). Throwing `runtime_error` is clearer than returning NaN. |
| Shared `check_difference_inputs` | All five formulas accept exactly the same $x$ and $h$, with the same messages. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | $x$ is NaN or infinite; $h \le 0$, NaN or infinite |
| `std::runtime_error` | $f$ returns NaN or infinity at $x$ or $x \pm h$ (overflow, singularity, outside its domain) |

## 6. Limitations

- Only functions given as formulas: the class does not differentiate tabulated data.
- Equally spaced points only, and no automatic choice of $h$.
- The one-sided formulas are first order. Near a boundary where $f$ is not defined on the other side, use them with a small $h$.

## 7. Tests and examples

- **Tests (26 checks):** $\sin$, $e^x$, polynomials (central exact for quadratics, second derivative exact for cubics); observed orders 2, 4, 4 and 16 when $h$ is halved; the corner of $|x|$ (forward $+1$, backward $-1$, central 0); invalid $h$ and $x$; NaN and overflow results.
- **Example 1:** all five formulas at $x = 1$, the order table, the rounding-error table and the $|x|$ corner.
- **Example 2:** velocity and acceleration of a car, marginal cost, and Newton-Raphson using a numerical derivative (it reaches the same root as with the exact derivative).