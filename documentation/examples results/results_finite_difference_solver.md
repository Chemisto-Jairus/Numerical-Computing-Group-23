# Finite Difference Solver Examples Documentation

## 1. Linear Two-Point Boundary Value Problems (`example_finite_difference_solver.cpp`)

### Poisson Equation Baseline
* **Tested:** Solving a basic boundary value problem using the finite difference method.
* **Equation:** $y'' = -\pi^2 \sin(\pi x)$
* **Interval:** $[0, 1]$
* **Boundary Conditions:** $y(0) = 0, y(1) = 0$
* **Grid Intervals ($n$):** $10$
* **Expected Result:** Numerical solution matches the exact curve $y = \sin(\pi x)$.

### Order of Accuracy
* **Tested:** Error convergence rate as grid resolution increases.
* **Equation:** $y'' = -\pi^2 \sin(\pi x)$
* **Tested Grid Intervals ($n$):** $10, 20, 40, 80$
* **Expected Result:** The method demonstrates second-order accuracy; doubling $n$ divides the maximum error by approximately $4$.

### Variable Coefficients BVP
* **Tested:** Solving a linear boundary value problem with variable coefficients.
* **Equation:** $y'' = -\frac{2}{x} y' + \frac{2}{x^2} y + \frac{\sin(\ln x)}{x^2}$
* **Interval:** $[1, 2]$
* **Boundary Conditions:** $y(1) = 1, y(2) = 2$
* **Grid Intervals ($n$):** $10$
* **Expected Result:** Accurately matches the exact textbook analytical solution.

### Invalid Input Exceptions
* **Tested (Grid Size):** Rejection of a grid with only $1$ interval (no inner points).
* **Tested (Interval Bounds):** Rejection of an interval where the start is greater than the end ($a = 1, b = 0$).
* **Tested (Singularity):** Rejection of functions that evaluate to infinity at a grid node (e.g., $r(x) = \frac{1}{x}$ over the interval $[-1, 1]$).

---

## 2. Applied Boundary Value Problems (`example_finite_difference_solver_applied.cpp`)

### Cooling Fin Heat Transfer
* **Tested:** Calculating the steady temperature profile of a cooling fin that loses heat to the surrounding air.
* **Equation:** $T'' = 400(T - 25)$
* **Interval:** $[0.0, 0.1]$ meters
* **Boundary Conditions:** $T(0) = 100^\circ\text{C}, T(0.1) = 40^\circ\text{C}$
* **Grid Intervals ($n$):** $50$
* **Expected Result:** The numerical grid matches the analytical solution involving hyperbolic cosine and sine functions.

### Hanging Cable Sag
* **Tested:** Modeling the sag of a cable under its own uniform load.
* **Equation:** $y'' = 0.01$ (derived from load $50$ N/m over tension $5000$ N)
* **Interval:** $[0.0, 40.0]$ meters
* **Boundary Conditions:** $y(0) = 10.0, y(40) = 10.0$
* **Grid Intervals ($n$):** $40$
* **Expected Result:** Identifies the lowest point of the cable (at $x = 20$ m), which exactly matches the theoretical parabolic sag.