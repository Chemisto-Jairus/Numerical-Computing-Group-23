# BasicODESolver: Euler and Heun

| | |
|---|---|
| Owner | Mwanje Ian (Member 6) |
| Header | `include/numerical-computing/basic_ode_solver.hpp` |
| Source | `src/basic_ode_solver.cpp` (step rules) + `src/internal/ode_stepper.cpp` (shared loop) |
| Tests | `tests/test_basic_ode_solver.cpp` (27 checks) |
| Examples | `examples/example_basic_ode_solver.cpp`, `examples/example_basic_ode_solver_applied.cpp` |

## 1. What it does

Solves a first-order **initial value problem**

  dy/dt = f(t, y),  y(t0) = y0,  for t from t0 to t_end,

by stepping forward with a fixed step size h. The result is the whole solution curve as a `std::vector<std::pair<double, double>>` of (t, y) points, starting with (t0, y0) and ending exactly at t_end.

| Method | One step | Global error |
|---|---|---|
| `euler(f, t0, y0, t_end, h)` | y + h·f(t, y) | ~ h (first order) |
| `heun(f, t0, y0, t_end, h)` | y + h/2·(k1 + k2), k1 = f(t, y), k2 = f(t + h, y + h·k1) | ~ h² (second order) |

```cpp
numcomp::BasicODESolver solver;
auto curve = solver.heun([](double t, double y) { return y; }, 0.0, 1.0, 1.0, 0.01);
double y_end = curve.back().second;   // 2.71824..., close to e
```

## 2. How it works

**Euler:** follow the slope at the start of the step for the whole step. It is simple but drifts away from curved solutions.

**Heun (improved Euler, explicit trapezoid):** *predict* the end of the step with Euler, measure the slope there, and *correct* by averaging the two slopes. Errors in the two slopes partly cancel.

**The shared loop (`ode::run_fixed_steps`):** all four ODE methods (Euler, Heun, RK3, RK4) use the same loop. Only the one-step formula differs:
1. Check h > 0, t_end > t0 and at most 1,000,000 steps (`checks::check_ode_inputs`).
2. Store (t0, y0).
3. While t is not yet at t_end:
   - if t + h would pass t_end, shorten h to t_end − t;
   - y = step(f, t, y, h); t = t + h;
   - if t is within 1e-9·h of t_end, snap t to exactly t_end;
   - store (t, y).

Each method passes its formula as a named `static` function (`euler_step`, `heun_step`) through the function pointer type `ode::StepRule`.

## 3. Accuracy (measured)

For dy/dt = y on [0, 1], halving h halves Euler's error (ratio 2) and quarters Heun's (ratio 4). Both are checked in the tests (ratios within 1.8–2.2 and 3.6–4.4). Euler on dy/dt = 2t with h = 0.1 gives exactly 0.9 at t = 1 (the known first-order error h²·n(n−1)). Heun is exact there, because it integrates a straight-line slope exactly.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Return the whole curve | Users usually want to see or plot the solution, not only the end value. `curve.back()` gives the end value. |
| Shorten the last step | t_end does not have to be a multiple of h; the curve always ends exactly at t_end. |
| Snap to t_end within 1e-9·h | Repeated additions of h drift by rounding (0.1 is not exact in binary); without the snap a tiny extra step could appear. |
| Cap of 1,000,000 steps | Each step stores a point; a tiny h could otherwise exhaust memory. The same limit (`checks::MAX_POINTS`) caps Chebyshev nodes. |
| `!(h > 0)`, `!(t_end > t0)` | Also reject NaN, which used to return one point with no error (Week 3 bug B9). |
| One shared loop, per-method step rules | The loop used to be copied four times (about 30 lines each). Now each method is one line plus its formula, and a fix to the loop fixes all four methods. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | h ≤ 0, t_end ≤ t0 (either NaN), or (t_end − t0)/h > 1,000,000 |

## 6. Limitations

- Fixed step: no error control or adaptive step size (see `RungeKuttaFehlbergSolver`).
- One-step methods re-evaluate f several times per step; the multistep classes (`AdamsBashforthSolver`, `AdamsMoultonSolver`) reuse old slopes instead.
- Explicit methods: stiff equations (very fast decay) need very small h to stay stable.
- Single equations only. Systems y' = F(t, y) with vector y are not supported.
- Only forward in time (t_end > t0).

## 7. Tests and examples

- **Tests (27 checks):** polynomial, trig and exponential problems; error ratios 2 and 4; the first point; 11 points for h = 0.1; ending exactly at t_end; an uneven last step; h larger than the interval; a negative start time; six invalid inputs for each method.
- **Example 1:** the full curve for dy/dt = y, the order-of-accuracy table, three other right-hand sides, and rejected inputs.
- **Example 2:** Newton's law of cooling (a cup of tea), and logistic population growth.
