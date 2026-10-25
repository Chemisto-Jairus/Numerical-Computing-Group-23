# BasicIntegrator: Trapezoidal rule and Midpoint rule

| | |
|---|---|
| Owner | Aogon Sharon (Member 3) |
| Header | `include/numerical-computing/basic_integrator.hpp` |
| Source | `src/basic_integrator.cpp` |
| Tests | `tests/test_basic_integrator.cpp` (20 checks) |
| Examples | `examples/example_basic_integrator.cpp`, `examples/example_basic_integrator_applied.cpp` |

## 1. What it does

Approximates the definite integral ∫ₐᵇ f(x) dx by splitting [a, b] into n equal strips of width h = (b − a)/n and adding up simple shapes.

| Method | Shape per strip | Formula |
|---|---|---|
| `trapezoidal_rule(f, a, b, n)` | trapezoid under the chord | h·[ f(a)/2 + f(x_1) + … + f(x_(n−1)) + f(b)/2 ] |
| `midpoint_rule(f, a, b, n)` | rectangle at the strip's centre | h·[ f(m_1) + … + f(m_n) ],  m_i = a + (i − ½)h |

```cpp
numcomp::BasicIntegrator integrator;
double t = integrator.trapezoidal_rule([](double x) { return x * x; }, 0.0, 1.0, 100);   // 0.33335
double m = integrator.midpoint_rule([](double x) { return x * x; }, 0.0, 1.0, 100);      // 0.333325
```

## 2. How it works

**Trapezoidal rule:**
1. Check: n > 0, a and b finite (shared helper `checks::check_integration_inputs`).
2. h = (b − a)/n.
3. sum = ½(f(a) + f(b)); then add f(a + i·h) for i = 1..n−1.
4. Return sum·h.

**Midpoint rule:** same checks; add f(a + (i + ½)h) for i = 0..n−1; return sum·h. It never evaluates f at a or b, which helps when f is awkward at an end point.

## 3. Accuracy

| Rule | Error | Exact for |
|---|---|---|
| Trapezoidal | −(b − a)·h²·f''(ξ)/12 | straight lines |
| Midpoint | +(b − a)·h²·f''(ξ)/24 | straight lines |

- Both are **second order**: doubling n divides the error by about 4. Example 1 shows this with sin x on [0, π].
- The midpoint error is about **half** the trapezoidal error and of **opposite sign**. That is why Simpson's rule (a weighted average of the two) is so much better.
- For x² on [0, 1] the errors are exactly 1/(6n²) and −1/(12n²). The tests check these to 1e-12.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Equal strips (composite rules) | Simple, predictable, and the error analysis is standard. |
| b < a allowed | Gives the negative integral, matching ∫ₐᵇ = −∫ᵦᵃ. h is simply negative. |
| a = b gives 0 | Correct, and needs no special case. |
| Finite limits required | NaN or infinite limits used to return `nan` silently (Week 3 bug B10). |
| Shared `checks::check_integration_inputs` | The same rule for the trapezoidal, midpoint and Simpson rules. |
| `trapezoidal_rule` is reused by Romberg | `AdvancedIntegrator::romberg_integration` builds its first column by calling this method (composition), so the trapezoid code exists once. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | n ≤ 0, or a or b is NaN or infinite |

## 6. Limitations

- Fixed n: no automatic choice of n for a requested accuracy (Romberg comes close to that).
- Improper integrals (infinite limits, or a singularity inside [a, b]) are not supported.
- Cost is n + 1 (trapezoid) or n (midpoint) function evaluations.

## 7. Tests and examples

- **Tests (20 checks):** x² and sin x; exact results for straight lines (n = 1 and 7); the known errors for x²; reversed limits; a = b; negative intervals; n = 0, n < 0 and NaN/infinite limits throwing `invalid_argument`.
- **Example 1:** x², sin x and eˣ with both rules, the error-vs-n table, reversed limits, and rejected inputs.
- **Example 2:** distance from a velocity function, the normal-distribution probability P(−1 < Z < 1), and the work done by a spring.
