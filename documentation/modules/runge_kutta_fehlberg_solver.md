# RungeKuttaFehlbergSolver: adaptive RKF45

| | |
|---|---|
| Owner | Wasswa George Mpanga (Member 5) |
| Header | `include/numerical-computing/runge_kutta_fehlberg_solver.hpp` |
| Source | `src/runge_kutta_fehlberg_solver.cpp` |
| Tests | `tests/test_runge_kutta_fehlberg_solver.cpp` (18 checks) |
| Examples | `examples/example_runge_kutta_fehlberg_solver.cpp`, `examples/example_runge_kutta_fehlberg_solver_applied.cpp` |

## 1. What it does

| Method | Purpose |
|---|---|
| `rkf45(f, t0, y0, t_end, h_initial, tol)` | Solve dy/dt = f(t, y) and return every accepted point |
| `rkf45_value(f, t0, y0, t_end, tol)` | Return only y(t_end), with h_initial = (t_end − t0)/100 |

The user gives a **tolerance** instead of a step size. The returned points are not equally spaced: they are dense where the solution changes fast.

## 2. How it works

Each trial step from (t, y) with step h evaluates six slopes k1 … k6 (Fehlberg's coefficients). It combines them in two ways:

- y4 = y + 25/216·k1 + 1408/2565·k3 + 2197/4104·k4 − 1/5·k5 (fourth order);
- y5 = y + 16/135·k1 + 6656/12825·k3 + 28561/56430·k4 − 9/50·k5 + 2/55·k6 (fifth order).

Both use the same six slopes, so the error estimate is almost free. Then:

1. R = |y5 − y4| / h is the estimated error per unit time.
2. If R ≤ tol, the step is **accepted**: t ← t + h, y ← y4, and the point is stored.
3. Either way, h is rescaled by q = 0.84·(tol/R)^(1/4), limited to [0.1, 4] and to at most t_end − t0. The exponent 1/4 comes from the local error of the fourth-order formula; 0.84 is a safety factor.
4. The last step is shortened to land exactly on t_end.

This "estimate the error, then refine" idea is the same one that makes Romberg integration work. That is why George, who owns Romberg, implements it.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| Classic Fehlberg form (advance with y4, R per unit time) | It matches the textbook (Burden & Faires), so every line can be checked against the course notes. |
| q limited to [0.1, 4] | Prevents h from collapsing or exploding after one unusual step. |
| Safeguards: h ≥ 1e-12·max(1, |t|), at most 1,000,000 trial steps | A singular problem (y' = y², blow-up at t = 1) ends with a clear `runtime_error` instead of looping for ever. |
| Shared checks (`check_ode_inputs`, `check_tolerance`, `check_finite_result`) | Same rules and messages as the other ODE solvers. |
| Returns every accepted point | The spacing shows where the method worked hard, which is useful in reports and in the app. |
| `FehlbergStep` struct + static `fehlberg_step` | One trial step is written once and returns both numbers it produces. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | h_initial ≤ 0, t_end ≤ t0, tol ≤ 0 (any may be NaN), or (t_end − t0)/h_initial > 1,000,000 |
| `std::runtime_error` | Step size below 1e-12·|t|, more than 1,000,000 trial steps, or a non-finite solution |

## 5. Limitations

- The tolerance controls the *local* error per unit time. The global error is usually of the same size, but this is not guaranteed.
- The right-hand side should be smooth. A jump in f (for example a switch at t = 10) makes R stay large near the jump, and the step size collapses. Example 2 models the parachute opening smoothly for this reason.
- Explicit method: stiff problems force very small steps.

## 6. Tests and examples

- **Tests (18 checks):** error below tol for 1e-4 and 1e-8; more steps for a tighter tol; `rkf45_value`; a t⁴ solution integrated exactly in at most two steps; step sizes grow on a fast-then-slow problem; first point, exact t_end, increasing times; invalid inputs; blow-up.
- **Example 1:** tolerance against error and number of steps (1e-3 → 4 steps, 1e-9 → 76 steps), the step size growing from 0.001 to 0.008, `rkf45_value`, rejected inputs.
- **Example 2:** a skydiver's parachute opening (the smallest step is taken while it opens) and Michaelis-Menten drug elimination.
