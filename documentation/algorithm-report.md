# Algorithms Implemented in the Group 23 Numerical Computing Library

**Numerical Computing 2 · Group 23 · October 2026**
Chemisto Jairus · Batte Derrick Calvin · Aogon Sharon · Bakalu Valeria · Wasswa George Mpanga · Mwanje Ian · Muwanguzi Samuel · Kyeyune Praise Pauline · Nabakooza Bridget Maria

## 1. Introduction

The library `numcomp2` is a C++17 implementation of the eighteen algorithms on the course list, plus five related extras (Heun, RK3, RK4, LU decomposition and Chebyshev nodes). It is organised as fifteen stateless classes in namespace `numcomp`, covering seven areas: root finding, interpolation and approximation, differentiation, integration, initial value problems, boundary value problems and linear systems. For every algorithm this report states the method, its accuracy and cost, and the safeguards in our implementation. It ends with how the implementations were verified.

## 2. Root finding ($f(x) = 0$)

**Bisection** (`BasicRootFinder`) keeps the half of a bracket $[a, b]$ with $f(a) \cdot f(b) < 0$. After $k$ steps the width is $\frac{b - a}{2^k}$: linear but guaranteed convergence. Our code swaps reversed end points and rejects NaN values. **Newton-Raphson** (`BasicRootFinder`) uses $x_{k+1} = x_k - \frac{f(x_k)}{f'(x_k)}$ and converges quadratically near a simple root. It stops with an error if $|f'| < 10^{-12}$. The derivative may be exact or numerical (Section 4). **Secant** (`AdvancedRootFinder`) replaces $f'$ by the slope through the last two iterates: order $\approx 1.618$ with one evaluation per step. **Fixed-point iteration** (`AdvancedRootFinder`) iterates $x = g(x)$ and converges linearly with rate $|g'(x^*)|$ when this is below 1. For $x^2 - x - 2 = 0$, $g(x) = \sqrt{x + 2}$ converges, while $g(x) = x^2 - 2$ diverges. All four validate $\tau > 0$ and the iteration limit, and they throw `std::runtime_error` instead of returning an unconverged value.

## 3. Interpolation and approximation

**Piecewise linear interpolation** (`Interpolator`): $p(x) = y_i + \frac{y_{i+1} - y_i}{h_i}(x - x_i)$, with error $\le \frac{h^2 \max|f''|}{8}$. The interval is found by binary search, and $x$ outside the data is rejected. **Chebyshev nodes**: $x_k = \frac{a + b}{2} + \frac{b - a}{2}\cos\left(\frac{(2k - 1)\pi}{2n}\right)$, the roots of $T_n$ on $[a, b]$. They minimise $\max|\prod(x - x_k)|$ and so suppress Runge's phenomenon. For *piecewise linear* reconstruction, equal spacing is better (the error depends on the widest gap).

**Chebyshev approximation** (`Interpolator`). $f$ is sampled at the $n$ nodes (angles $\theta_k = \frac{(2k + 1)\pi}{2n}$) and $c_j = \frac{2}{n}\sum_k f(x_k) \cos(j\theta_k)$. Then $p(x) = \frac{c_0}{2} + \sum_{j \ge 1} c_j T_j(u)$ with $u = \frac{2x - a - b}{b - a}$. $p$ interpolates $f$ at the nodes and is close to the minimax polynomial. It is evaluated with Clenshaw's recurrence $b_j = 2u b_{j+1} - b_{j+2} + c_j$ in $O(n)$. For smooth $f$ the coefficients decay geometrically: for $e^x$ on $[0, 1]$ the error was $1.2 \times 10^{-6}$ with 6 terms and $2.7 \times 10^{-15}$ with 12. For the Runge function $\frac{1}{1 + 25x^2}$, where equally spaced interpolation diverges, the error fell from $0.40$ (degree 4) to $2.5 \times 10^{-6}$ (degree 64).

**Natural cubic spline** (`CubicSpline`). The second derivatives $M_i$ satisfy the tridiagonal system $h_{i-1}M_{i-1} + 2(h_{i-1} + h_i)M_i + h_iM_{i+1} = 6\left[ \frac{y_{i+1} - y_i}{h_i} - \frac{y_i - y_{i-1}}{h_{i-1}} \right]$, with $M_0 = M_n = 0$. It is strictly diagonally dominant, so the Thomas algorithm solves it in $O(n)$ without pivoting. The spline is fourth-order accurate.

## 4. Numerical differentiation (`NumericalDifferentiator`)

From the Taylor series $f(x \pm h) = f \pm hf' + \frac{h^2}{2}f'' \pm \frac{h^3}{6}f''' + \dots$:

| Formula | Expression | Error |
|---|---|---|
| Forward / backward | $\frac{f(x + h) - f(x)}{h}$,  $\frac{f(x) - f(x - h)}{h}$ | $\frac{h}{2}f''(\xi)$, order 1 |
| Central | $\frac{f(x + h) - f(x - h)}{2h}$ | $\frac{h^2}{6}f'''(\xi)$, order 2 |
| Second derivative | $\frac{f(x + h) - 2f(x) + f(x - h)}{h^2}$ | $\frac{h^2}{12}f^{(4)}(\xi)$, order 2 |
| Richardson | $\frac{4D(h/2) - D(h)}{3}$, $D = \text{central}$ | $O(h^4)$ |

Truncation error falls with $h$, but cancellation in $f(x + h) - f(x - h)$ grows like $\frac{\varepsilon}{h}$. For the central difference of $e^x$ at 0 the error was $1.7 \times 10^{-9}$ at $h = 10^{-4}$, $2.7 \times 10^{-11}$ at $10^{-6}$, and $3.3 \times 10^{-5}$ at $10^{-12}$, so the best $h$ is near $\varepsilon^{1/3} \approx 10^{-5}$. Every result is checked to be finite.

## 5. Numerical integration

| Rule ($h = \frac{b - a}{n}$) | Formula | Error term | Exact for |
|---|---|---|---|
| Trapezoidal | $h\left[ \frac{1}{2}f_0 + f_1 + \dots + f_{n-1} + \frac{1}{2}f_n \right]$ | $-\frac{(b - a)h^2}{12} f''(\xi)$ | degree $\le 1$ |
| Midpoint | $h \sum f(a + (i - \frac{1}{2})h)$ | $+\frac{(b - a)h^2}{24} f''(\xi)$ | degree $\le 1$ |
| Simpson 1/3 ($n$ even) | $\frac{h}{3}\left[ f_0 + 4f_1 + 2f_2 + \dots + 4f_{n-1} + f_n \right]$ | $-\frac{(b - a)h^4}{180} f^{(4)}(\xi)$ | degree $\le 3$ |

**Romberg** (`AdvancedIntegrator`). The trapezoidal error expands in even powers of $h$, so $R_{i,j} = \frac{4^j R_{i,j-1} - R_{i-1,j-1}}{4^j - 1}$ removes the $h^{2j}$ term. Column 0 reuses `BasicIntegrator::trapezoidal_rule` on $2^i$ strips, and the table is capped at 25 rows. For $\int_0^1 e^x \, dx$ to $10^{-10}$, Romberg needed 36 evaluations and Simpson 129.

## 6. Initial value problems ($y' = f(t, y)$, $y(t_0) = y_0$)

**One-step methods** (`BasicODESolver`, `AdvancedODESolver`). These are Euler (order 1), Heun (order 2), Kutta's RK3 and classic RK4. Their weights are $(1)$, $(\frac{1}{2}, \frac{1}{2})$, $(\frac{1}{6}, \frac{4}{6}, \frac{1}{6})$ and $(\frac{1}{6}, \frac{2}{6}, \frac{2}{6}, \frac{1}{6})$ on 1, 2, 3 and 4 slopes per step. They share one fixed-step loop. It validates $h > 0$, $t_{\text{end}} > t_0$ and at most $10^6$ steps, shortens the last step, and lands exactly on $t_{\text{end}}$.

**Adams-Bashforth** (`AdamsBashforthSolver`, explicit multistep). The slope is extrapolated from stored values $f_k = f(t_k, y_k)$: AB2 $y_{n+1} = y_n + \frac{h}{2}(3f_n - f_{n-1})$ and AB4 $y_{n+1} = y_n + \frac{h}{24}(55f_n - 59f_{n-1} + 37f_{n-2} - 9f_{n-3})$. Each step needs **one** new evaluation of $f$. On one problem AB4 used 217 evaluations where RK4 used 800 at the same order.

**Adams-Moulton** (`AdamsMoultonSolver`, implicit). AM2 (the trapezoidal rule) $y_{n+1} = y_n + \frac{h}{2}(f_{n+1} + f_n)$, and AM4 $y_{n+1} = y_n + \frac{h}{24}(9f_{n+1} + 19f_n - 5f_{n-1} + f_{n-2})$. The equation $y = \text{known} + \beta h \cdot f(t_{n+1}, y)$ is solved by fixed-point iteration from the Euler predictor, to a relative tolerance of $10^{-12}$. This converges when $\beta h \left|\frac{\partial f}{\partial y}\right| < 1$; otherwise a `runtime_error` asks for a smaller $h$. At equal $h$ the error constants were about 5× (order 2) and 15× (order 4) smaller than Adams-Bashforth's. The methods are also much more stable: on $y' = -50(y - 1)$ with $h = 0.03$, AB2 grew to $1.7 \times 10^6$ while AM2 stayed at 1.

Both Adams classes share a second loop that stores each slope once. It creates the first $k - 1$ starting values with RK4 (reusing `AdvancedODESolver`), so the start does not reduce the order. It also uses RK4 for a final step shorter than $h$.

**Runge-Kutta-Fehlberg** (`RungeKuttaFehlbergSolver`). Six slopes give a fourth-order value $y_4$ and a fifth-order value $y_5$. The difference $R = \frac{|y_5 - y_4|}{h}$ estimates the error per unit time. A step is accepted when $R \le \text{tol}$, and $h$ is rescaled by $0.84\left(\frac{\text{tol}}{R}\right)^{1/4}$, limited to $[0.1, 4]$. Safeguards stop the loop when $h < 10^{-12}|t|$ or after $10^6$ trial steps. On $y' = -2y$ the global error was $1.8 \times 10^{-5}$, $2.1 \times 10^{-7}$ and $2.2 \times 10^{-9}$ for $\text{tol} = 10^{-4}$, $10^{-6}$ and $10^{-8}$. Steps concentrate where the solution changes quickly.

## 7. Boundary value problems: finite difference method (`FiniteDifferenceSolver`)

For $y'' = p(x)y' + q(x)y + r(x)$ with $y(a) = \alpha$ and $y(b) = \beta$, the grid is $x_i = a + ih$, $h = \frac{b - a}{n}$. The central differences of Section 4 replace $y''$ and $y'$ at each inner point:

$-\left(1 + \frac{hp_i}{2}\right)y_{i-1} + (2 + h^2 q_i)y_i - \left(1 - \frac{hp_i}{2}\right)y_{i+1} = -h^2 r_i, \quad i = 1 \dots n - 1$,

and the known $y_0 = \alpha$ and $y_n = \beta$ move to the right-hand side. The system is tridiagonal and diagonally dominant when $q \ge 0$ and $\frac{h|p|}{2} < 1$. It is solved by the same Thomas routine as the spline, which was moved into a shared helper. The error is $O(h^2)$: for $y'' = -\pi^2 \sin(\pi x)$ the maximum errors for $n = 10, 20, 40$ and $80$ were $8.3 \times 10^{-3}$, $2.1 \times 10^{-3}$, $5.1 \times 10^{-4}$ and $1.3 \times 10^{-4}$. `poisson` is the case $p = q = 0$.

## 8. Linear systems ($Ax = b$)

**Gaussian elimination with partial pivoting** (`MatrixSolver`). For each column, the row with the largest $|a_{ik}|$ is swapped into the pivot position and the entries below it are eliminated. Back substitution then gives $x$. A pivot below $10^{-12}$ means a singular matrix. **LU decomposition** records the same elimination as $PA = LU$. It then solves $Ly = Pb$ and $Ux = y$. Both cost about $O(n^3)$ multiply-adds, and LU's factors can be reused.

**Conjugate gradient** (`ConjugateGradientSolver`) for symmetric positive definite $A$. Starting from $r_0 = b - Ax_0$ and $p_0 = r_0$, each iteration computes $\alpha = \frac{r^T r}{p^T A p}$, $x \leftarrow x + \alpha p$, $r \leftarrow r - \alpha A p$, then $\beta = \frac{r_{\text{new}}^T r_{\text{new}}}{r^T r}$ and $p \leftarrow r + \beta p$. It stops when $\|r\| \le \text{tol} \cdot \|b\|$. The directions are $A$-conjugate, so in exact arithmetic it terminates in at most $n$ iterations. Each iteration costs one matrix-vector product, and $A$ is never modified. Symmetry is checked first (`invalid_argument`). A non-positive $p^T A p$ proves $A$ is not positive definite (`runtime_error`). For a 99-unknown heat-conduction system, CG reproduced $\frac{x(1 - x)}{2}$ to the printed digits.

## 9. Implementation design

## Stateless Design in Numerical Computing

The stateless design was a highly effective architectural choice for the scope of this project because it heavily prioritizes code safety, predictability, and simplicity over extreme optimization. Software design is always a trade-off, and for a general-purpose numerical library aimed at small-to-medium datasets, this approach offers distinct advantages alongside a few performance penalties.

### Why it was a good idea

* **Guaranteed Safety:** Because the classes hold no private data, there is zero risk of stale variables corrupting subsequent calculations. Every function call starts with a clean slate, and inputs are safely passed by constant reference and copied internally so the caller's original data is never altered.

* **Architectural Uniformity:** All 15 classes in the library follow the exact same predictable pattern. Users only ever need to initialize a default constructor and pass their data directly to the method, making the API exceptionally easy to learn and test.

* **Aligned with the Scope:** The library is explicitly meant for small, dense systems (tens to a few hundreds of unknowns). At this scale, the CPU overhead required to recalculate an $O(n)$ tridiagonal system or an $O(n^3)$ matrix from scratch is negligible.

### Where it sacrifices efficiency

* **Redundant LU Factorization:** The primary mathematical advantage of LU decomposition is factoring matrix A into L and U just once, which allows you to solve for many different b vectors in $O(n^2)$ time rather than $O(n^3)$. Because `MatrixSolver` is stateless, it discards L and U after every call, forcing a full refactorization every time the method is used.

* **Repeated Spline Generation:** The `CubicSpline` module must re-run the Thomas algorithm to calculate the second derivatives (M) from scratch on every single interpolate call. Evaluating 1,000 points on the same curve forces the $O(n)$ algorithm to run 1,000 redundant times instead of just once.

### How Industry Libraries Handle This

Most industry-standard numerical computing libraries on the internet—such as SciPy, Eigen, and scikit-learn—primarily use a **stateful design** to maximize performance on large datasets. While stateless utility functions exist for one-off calculations, professional libraries store expensive intermediate mathematical results in memory to avoid redundant computation.

* **Python's SciPy:** In `scipy.interpolate.CubicSpline`, creating the spline object calculates and stores the polynomial coefficients as its state. When you evaluate the curve later, it strictly performs the fast $O(\log n)$ lookup and evaluation, bypassing the tridiagonal matrix math entirely.

* **C++ Eigen Library:** For linear algebra, Eigen uses stateful solver objects. If you initialize `Eigen::PartialPivLU`, the object computes and stores the LU decomposition. You can then call `.solve(b)` thousands of times in $O(n^2)$ time without ever repeating the heavy $O(n^3)$ factorization.

* **Scikit-Learn:** Data modeling libraries rely almost entirely on the stateful `fit()` and `predict()` pattern. The training phase saves the calculated weights as the object's state, which the prediction phase then applies to new inputs.

While a stateless approach guarantees safe memory handling and prevents side effects, real-world libraries accept the complexity of managing state because recalculating massive mathematical systems from scratch is computationally prohibitive.

### When do they choose which?

Real-world libraries do not use stateful classes for absolutely every module. They take a pragmatic, mixed approach, reserving stateful design specifically for the modules where it provides a mathematical or performance benefit.

### 1. When they use Stateful Classes

Libraries use stateful classes when a calculation involves an expensive "setup" phase that can be reused later.

* **Matrix Solvers:** Factorizing a matrix (like LU or Cholesky decomposition) is computationally heavy ($O(n^3)$). Libraries use objects to store those factored matrices in memory so subsequent solves against new vectors are extremely fast ($O(n^2)$).

* **Machine Learning Models:** Training a model takes significant time and computes thousands of weights and coefficients. Those weights *are* the state.

* **Spline Interpolation:** Calculating the second derivatives takes $O(n)$ time. Spline objects calculate these once and save them so that looking up 10,000 different curve points later is instantaneous.

### 2. When they use Stateless Functions

They stick to simple, stateless functions when the calculation is a one-off operation or where there is no "setup" phase to save (exactly like your library's approach).

* **Root Finding (Bisection/Newton):** Finding the root of an equation is a one-time iterative loop. There are no heavy matrices to decompose and save, so SciPy just provides a standard function (`scipy.optimize.root_scalar`), which works exactly like your stateless `BasicRootFinder`.

* **Basic Numerical Integration:** Calculating the area under a curve using Simpson's or the Trapezoidal rule does not require saving intermediate states. SciPy provides functions like `scipy.integrate.simpson` that calculate the area from scratch and return the answer, leaving no memory trace behind (exactly like your `BasicIntegrator`).

Every class is stateless, and functions are passed as `std::function`, so any callable can be used. Logic used by more than one algorithm is written once:

- input checks and finite-result checks;
- the fixed-step ODE loop and the multistep loop (each method passes its formula as a function pointer);
- the tridiagonal solver.

Classes also reuse each other: Romberg calls the trapezoidal rule, Richardson calls the central difference, Chebyshev approximation calls `chebyshev_nodes`, and the Adams starters call RK4. Errors use the standard hierarchy: `invalid_argument` (bad parameter), `out_of_range` (point outside the data) and `runtime_error` (method failure). Comparisons are written as `!(tol > 0)` so that NaN is rejected.

## 10. Verification

Fifteen test programs (335 checks) cover normal use, edge cases and invalid input for every class. They all pass in Debug and Release builds with no compiler warnings, and 30 example programs also run under `ctest`. The observed orders were measured by halving the step, so each ratio should approach $2^p$:

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

- Simpson for cubics, and Romberg for $x^4$ with three rows;
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

## References for state design

* SciPy Documentation - Cubic Spline Interpolation: *scipy.interpolate.CubicSpline*
* SciPy Documentation - Root Finding: *scipy.optimize.root_scalar*
* SciPy Documentation - Integration: *scipy.integrate.simpson*
* Eigen C++ Library Documentation - Linear Algebra and Decompositions (PartialPivLU)
* Scikit-Learn API Reference - The Estimator API (fit and predict pattern)