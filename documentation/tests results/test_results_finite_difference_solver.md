# Finite Difference Solver Tests Documentation

## 1. Unit Tests (`test_finite_difference_solver.cpp`)

### Accuracy on Standard Problems

* **Tested (Poisson - Trigonometric):** $y'' = -\pi^2 \sin(\pi x)$
  * **Boundary Conditions:** $y(0) = 0.0$, $y(1) = 0.0$
  * **Interval:** $[0.0, 1.0]$ with $n = 100$
  * **Expected Result:** Maximum error compared to exact solution $y = \sin(\pi x)$ is below $10^{-4}$.

* **Tested (Poisson - Quadratic Exactness):** $y'' = 2$
  * **Boundary Conditions:** $y(0) = 0.0$, $y(1) = 1.0$
  * **Interval:** $[0.0, 1.0]$ with $n = 10$
  * **Expected Result:** Exact match with $y = x^2$ (error $< 10^{-12}$). Central difference methods are exactly correct for quadratic functions.

* **Tested (Linear BVP - Exponential/Hyperbolic):** $y'' = y$ (where $p=0, q=1, r=0$)
  * **Boundary Conditions:** $y(0) = 0.0$, $y(1) = \sinh(1)$
  * **Interval:** $[0.0, 1.0]$ with $n = 50$
  * **Expected Result:** Maximum error compared to exact solution $y = \sinh(x)$ is below $10^{-5}$.

* **Tested (Linear BVP - Complex Textbook Problem):** $y'' = -(2/x)y' + (2/x^2)y + \sin(\ln x)/x^2$
  * **Boundary Conditions:** $y(1) = 1.0$, $y(2) = 2.0$
  * **Interval:** $[1.0, 2.0]$ with $n = 10$
  * **Expected Result:** Maximum error compared to the textbook exact solution is below $10^{-4}$.

### Observed Order of Convergence

* **Tested:** Error reduction scaling when doubling the number of subintervals ($n$) from $20$ to $40$ on the trigonometric Poisson problem.
* **Expected Result:** Doubling $n$ divides the maximum error by approximately 4 (measured ratio strictly between 3.8 and 4.2), demonstrating second-order accuracy.

### Boundaries and Grid Structure

* **Tested (Grid Point Generation):** Poisson equation $y'' = 0$ on $[-1.0, 3.0]$ with boundary values $y(-1.0) = 5.0$, $y(3.0) = -3.0$, and $n = 8$.
  * **Expected Result:** The grid contains exactly 9 points. The first point is exactly $(-1.0, 5.0)$ and the last is exactly $(3.0, -3.0)$. The midpoint evaluates exactly to the straight line interpolation between the boundaries.

* **Tested (Smallest Possible Grid):** Poisson equation $y'' = 2$ on $[0.0, 2.0]$ with boundaries $y(0)=0.0, y(2)=4.0$, and $n = 2$ (meaning exactly 1 inner point).
  * **Expected Result:** Returns a grid of 3 points where the single inner point accurately computes $y(1) = 1.0$.

### Invalid Input and Singular Systems

* **Tested (Invalid Grid & Domains):** Rejection of $n = 0$, $n = 1$ (no inner points), $n > 1,000,000$, $a = b$, $a > b$, $a = \text{NaN}$, $\alpha = \infty$, and $\beta = \text{NaN}$.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Function Singularity):** Evaluating $r(x) = 1/x$ on $[-1.0, 1.0]$, forcing a grid evaluation exactly at $x = 0$.
  * **Expected Result:** Throws `std::runtime_error` due to the mathematical singularity.

* **Tested (Singular Matrix System):** Linear BVP configured such that the central diagonal entry $2 + h^2 q$ becomes exactly zero ($h = 0.5, q = -8$).
  * **Expected Result:** Throws `std::runtime_error` because the underlying tridiagonal matrix system cannot be solved (zero pivot).