# Advanced ODE Solver Tests Documentation

## 1. Unit Tests (`test_advanced_ode_solver.cpp`)

### Accuracy on Standard Problems

* **Tested (Polynomial Exactness):** $dy/dt = 4t^3$, $y(0) = 0.0$
  * **Interval:** $[0.0, 2.0]$ with $h = 0.1$
  * **Expected Value:** $16.0$ exactly. Because the function depends only on $t$, RK3 and RK4 act like Simpson's rule and are exact for cubic polynomials.
* **Tested (Trigonometric):** $dy/dt = -\sin(t)$, $y(0) = 1.0$
  * **Interval:** $[0.0, \pi]$ with $h = 0.01$
  * **Expected Value:** $-1.0$ (converges closely for both RK3 and RK4).
* **Tested (Exponential Decay):** $dy/dt = -2y$, $y(0) = 1.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.1$
  * **Expected Value:** Converges closely to the exact value $e^{-2}$.

### Observed Order of Convergence

* **Tested:** Error reduction scaling when halving the step size ($h$) from $0.1$ to $0.05$ on the exponential decay problem.
* **Expected Result (RK3):** Halving $h$ divides the error by approximately 8 (measured ratio strictly between 7.0 and 9.0), demonstrating third-order accuracy.
* **Expected Result (RK4):** Halving $h$ divides the error by approximately 16 (measured ratio strictly between 14.0 and 18.0), demonstrating fourth-order accuracy.

### Boundaries and Curve Shape

* **Tested (Vector Shape):** RK4 with $h = 0.1$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The first point is exactly $(t_0, y_0)$, the returned vector contains exactly 11 points, and the final time value is exactly $t_{end}$.
* **Tested (Uneven Steps):** RK3 with $h = 0.3$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The final step is properly shortened to end exactly at $t = 1.0$, returning a total of 5 points.
* **Tested (Oversized Step):** RK4 with $h = 5.0$ on interval $[0.0, 1.0]$ (step is larger than the entire interval).
  * **Expected Result:** Gracefully completes in a single step to $t_{end}$, returning exactly 2 points.

### Invalid Input Exceptions

* **Tested (Invalid parameters):** Rejection of $h = 0$, $h < 0$, $h = \text{NaN}$, $t_{end} = t_0$, $t_{end} < t_0$, and requested intervals requiring too many steps (e.g., $h = 10^{-7}$).
  * **Expected Result:** Both RK3 and RK4 correctly intercept these inputs and throw `std::invalid_argument`.