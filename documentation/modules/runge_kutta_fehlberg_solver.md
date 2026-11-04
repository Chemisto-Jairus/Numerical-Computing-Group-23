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
| `rkf45(f, t0, y0, t_end, h_initial, tol)` | Solve $\frac{dy}{dt} = f(t, y)$ and return every accepted point |
| `rkf45_value(f, t0, y0, t_end, tol)` | Return only $y(t_{\text{end}})$, with $h_{\text{initial}} = \frac{t_{\text{end}} - t_0}{100}$ |

The user gives a **tolerance** instead of a step size. The returned points are not equally spaced: they are dense where the solution changes fast.

## 2. How it works

Each trial step from $(t, y)$ with step $h$ evaluates six slopes $k_1 \dots k_6$ (Fehlberg's coefficients). It combines them in two ways:

- $y_4 = y + \frac{25}{216}k_1 + \frac{1408}{2565}k_3 + \frac{2197}{4104}k_4 - \frac{1}{5}k_5$ (fourth order);
- $y_5 = y + \frac{16}{135}k_1 + \frac{6656}{12825}k_3 + \frac{28561}{56430}k_4 - \frac{9}{50}k_5 + \frac{2}{55}k_6$ (fifth order).

Both use the same six slopes, so the error estimate is almost free. Then:

1. $R = \frac{|y_5 - y_4|}{h}$ is the estimated error per unit time.
2. If $R \le \text{tol}$, the step is **accepted**: $t \leftarrow t + h$, $y \leftarrow y_4$, and the point is stored.
3. Either way, $h$ is rescaled by $q = 0.84 \left(\frac{\text{tol}}{R}\right)^{1/4}$, limited to $[0.1, 4]$ and to at most $t_{\text{end}} - t_0$. The exponent $1/4$ comes from the local error of the fourth-order formula; 0.84 is a safety factor.
4. The last step is shortened to land exactly on $t_{\text{end}}$.

This "estimate the error, then refine" idea is the same one that makes Romberg integration work. That is why George, who owns Romberg, implements it.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| Classic Fehlberg form (advance with $y_4$, $R$ per unit time) | It matches the textbook (Burden & Faires), so every line can be checked against the course notes. |
| $q$ limited to $[0.1, 4]$ | Prevents $h$ from collapsing or exploding after one unusual step. |
| Safeguards: $h \ge 10^{-12} \cdot \max(1, |t|)$, at most 1,000,000 trial steps | A singular problem ($y' = y^2$, blow-up at $t = 1$) ends with a clear `runtime_error` instead of looping for ever. |
| Shared checks (`check_ode_inputs`, `check_tolerance`, `check_finite_result`) | Same rules and messages as the other ODE solvers. |
| Returns every accepted point | The spacing shows where the method worked hard, which is useful in reports and in the app. |
| `FehlbergStep` struct + static `fehlberg_step` | One trial step is written once and returns both numbers it produces. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | $h_{\text{initial}} \le 0$, $t_{\text{end}} \le t_0$, $\text{tol} \le 0$ (any may be NaN), or $\frac{t_{\text{end}} - t_0}{h_{\text{initial}}} > 1,000,000$ |
| `std::runtime_error` | Step size below $10^{-12} \cdot |t|$, more than 1,000,000 trial steps, or a non-finite solution |

## 5. Limitations

- The tolerance controls the *local* error per unit time. The global error is usually of the same size, but this is not guaranteed.
- The right-hand side should be smooth. A jump in $f$ (for example a switch at $t = 10$) makes $R$ stay large near the jump, and the step size collapses. Example 2 models the parachute opening smoothly for this reason.
- Explicit method: stiff problems force very small steps.

## 6. Tests and examples

- **Tests (18 checks):** error below $\text{tol}$ for $10^{-4}$ and $10^{-8}$; more steps for a tighter $\text{tol}$; `rkf45_value`; a $t^4$ solution integrated exactly in at most two steps; step sizes grow on a fast-then-slow problem; first point, exact $t_{\text{end}}$, increasing times; invalid inputs; blow-up.
- **Example 1:** tolerance against error and number of steps ($10^{-3} \rightarrow 4$ steps, $10^{-9} \rightarrow 76$ steps), the step size growing from $0.001$ to $0.008$, `rkf45_value`, rejected inputs.
- **Example 2:** a skydiver's parachute opening (the smallest step is taken while it opens) and Michaelis-Menten drug elimination.