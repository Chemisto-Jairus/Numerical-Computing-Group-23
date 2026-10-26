# AdvancedIntegrator: Simpson's 1/3 rule and Romberg integration

| | |
|---|---|
| Owner | Wasswa George Mpanga (Member 5) |
| Header | `include/numerical-computing/advanced_integrator.hpp` |
| Source | `src/advanced_integrator.cpp` |
| Tests | `tests/test_advanced_integrator.cpp` (17 checks) |
| Examples | `examples/example_advanced_integrator.cpp`, `examples/example_advanced_integrator_applied.cpp` |

## 1. What it does

| Method | Idea | Error |
|---|---|---|
| `simpsons_rule(f, a, b, n)` | fit a parabola through each pair of strips | ~ h⁴; exact for cubics |
| `romberg_integration(f, a, b, max_iter)` | trapezoidal results with 1, 2, 4, … strips, combined by Richardson extrapolation | removes one error term per column |

```cpp
numcomp::AdvancedIntegrator integrator;
double s = integrator.simpsons_rule([](double x) { return x * x * x; }, 0.0, 2.0, 2);          // exactly 4
double r = integrator.romberg_integration([](double x) { return std::sin(x); }, 0.0, M_PI, 8);  // 2
```

## 2. How Simpson's rule works

Through three equally spaced points there is exactly one parabola. Its integral over the two strips is h/3·(f_0 + 4f_1 + f_2). Adding this over n/2 pairs gives

  S = h/3 · [ f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + … + 4f(x_(n−1)) + f(x_n) ]

1. Check n > 0 and finite limits (`checks::check_integration_inputs`), then n even (the strips are used in pairs).
2. Sum with weights 1, 4, 2, 4, …, 4, 1 and multiply by h/3.

**Error:** −(b − a)·h⁴·f⁽⁴⁾(ξ)/180. Fourth order, so doubling n divides the error by 16 (shown in example 1). Because f⁽⁴⁾ = 0 for cubics, they are integrated exactly, even with n = 2.

## 3. How Romberg integration works

The trapezoidal error is a series in even powers of h: T(h) = I + c₁h² + c₂h⁴ + …. Combining two estimates cancels the leading term:

  (4·T(h/2) − T(h)) / 3 = I + O(h⁴)

Romberg repeats this in a triangular table R:
1. Check max_iter ≥ 1 and ≤ 25, and finite limits.
2. **Column 0:** R[i][0] = trapezoidal rule with 2^i strips, by calling `BasicIntegrator::trapezoidal_rule`.
3. **Column j ≥ 1:** R[i][j] = (4^j·R[i][j−1] − R[i−1][j−1]) / (4^j − 1). Each column removes the next error term (h², then h⁴, h⁶, …).
4. Return R[m−1][m−1].

Column 1 equals Simpson's rule, and column j is exact for polynomials up to degree 2j + 1. In the tests, 3 rows integrate x⁴ exactly.

**Cost:** the last row uses 2^(m−1) strips. With the current implementation, which recomputes each trapezoid from scratch, about 2^m evaluations are needed in total. Example 2 counts them: for eˣ on [0, 1] to 1e-10, Romberg needs 5 rows and 36 evaluations, while Simpson needs n = 128 and 129 evaluations.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Simpson requires even n (`invalid_argument` otherwise) | The parabolas span pairs of strips; silently changing n would surprise the user. |
| Romberg capped at 25 rows | `1 << i` overflows `int` at i = 31, and the work doubles every row: 25 rows already means about 16.7 million strips in the last row. |
| Romberg calls `BasicIntegrator::trapezoidal_rule` | Column 0 *is* the trapezoidal rule. Reusing the class (composition) means the code exists once and gives identical numbers. |
| Shared checks | Same rules and messages as the basic rules. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | Simpson: n ≤ 0 or odd; Romberg: max_iter ≤ 0 or > 25; either: a or b NaN or infinite |

## 6. Limitations

- Romberg assumes a smooth integrand. Kinks or singularities spoil the extrapolation.
- Romberg does not reuse earlier function values (each trapezoid is computed from scratch). It is correct but costs about twice the minimum.
- No adaptive stopping test: the user chooses n or the number of rows.

## 7. Tests and examples

- **Tests (17 checks):** x³ and sin x; Simpson exact for a cubic with n = 2; Romberg exact for x³ (2 rows) and x⁴ (3 rows); reversed limits; 1 row equal to one trapezoid; 20 rows running without error; odd n, n = 0, 26 rows, 0 rows, NaN and infinite limits throwing.
- **Example 1:** Simpson on three integrals, its 16× convergence, and a Romberg accuracy-vs-rows table.
- **Example 2:** erf(1), the arc length of y = x², and evaluations needed for 1e-10 (Simpson vs Romberg).
