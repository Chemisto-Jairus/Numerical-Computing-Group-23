# AdamsMoultonSolver: implicit multistep methods AM2 and AM4

| | |
|---|---|
| Owner | Muwanguzi Samuel (Member 7) |
| Header | `include/numerical-computing/adams_moulton_solver.hpp` |
| Source | `src/adams_moulton_solver.cpp` |
| Shared loop | `src/internal/multistep.hpp/.cpp` (`ode::run_multistep`) |
| Tests | `tests/test_adams_moulton_solver.cpp` (25 checks) |
| Examples | `examples/example_adams_moulton_solver.cpp`, `examples/example_adams_moulton_solver_applied.cpp` |

## 1. What it does

| Method | Formula | Error |
|---|---|---|
| `am2(f, t0, y0, t_end, h)` (trapezoidal rule) | y_(n+1) = y_n + h/2·(f_(n+1) + f_n) | ~ h² |
| `am4(f, t0, y0, t_end, h)` | y_(n+1) = y_n + h/24·(9f_(n+1) + 19f_n − 5f_(n−1) + f_(n−2)) | ~ h⁴ |

f_(n+1) = f(t_(n+1), y_(n+1)) contains the unknown y_(n+1), so the methods are **implicit**.

## 2. How it works

Adams-Moulton fits the interpolating polynomial through the slopes at the new point *and* the old points. Using the new point makes the polynomial more accurate over the step. Example 1 shows the error constants are about 5× (order 2) and 15× (order 4) smaller than Adams-Bashforth's at the same h. It also makes the method far more stable.

**Solving for y_(n+1).** Both formulas have the form

  y_(n+1) = known + β·h·f(t_(n+1), y_(n+1)),  with β = 1/2 (AM2) or 9/24 (AM4).

`solve_corrector` solves this by fixed-point iteration (like `fixed_point_iteration` in AdvancedRootFinder):
1. Start from the explicit Euler guess y_n + h·f_n (the *predictor*).
2. Repeat y ← known + βh·f(t_(n+1), y) (the *corrector*) until two values agree to 1e-12 relative to |y|.
3. If 100 iterations are not enough, throw `runtime_error`.

The iteration converges when βh·|∂f/∂y| < 1. For y' = −50y with h = 0.1, this is 0.5·0.1·50 = 2.5 > 1. The step is too large, and the error message says to reduce h.

**Starting values.** AM4 uses f_(n−2), so the shared loop makes the first two points with RK4. AM2 needs only f_n and starts at once.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| Corrector solved to convergence, not a fixed number of passes | The result is the true AM value, so its order and stability are the textbook ones. |
| Predictor = Euler step, not Adams-Bashforth | Keeps the class independent of `AdamsBashforthSolver`. The predictor only affects how many corrector passes are needed, not the result. |
| One `solve_corrector` for both methods | The two formulas differ only in `known` and β, so the iteration is written once. |
| Uses `ode::run_multistep` | Same starting values, checks and landing on t_end as Adams-Bashforth; only the formula is new. |
| `runtime_error` when the corrector fails | The input is valid but h is too large for this problem. The user is told what to change. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | h ≤ 0, t_end ≤ t0 (either may be NaN), more than 1,000,000 steps |
| `std::runtime_error` | The corrector iteration does not converge (h too large); the solution stops being finite |

## 5. Limitations

- Fixed-point iteration limits h for stiff problems. A Newton iteration would remove that limit, but it needs ∂f/∂y.
- Each step costs 1 + (number of corrector passes) evaluations of f, usually 3 to 6.
- Fixed step size.

## 6. Tests and examples

- **Tests (25 checks):** decay, y' = 2t (AM2 exact), y' = y − t; orders ≈ 4 and ≈ 16; 11 points for h = 0.1; uneven h; one step bigger than the interval; six invalid inputs per method; corrector failure for a stiff problem with h = 0.1 and success with h = 0.01.
- **Example 1:** AM2/AM4 table, implicit against explicit errors, a fast-decaying problem where AB2 explodes and AM2 stays at 1, rejected inputs.
- **Example 2:** a second-order chemical reaction and a tank draining by Torricelli's law.
