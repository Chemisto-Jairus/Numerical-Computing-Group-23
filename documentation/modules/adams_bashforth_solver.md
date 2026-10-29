# AdamsBashforthSolver: explicit multistep methods AB2 and AB4

| | |
|---|---|
| Owner | Mwanje Ian (Member 6) |
| Header | `include/numerical-computing/adams_bashforth_solver.hpp` |
| Source | `src/adams_bashforth_solver.cpp` |
| Shared loop | `src/internal/multistep.hpp/.cpp` (`ode::run_multistep`) |
| Tests | `tests/test_adams_bashforth_solver.cpp` (26 checks) |
| Examples | `examples/example_adams_bashforth_solver.cpp`, `examples/example_adams_bashforth_solver_applied.cpp` |

## 1. What it does

Both methods solve dy/dt = f(t, y), y(t0) = y0 on [t0, t_end] with a fixed step h. Like the other ODE solvers, they return the curve as (t, y) pairs.

| Method | Formula | Error | New f evaluations per step |
|---|---|---|---|
| `ab2(f, t0, y0, t_end, h)` | y_(n+1) = y_n + h/2·(3f_n − f_(n−1)) | ~ h² | 1 |
| `ab4(f, t0, y0, t_end, h)` | y_(n+1) = y_n + h/24·(55f_n − 59f_(n−1) + 37f_(n−2) − 9f_(n−3)) | ~ h⁴ | 1 |

Here f_k = f(t_k, y_k) is the slope at an earlier point.

## 2. How it works

A one-step method such as RK4 computes four slopes inside every step and then throws them away. A **multistep** method keeps the slopes at the last few points. It fits a polynomial through them and integrates that polynomial over the next step:

  y(t_(n+1)) = y(t_n) + ∫ f(t, y(t)) dt  over [t_n, t_(n+1)].

Fitting a straight line through f_(n−1), f_n gives AB2. Fitting a cubic through four slopes gives AB4. Only f_(n+1) is new, so each step costs **one** evaluation of f, against four for RK4 (example 1: 217 against 800 evaluations for the same problem).

The weights (3, −1)/2 and (55, −59, 37, −9)/24 are the integrals of the Lagrange basis polynomials. They are fixed numbers, written straight into `ab2_step` and `ab4_step`.

**Starting values.** AB4 needs four slopes, but at t0 only one exists. The shared loop creates the first three points with RK4, which has the same order, so the start does not spoil the accuracy. A last step shorter than h also uses RK4, because the AB weights assume equal spacing.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| Uses `ode::run_multistep` (written once, shared with Adams-Moulton) | Starting values, the stored slopes, input checks, the short last step and landing on t_end are the same for every multistep method. Each class only supplies a formula, like `run_fixed_steps` for Euler … RK4. |
| Starter = `AdvancedODESolver::rk4` | Reuses the existing, tested RK4 instead of writing it again. |
| Slopes stored once per point | Keeps the cost at one f evaluation per step, which is the whole point of the method. |
| Same signature and return type as the other solvers | Programs can switch method by changing one name; the app and tests treat all solvers the same way. |
| Result checked for finiteness | A blow-up (for example y' = y², which reaches infinity at t = 1) throws `runtime_error` instead of returning inf. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | h ≤ 0, t_end ≤ t0 (either may be NaN), more than 1,000,000 steps |
| `std::runtime_error` | The solution stops being a finite number |

## 5. Limitations

- Explicit methods have small stability regions. For fast-decaying (stiff) problems h must be small; example 1 of Adams-Moulton shows AB2 exploding with h = 0.03 on y' = −50(y − 1).
- Fixed step size. Use `RungeKuttaFehlbergSolver` when the solution changes speed.

## 6. Tests and examples

- **Tests (26 checks):** decay, polynomial slopes (AB2 exact for slopes linear in t, AB4 for cubic), −sin t; observed orders ≈ 4 and ≈ 16 when h is halved; 11 points for h = 0.1; a run shorter than the starter; uneven h; an interval not starting at 0; six invalid inputs for each method; blow-up.
- **Example 1:** AB2/AB4 table, order table, evaluation count against RK4, rejected inputs.
- **Example 2:** a skydiver reaching terminal velocity (exact tanh solution) and logistic bacterial growth.
