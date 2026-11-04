# AdvancedODESolver: Runge-Kutta 3 and Runge-Kutta 4

| | |
|---|---|
| Owner | Muwanguzi Samuel (Member 7) |
| Header | `include/numerical-computing/advanced_ode_solver.hpp` |
| Source | `src/advanced_ode_solver.cpp` (step rules) + `src/internal/ode_stepper.cpp` (shared loop) |
| Tests | `tests/test_advanced_ode_solver.cpp` (25 checks) |
| Examples | `examples/example_advanced_ode_solver.cpp`, `examples/example_advanced_ode_solver_applied.cpp` |

## 1. What it does

Solves $\frac{dy}{dt} = f(t, y)$, $y(t_0) = y_0$, like `BasicODESolver`, but with higher-order **Runge-Kutta** methods. These sample the slope several times inside each step and combine the samples.

| Method | Slopes per step | Combination | Global error |
|---|---|---|---|
| `rk3(f, t0, y0, t_end, h)` | 3 | $\frac{h}{6}(k_1 + 4k_2 + k_3)$ | $\sim h^3$ |
| `rk4(f, t0, y0, t_end, h)` | 4 | $\frac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4)$ | $\sim h^4$ |

```cpp
numcomp::AdvancedODESolver solver;
auto curve = solver.rk4([](double t, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.1);
double y_end = curve.back().second;   // 0.1353355..., e^-2 = 0.1353353
```

## 2. How it works

**RK4 (the classic method):**
- $k_1 = f(t, y)$: slope at the start
- $k_2 = f(t + \frac{h}{2}, y + \frac{h \cdot k_1}{2})$: slope at the midpoint, reached with $k_1$
- $k_3 = f(t + \frac{h}{2}, y + \frac{h \cdot k_2}{2})$: slope at the midpoint again, reached with $k_2$
- $k_4 = f(t + h, y + h \cdot k_3)$: slope at the end
- $y_{\text{new}} = y + \frac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4)$

**RK3 (Kutta's third-order method):**
- $k_1 = f(t, y)$, $k_2 = f(t + \frac{h}{2}, y + \frac{h \cdot k_1}{2})$, $k_3 = f(t + h, y - h \cdot k_1 + 2h \cdot k_2)$
- $y_{\text{new}} = y + \frac{h}{6}(k_1 + 4k_2 + k_3)$

When $f$ depends on $t$ only, both weightings reduce to **Simpson's rule** (weights 1, 4, 1). So they integrate cubic slopes exactly: $\frac{dy}{dt} = 4t^3$ gives exactly $y(2) = 16$ (tested).

The time loop (input checks, storing points, shortening the last step, landing on $t_{\text{end}}$) is the shared `ode::run_fixed_steps`, also used by Euler and Heun. Each method only supplies its step formula as a `static` function (`rk3_step`, `rk4_step`).

`rk4` is also used by the multistep loop (`ode::run_multistep`): Adams-Bashforth and Adams-Moulton call it to create their starting values, so RK4 exists only once in the library.

## 3. Accuracy (measured)

For $\frac{dy}{dt} = -2y$ on $[0, 1]$, halving $h$ divides the RK3 error by about 8 and the RK4 error by about 16. The tests require ratios of 7–9 and 14–18. With $h = 0.1$ the RK4 error at $t = 1$ is about $4 \times 10^{-6}$, against about $10^{-4}$ for RK3 at the same step.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Same interface as `BasicODESolver` | Methods are interchangeable: a user can switch from Euler to RK4 by changing one name. |
| Shared loop with the basic solvers | Identical behaviour (checks, last step, landing on $t_{\text{end}}$) for all four methods, and the code exists once. |
| Kutta's RK3 | The standard third-order scheme, with a $k_3$ that reaches the end of the step. |
| Fixed step size | Simple and predictable for teaching. The step-size table shows the order of each method. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | $h \le 0$, $t_{\text{end}} \le t_0$ (either NaN), or $\frac{t_{\text{end}} - t_0}{h} > 1,000,000$ |

## 6. Limitations

- Fixed step: no embedded error estimate and no adaptive $h$. For that, use `RungeKuttaFehlbergSolver`.
- Explicit methods: stiff problems need small $h$.
- Single equations only (no systems or higher-order ODEs without rewriting them by hand).

## 7. Tests and examples

- **Tests (25 checks):** exactness for $4t^3$; trig and decay problems; error ratios about 8 and 16; the first point; 11 points; ending exactly at $t_{\text{end}}$; an uneven last step; $h$ larger than the interval; six invalid inputs for each method.
- **Example 1:** the decay curve with both methods, the order table, three other right-hand sides, and rejected inputs.
- **Example 2:** charging a capacitor through a resistor (RC circuit), and drug concentration during an infusion.