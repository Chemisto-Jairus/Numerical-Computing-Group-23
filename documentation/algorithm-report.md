# Algorithms Implemented in the Group 23 Numerical Computing Library

**Numerical Computing 2 · Group 23 · October 2026**
Chemisto Jairus · Batte Derrick Calvin · Aogon Sharon · Bakalu Valeria · Wasswa George Mpanga · Mwanje Ian · Muwanguzi Samuel · Kyeyune Praise Pauline · Nabakooza Bridget Maria

## 1. Introduction

The library `numcomp2` is a C++17 implementation of the eighteen algorithms on the course list, plus five related extras (Heun, RK3, RK4, LU decomposition and Chebyshev nodes). It is organised as fifteen stateless classes in namespace `numcomp`, covering seven areas: root finding, interpolation and approximation, differentiation, integration, initial value problems, boundary value problems and linear systems. For every algorithm this report states the method, its accuracy and cost, and the safeguards in our implementation. It ends with how the implementations were verified.

## 2. Root finding (f(x) = 0)

**Bisection** (`BasicRootFinder`) keeps the half of a bracket [a, b] with f(a)·f(b) < 0. After k steps the width is (b − a)/2<sup>k</sup>: linear but guaranteed convergence. Our code swaps reversed end points and rejects NaN values. **Newton-Raphson** (`BasicRootFinder`) uses x<sub>k+1</sub> = x<sub>k</sub> − f(x<sub>k</sub>)/f′(x<sub>k</sub>) and converges quadratically near a simple root. It stops with an error if |f′| < 10⁻¹². The derivative may be exact or numerical (Section 4). **Secant** (`AdvancedRootFinder`) replaces f′ by the slope through the last two iterates: order ≈ 1.618 with one evaluation per step. **Fixed-point iteration** (`AdvancedRootFinder`) iterates x = g(x) and converges linearly with rate |g′(x<sup>&#42;</sup>)| when this is below 1. For x² − x − 2 = 0, g(x) = √(x + 2) converges, while g(x) = x² − 2 diverges. All four validate τ > 0 and the iteration limit, and they throw `std::runtime_error` instead of returning an unconverged value.

## 3. Interpolation and approximation

**Piecewise linear interpolation** (`Interpolator`): p(x) = y<sub>i</sub> + (y<sub>i+1</sub> − y<sub>i</sub>)(x − x<sub>i</sub>)/h<sub>i</sub>, with error ≤ h²·max|f″|/8. The interval is found by binary search, and x outside the data is rejected. **Chebyshev nodes**: x<sub>k</sub> = (a + b)/2 + (b − a)/2·cos((2k − 1)π/2n), the roots of T<sub>n</sub> on [a, b]. They minimise max|∏(x − x<sub>k</sub>)| and so suppress Runge's phenomenon. For *piecewise linear* reconstruction, equal spacing is better (the error depends on the widest gap).

**Chebyshev approximation** (`Interpolator`). f is sampled at the n nodes (angles θ<sub>k</sub> = (2k + 1)π/2n) and c<sub>j</sub> = (2/n)Σ<sub>k</sub> f(x<sub>k</sub>) cos(jθ<sub>k</sub>). Then p(x) = c₀/2 + Σ<sub>j≥1</sub> c<sub>j</sub>T<sub>j</sub>(u) with u = (2x − a − b)/(b − a). p interpolates f at the nodes and is close to the minimax polynomial. It is evaluated with Clenshaw's recurrence b<sub>j</sub> = 2u·b<sub>j+1</sub> − b<sub>j+2</sub> + c<sub>j</sub> in O(n). For smooth f the coefficients decay geometrically: for eˣ on [0, 1] the error was 1.2·10⁻⁶ with 6 terms and 2.7·10⁻¹⁵ with 12. For the Runge function 1/(1 + 25x²), where equally spaced interpolation diverges, the error fell from 0.40 (degree 4) to 2.5·10⁻⁶ (degree 64).

**Natural cubic spline** (`CubicSpline`). The second derivatives M<sub>i</sub> satisfy the tridiagonal system h<sub>i−1</sub>M<sub>i−1</sub> + 2(h<sub>i−1</sub> + h<sub>i</sub>)M<sub>i</sub> + h<sub>i</sub>M<sub>i+1</sub> = 6[(y<sub>i+1</sub> − y<sub>i</sub>)/h<sub>i</sub> − (y<sub>i</sub> − y<sub>i−1</sub>)/h<sub>i−1</sub>], with M₀ = M<sub>n</sub> = 0. It is strictly diagonally dominant, so the Thomas algorithm solves it in O(n) without pivoting. The spline is fourth-order accurate.

## 4. Numerical differentiation (`NumericalDifferentiator`)

From the Taylor series f(x ± h) = f ± hf′ + h²f″/2 ± h³f‴/6 + …:

| Formula | Expression | Error |
|---|---|---|
| Forward / backward | (f(x + h) − f(x))/h,  (f(x) − f(x − h))/h | (h/2)·f″(ξ), order 1 |
| Central | (f(x + h) − f(x − h))/2h | (h²/6)·f‴(ξ), order 2 |
| Second derivative | (f(x + h) − 2f(x) + f(x − h))/h² | (h²/12)·f⁽⁴⁾(ξ), order 2 |
| Richardson | (4D(h/2) − D(h))/3, D = central | O(h⁴) |

Truncation error falls with h, but cancellation in f(x + h) − f(x − h) grows like ε/h. For the central difference of eˣ at 0 the error was 1.7·10⁻⁹ at h = 10⁻⁴, 2.7·10⁻¹¹ at 10⁻⁶, and 3.3·10⁻⁵ at 10⁻¹², so the best h is near ε<sup>1/3</sup> ≈ 10⁻⁵. Every result is checked to be finite.

## 5. Numerical integration

| Rule (h = (b − a)/n) | Formula | Error term | Exact for |
|---|---|---|---|
| Trapezoidal | h[½f₀ + f₁ + … + f<sub>n−1</sub> + ½f<sub>n</sub>] | −(b − a)h²f″(ξ)/12 | degree ≤ 1 |
| Midpoint | h Σ f(a + (i − ½)h) | +(b − a)h²f″(ξ)/24 | degree ≤ 1 |
| Simpson 1/3 (n even) | (h/3)[f₀ + 4f₁ + 2f₂ + … + 4f<sub>n−1</sub> + f<sub>n</sub>] | −(b − a)h⁴f⁽⁴⁾(ξ)/180 | degree ≤ 3 |

**Romberg** (`AdvancedIntegrator`). The trapezoidal error expands in even powers of h, so R<sub>i,j</sub> = (4<sup>j</sup>R<sub>i,j−1</sub> − R<sub>i−1,j−1</sub>)/(4<sup>j</sup> − 1) removes the h<sup>2j</sup> term. Column 0 reuses `BasicIntegrator::trapezoidal_rule` on 2<sup>i</sup> strips, and the table is capped at 25 rows. For ∫₀¹ eˣ dx to 10⁻¹⁰, Romberg needed 36 evaluations and Simpson 129.

## 6. Initial value problems (y′ = f(t, y), y(t₀) = y₀)

**One-step methods** (`BasicODESolver`, `AdvancedODESolver`). These are Euler (order 1), Heun (order 2), Kutta's RK3 and classic RK4. Their weights are (1), (½, ½), (1/6, 4/6, 1/6) and (1/6, 2/6, 2/6, 1/6) on 1, 2, 3 and 4 slopes per step. They share one fixed-step loop. It validates h > 0, t<sub>end</sub> > t₀ and at most 10⁶ steps, shortens the last step, and lands exactly on t<sub>end</sub>.

**Adams-Bashforth** (`AdamsBashforthSolver`, explicit multistep). The slope is extrapolated from stored values f<sub>k</sub> = f(t<sub>k</sub>, y<sub>k</sub>): AB2 y<sub>n+1</sub> = y<sub>n</sub> + (h/2)(3f<sub>n</sub> − f<sub>n−1</sub>) and AB4 y<sub>n+1</sub> = y<sub>n</sub> + (h/24)(55f<sub>n</sub> − 59f<sub>n−1</sub> + 37f<sub>n−2</sub> − 9f<sub>n−3</sub>). Each step needs **one** new evaluation of f. On one problem AB4 used 217 evaluations where RK4 used 800 at the same order.

**Adams-Moulton** (`AdamsMoultonSolver`, implicit). AM2 (the trapezoidal rule) y<sub>n+1</sub> = y<sub>n</sub> + (h/2)(f<sub>n+1</sub> + f<sub>n</sub>), and AM4 y<sub>n+1</sub> = y<sub>n</sub> + (h/24)(9f<sub>n+1</sub> + 19f<sub>n</sub> − 5f<sub>n−1</sub> + f<sub>n−2</sub>). The equation y = known + βh·f(t<sub>n+1</sub>, y) is solved by fixed-point iteration from the Euler predictor, to a relative tolerance of 10⁻¹². This converges when βh|∂f/∂y| < 1; otherwise a `runtime_error` asks for a smaller h. At equal h the error constants were about 5× (order 2) and 15× (order 4) smaller than Adams-Bashforth's. The methods are also much more stable: on y′ = −50(y − 1) with h = 0.03, AB2 grew to 1.7·10⁶ while AM2 stayed at 1.

Both Adams classes share a second loop that stores each slope once. It creates the first k − 1 starting values with RK4 (reusing `AdvancedODESolver`), so the start does not reduce the order. It also uses RK4 for a final step shorter than h.

**Runge-Kutta-Fehlberg** (`RungeKuttaFehlbergSolver`). Six slopes give a fourth-order value y₄ and a fifth-order value y₅. The difference R = |y₅ − y₄|/h estimates the error per unit time. A step is accepted when R ≤ tol, and h is rescaled by 0.84(tol/R)<sup>1/4</sup>, limited to [0.1, 4]. Safeguards stop the loop when h < 10⁻¹²|t| or after 10⁶ trial steps. On y′ = −2y the global error was 1.8·10⁻⁵, 2.1·10⁻⁷ and 2.2·10⁻⁹ for tol = 10⁻⁴, 10⁻⁶ and 10⁻⁸. Steps concentrate where the solution changes quickly.

## 7. Boundary value problems: finite difference method (`FiniteDifferenceSolver`)

For y″ = p(x)y′ + q(x)y + r(x) with y(a) = α and y(b) = β, the grid is x<sub>i</sub> = a + ih, h = (b − a)/n. The central differences of Section 4 replace y″ and y′ at each inner point:

−(1 + hp<sub>i</sub>/2)y<sub>i−1</sub> + (2 + h²q<sub>i</sub>)y<sub>i</sub> − (1 − hp<sub>i</sub>/2)y<sub>i+1</sub> = −h²r<sub>i</sub>,  i = 1 … n − 1,

and the known y₀ = α and y<sub>n</sub> = β move to the right-hand side. The system is tridiagonal and diagonally dominant when q ≥ 0 and h|p|/2 < 1. It is solved by the same Thomas routine as the spline, which was moved into a shared helper. The error is O(h²): for y″ = −π² sin πx the maximum errors for n = 10, 20, 40 and 80 were 8.3·10⁻³, 2.1·10⁻³, 5.1·10⁻⁴ and 1.3·10⁻⁴. `poisson` is the case p = q = 0.

## 8. Linear systems (A x = b)

**Gaussian elimination with partial pivoting** (`MatrixSolver`). For each column, the row with the largest |a<sub>ik</sub>| is swapped into the pivot position and the entries below it are eliminated. Back substitution then gives x. A pivot below 10⁻¹² means a singular matrix. **LU decomposition** records the same elimination as PA = LU. It then solves Ly = Pb and Ux = y. Both cost about n³/3 multiply-adds, and LU's factors can be reused.

**Conjugate gradient** (`ConjugateGradientSolver`) for symmetric positive definite A. Starting from r₀ = b − Ax₀ and p₀ = r₀, each iteration computes α = rᵀr/pᵀAp, x ← x + αp, r ← r − αAp, then β = r<sub>new</sub>ᵀr<sub>new</sub>/rᵀr and p ← r + βp. It stops when ‖r‖ ≤ tol·‖b‖. The directions are A-conjugate, so in exact arithmetic it terminates in at most n iterations. Each iteration costs one matrix-vector product, and A is never modified. Symmetry is checked first (`invalid_argument`). A non-positive pᵀAp proves A is not positive definite (`runtime_error`). For a 99-unknown heat-conduction system, CG reproduced x(1 − x)/2 to the printed digits.

## 9. Implementation design

Every class is stateless, and functions are passed as `std::function`, so any callable can be used. Logic used by more than one algorithm is written once:

- input checks and finite-result checks;
- the fixed-step ODE loop and the multistep loop (each method passes its formula as a function pointer);
- the tridiagonal solver.

Classes also reuse each other: Romberg calls the trapezoidal rule, Richardson calls the central difference, Chebyshev approximation calls `chebyshev_nodes`, and the Adams starters call RK4. Errors use the standard hierarchy: `invalid_argument` (bad parameter), `out_of_range` (point outside the data) and `runtime_error` (method failure). Comparisons are written as `!(tol > 0)` so that NaN is rejected.

## 10. Verification

Fifteen test programs (335 checks) cover normal use, edge cases and invalid input for every class. They all pass in Debug and Release builds with no compiler warnings, and 30 example programs also run under `ctest`. The observed orders were measured by halving the step, so each ratio should approach 2<sup>p</sup>:

| Method | p | Ratios | Method | p | Ratios |
|---|---|---|---|---|---|
| Euler | 1 | 1.92, 1.96, 1.98 | Forward difference | 1 | 2.03, 2.02, 2.01 |
| Heun | 2 | 3.85, 3.93, 3.96 | Central difference | 2 | 4.00, 4.00, 4.00 |
| RK3 | 3 | 9.4, 8.7, 8.3 | Richardson | 4 | 16.0, 16.0, 16.0 |
| RK4 | 4 | 18.9, 17.4, 16.7 | Trapezoidal | 2 | 4.03, 4.01, 4.00 |
| AB2 | 2 | 4.22, 3.90, 3.91 | Simpson | 4 | 16.9, 16.2, 16.1 |
| AB4 | 4 | 16.7, 15.4, 15.6 | Cubic spline | 4 | 16.9, 16.2, 16.1 |
| AM2 | 2 | 4.22, 3.90 | Finite differences | 2 | 4.01, 4.00, 4.00 |
| AM4 | 4 | 16.0, 14.8 | | | |

Exactness was also confirmed:

- Simpson for cubics, and Romberg for x⁴ with three rows;
- AB4 for cubic slopes and AM2 for linear slopes;
- the central difference for quadratics;
- Chebyshev approximation for cubics with four terms;
- the finite difference method for quadratic solutions.

## 11. Limitations

The ODE solvers handle single equations, and the multistep and RKF45 methods are explicit or use fixed-point correctors, so stiff problems need small steps. The finite difference solver handles linear problems with Dirichlet conditions. The matrix solvers store dense matrices, and CG is not preconditioned. Romberg recomputes each trapezoidal sum, and interpolation never extrapolates.

## References

1. R. L. Burden, J. D. Faires and A. M. Burden, *Numerical Analysis*, 10th ed., Cengage Learning, 2016.
2. S. C. Chapra and R. P. Canale, *Numerical Methods for Engineers*, 7th ed., McGraw-Hill, 2015.
3. W. H. Press et al., *Numerical Recipes: The Art of Scientific Computing*, 3rd ed., Cambridge University Press, 2007.
